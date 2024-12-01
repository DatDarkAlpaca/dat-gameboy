#pragma once
#include <core/assets/texture_library.hpp>
#include "frames/cartridge_frame.hpp"

namespace dat
{
	class s_CartridgeHolder;

	class s_CartridgeFrameHolder : public IFrame
	{
	public:
		void initialize(s_CartridgeHolder* holder, s_TextureLibrary* textureLibrary, asset_handle textureHandle, asset_handle invalidCartridgeTextureHandle);

	public:
		void on_event(Event& event);

		void render();

	private:
		void initialize_textures();

		void create_cartridge_frame(const dat_shared<ICartridge>& cartridge, cartridge_handle handle);
		
		void remove_cartridge_frame(cartridge_handle handle);

	private:
		void remove_scheduled_frame();
	
	private:
		std::unordered_map<cartridge_handle, dat_unique<s_CartridgeFrame>> m_Frames;
		cartridge_handle m_ScheduledToDestroy = invalid_asset;

		s_TextureLibrary* m_TextureLibrary = nullptr;
		asset_handle m_TextureHandle = invalid_asset;
		asset_handle m_InvalidCartridgeTextureHandle = invalid_asset;

		gl_handle m_Texture = invalid_handle;
		gl_handle m_InvalidTexture = invalid_handle;

	private:
		NON_OWNING s_CartridgeHolder* r_Holder = nullptr;
	};
}