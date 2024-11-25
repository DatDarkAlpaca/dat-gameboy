#include "pch.hpp"
#include "application.hpp"
#include "graphics/ui.hpp"

namespace dat
{
	s_DatApplication::s_DatApplication()
	{
		// Window:
		setup_opengl_windowing(
			window,
			WindowProperties{ "DatGameboy v1.0", { 800, 600 } },
			{ GraphicsAPI::OPENGL, 3, 3 }
		);

		window.set_event_callback(BIND(&s_DatApplication::on_event, this));

		cartridgeHolder = dat::make_unique<s_CartridgeHolder>();
		cartridgeHolder->set_event_callback(BIND(&s_DatApplication::on_event, this));

		// Renderer:
		mainRenderer = dat::make_unique<s_QuadRenderer>(std::vector<ShaderDescriptor> {
			ShaderDescriptor{ "res/shaders/basic.vert", ShaderType::VERTEX },
			ShaderDescriptor{ "res/shaders/basic.frag", ShaderType::FRAGMENT },
		});

		initialize_assets();

		// Gameboy:
		{
			auto json = dat::load_json_file("res/opcodes.json");
			initialize_instruction_set(json);

			gameboy = dat::make_unique<s_Gameboy>();
			gameboy->load_bootloader_rom(read_ROM("res/DMG_ROM.bin"));
		}

		// UI:
		dat::setup_ui(window);
		initialize_ui();
	}

	void s_DatApplication::run()
	{
		while (window.is_open() && m_IsOpen)
		{
			window.poll_events();

			on_update();

			on_render();

			present(window);
		}

		window.shutdown();
	}

	void s_DatApplication::on_event(Event& event)
	{
		Subscriber subscriber(event);

		subscriber.subscribe<KeyPressedEvent>([&](const KeyPressedEvent& event) -> bool {
			
			if (event.key == Key::KEY_UP)
				gameboy->memory.joypad().set_value(e_Button::UP, true);

			return false;
		});

		subscriber.subscribe<KeyReleasedEvent>([&](const KeyReleasedEvent& event) -> bool {

			if (event.key == Key::KEY_UP)
				gameboy->memory.joypad().set_value(e_Button::UP, false);

			return false;
		});

		subscriber.subscribe<WindowCloseEvent>([&](const WindowCloseEvent& event) -> bool {
			m_IsOpen = false;
			return false;
		});

		subscriber.subscribe<WindowFramebufferEvent>([](const WindowFramebufferEvent& event) -> bool {
			glViewport(0, 0, event.width, event.height);
			return false;
		});

		// Holder:
		cartridgeHolder->on_event(event);

		subscriber.subscribe<CartridgeFocusEvent>([&](const CartridgeFocusEvent& event) -> bool {
			auto cartridge = cartridgeHolder->get_cartridge(event.handle);
			cartridgeAnalyzer->set_cartridge(cartridge);
			return false;
		});

		// Gameboy:
		subscriber.subscribe<InsertCartridgeEvent>([&](const InsertCartridgeEvent& event) -> bool {
			auto cartridge = cartridgeHolder->get_cartridge(event.handle);
			gameboy->load_cartridge(*cartridge);

			return false;
		});

		// Frames:
		cartridgeAnalyzer->on_event(event);
		cartridgeFrames->on_event(event);
	}

	void s_DatApplication::on_update()
	{
		gameboy->run(1);
	}

	void s_DatApplication::on_render()
	{
		dat::begin_ui();

		cartridgeAnalyzer->render();
		menuBar->render();
		disassembler->render();
		gameboyFrame->render();
		cartridgeFrames->render();

		dat::end_ui();
	}

	void s_DatApplication::initialize_assets()
	{
		// Default Texture:
		{
			defaultTextureHandle = textureLibrary.insert_texture(create_placeholder_texture());
		}

		// Gameboy Texture:
		{
			auto textureAsset = load_texture("res/gameboy.png");
			gameboyTextureHandle = textureLibrary.insert_texture(textureAsset);
		}

		// Cartridge Texture:
		{
			auto textureAsset = load_texture("res/cartridge.png");
			cartridgeTextureHandle = textureLibrary.insert_texture(textureAsset);
		}

		// Invalid Cartridge Texture:
		{
			auto textureAsset = load_texture("res/invalid_cartridge.png");
			invalidCartridgeTextureHandle = textureLibrary.insert_texture(textureAsset);
		}
	}

	void s_DatApplication::initialize_ui()
	{
		menuBar = dat::make_shared<s_Menubar>();
		cartridgeAnalyzer = dat::make_shared<s_CartridgeAnalyzer>();
		disassembler = dat::make_shared<s_Disassembler>();
		gameboyFrame = dat::make_shared<s_GameboyFrame>(*gameboy);
		cartridgeFrames = dat::make_shared<s_CartridgeFrameHolder>();

		// Memory Viewer:
		{
			disassembler->initialize(gameboy.get());
		}

		// Gameboy Frame:
		{
			auto textureData = textureLibrary.get_texture(gameboyTextureHandle).lock();

			TextureDescriptor descriptor;
			{
				descriptor.width = textureData->width;
				descriptor.height = textureData->height;
				descriptor.generateMipmaps = true;
				descriptor.internalFormat = GL_RGBA;
				descriptor.dataFormat = GL_RGBA;
			}
			
			auto gameboyTexture = create_texture(descriptor, textureData->buffer.data);
			gameboyFrame->initialize(gameboyTexture, 128.f * 4, 204.f * 4);
			gameboyFrame->set_scalable(true);
		}

		// Cartridge Frames:
		{
			cartridgeFrames->set_event_callback(BIND(&s_DatApplication::on_event, this));
			cartridgeFrames->initialize(&textureLibrary, cartridgeTextureHandle, invalidCartridgeTextureHandle);
		}

		// Menu Bar:
		{
			menuBar->set_event_callback(BIND(&s_DatApplication::on_event, this));

			menuBar->register_frame("Cartridge Analyzer", cartridgeAnalyzer);
			menuBar->register_frame("Disassembler", disassembler);
			menuBar->register_frame("Gameboy", gameboyFrame);
			menuBar->register_frame("Cartridges", cartridgeFrames);
		}
	}
}