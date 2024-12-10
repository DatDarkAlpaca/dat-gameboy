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
			gameboy = dat::make_unique<s_Gameboy>();
			gameboy->load_bootloader_rom(read_ROM("res/roms/DMG_ROM.gb"));
		}

		// UI:
		dat::setup_ui(window);
		initialize_ui();

		inputHandler.initialize(&gameboy->memory);
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

		inputHandler.on_event(event);

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

		subscriber.subscribe<InsertCartridgeEvent>([&](const InsertCartridgeEvent& event) -> bool {
			auto cartridge = cartridgeHolder->get_cartridge(event.handle);
			gameboy->load_cartridge(cartridge);

			return false;
		});

		// Disassembler:
		subscriber.subscribe<DisassemblerStepEvent>([&](const DisassemblerStepEvent&) -> bool {
			gameboy->cpu.tick();
			gameboy->ppu.tick(4);
			gameboy->timer.tick(4);
			return false;
		});

		// Frames:
		cartridgeAnalyzer->on_event(event);
		cartridgeFrames->on_event(event);
	}

	void s_DatApplication::on_update()
	{
		if (!gameboy->isOn())
			return;

		u32 cpuTicksPerFrame = 17592;
		for (u32 i = 0; i < cpuTicksPerFrame; ++i) 
		{
			gameboy->cpu.tick();
			gameboy->timer.tick(4);
			gameboy->ppu.tick(4);
		}
		
		gameboyFrame->update(gameboy->ppu.get_framebuffer());
	}

	void s_DatApplication::on_render()
	{
		dat::begin_ui();

		cartridgeAnalyzer->render();
		menuBar->render();
		disassembler->render();
		gameboyFrame->render();
		cartridgeFrames->render();
		serialReader->render();

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
			auto textureAsset = load_texture("res/sprites/gameboy.png");
			gameboyTextureHandle = textureLibrary.insert_texture(textureAsset);
		}

		// Cartridge Texture:
		{
			auto textureAsset = load_texture("res/sprites/cartridge.png");
			cartridgeTextureHandle = textureLibrary.insert_texture(textureAsset);
		}

		// Invalid Cartridge Texture:
		{
			auto textureAsset = load_texture("res/sprites/invalid_cartridge.png");
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
		serialReader = dat::make_shared<s_SerialReader>();
		
		// Cartridge Analyzer:
		{
			cartridgeAnalyzer->set_cartridge_holder(cartridgeHolder.get());
		}

		// Memory Viewer:
		{
			disassembler->initialize(gameboy.get());
			disassembler->set_event_callback(BIND(&s_DatApplication::on_event, this));
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
			cartridgeFrames->initialize(cartridgeHolder.get(), &textureLibrary, cartridgeTextureHandle, invalidCartridgeTextureHandle);
		}

		// Serial Reader:
		{
			serialReader->initialize(&gameboy->memory);
		}

		// Menu Bar:
		{
			menuBar->set_event_callback(BIND(&s_DatApplication::on_event, this));

			menuBar->register_frame("Cartridge Analyzer", cartridgeAnalyzer);
			menuBar->register_frame("Disassembler", disassembler);
			menuBar->register_frame("Gameboy", gameboyFrame);
			menuBar->register_frame("Cartridges", cartridgeFrames);
			menuBar->register_frame("Serial Reader", serialReader);
		}
	}
}