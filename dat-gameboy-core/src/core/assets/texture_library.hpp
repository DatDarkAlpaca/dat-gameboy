#pragma once
#include <vector>

#include "common.hpp"
#include <core/buffer.hpp>
#include "asset_handle.hpp"

namespace dat
{
	struct s_TextureAsset
	{
		std::filesystem::path filepath;
		s_Buffer buffer;

		u32 width = 0;
		u32 height = 0;
		u32 channels = 0;
	};

	class s_TextureLibrary
	{
	public:
		asset_handle insert_texture(const s_TextureAsset& asset);

		dat_weak<s_TextureAsset> get_texture(asset_handle handle);

	private:
		std::vector<dat_shared<s_TextureAsset>> m_Textures;
	};

	s_TextureAsset load_texture(const std::filesystem::path& filepath);

	s_TextureAsset create_placeholder_texture();
}