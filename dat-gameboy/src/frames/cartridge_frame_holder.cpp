#include "pch.hpp"
#include "cartridge_frame_holder.hpp"
#include "core/cartridge_holder.hpp"

namespace dat
{
	void s_CartridgeFrameHolder::initialize(s_CartridgeHolder* holder, s_TextureLibrary* textureLibrary, asset_handle textureHandle, asset_handle invalidCartridgeTextureHandle)
	{
		r_Holder = holder;
		m_TextureLibrary = textureLibrary;
		m_TextureHandle = textureHandle;
		m_InvalidCartridgeTextureHandle = invalidCartridgeTextureHandle;

		initialize_textures();
	}

	void s_CartridgeFrameHolder::on_event(Event& event)
	{
		Subscriber subscriber(event);

		subscriber.subscribe<PropagateLoadROMFileEvent>([&](const PropagateLoadROMFileEvent& event) -> bool {
			auto cartridge = r_Holder->get_cartridge(event.handle);
			create_cartridge_frame(cartridge, event.handle);
			return false;
		});

		subscriber.subscribe<CartridgeRemoveEvent>([&](const CartridgeRemoveEvent& event) -> bool {
			m_ScheduledToDestroy = event.handle;
			return false;
		});
	}

	void s_CartridgeFrameHolder::render()
	{
		if (!isOpen)
			return;

		for (const auto& [_, cartridgeFrame] : m_Frames)
		{
			if (cartridgeFrame)
				cartridgeFrame->render();
		}

		remove_scheduled_frame();
	}

	void s_CartridgeFrameHolder::initialize_textures()
	{
		auto textureData = m_TextureLibrary->get_texture(m_TextureHandle).lock();

		TextureDescriptor descriptor;
		{
			descriptor.width = textureData->width;
			descriptor.height = textureData->height;
			descriptor.generateMipmaps = true;
			descriptor.internalFormat = GL_RGBA;
			descriptor.dataFormat = GL_RGBA;
		}

		m_Texture = create_texture(descriptor, textureData->buffer.data);

		// Invalid Texture:
		textureData = m_TextureLibrary->get_texture(m_InvalidCartridgeTextureHandle).lock();
		{
			descriptor.width = textureData->width;
			descriptor.height = textureData->height;
			descriptor.generateMipmaps = true;
			descriptor.internalFormat = GL_RGBA;
			descriptor.dataFormat = GL_RGBA;
		}

		m_InvalidTexture = create_texture(descriptor, textureData->buffer.data);
	}

	void s_CartridgeFrameHolder::create_cartridge_frame(const dat_shared<ICartridge>& cartridge, cartridge_handle handle)
	{
		std::string cartridgeName = cartridge->filepath.filename().string();
		m_Frames[handle] = dat::make_unique<s_CartridgeFrame>(cartridgeName, handle);

		auto& frame = m_Frames[handle];
		frame->initialize(m_Texture, 78.f * 3, 96.f * 3);
		frame->set_event_callback(callback);

		// Invalid Cartridge Texture:
		if (!is_cartridge_logo_valid(cartridge->header))
			frame->set_texture(m_InvalidTexture);
	}

	void s_CartridgeFrameHolder::remove_cartridge_frame(cartridge_handle handle)
	{
		m_Frames.extract(handle);
	}

	void s_CartridgeFrameHolder::remove_scheduled_frame()
	{
		if (m_ScheduledToDestroy != invalid_handle)
		{
			remove_cartridge_frame(m_ScheduledToDestroy);
			m_ScheduledToDestroy = invalid_handle;
		}
	}
}
