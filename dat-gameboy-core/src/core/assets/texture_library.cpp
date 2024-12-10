#include "pch.hpp"
#include "texture_library.hpp"

#include <stb_image.h>

namespace dat
{
	asset_handle s_TextureLibrary::insert_texture(const s_TextureAsset& asset)
	{
		m_Textures.push_back(dat::make_shared<s_TextureAsset>(asset));
		return static_cast<asset_handle>(m_Textures.size() - 1);
	}

	dat_weak<s_TextureAsset> s_TextureLibrary::get_texture(asset_handle handle)
	{
		return m_Textures[static_cast<size_t>(handle)];
	}

	s_TextureAsset load_texture(const std::filesystem::path& filepath)
	{
		if (!std::filesystem::exists(filepath))
		{
			DAT_LOG_ERROR("Failed to load texture: '{}'. File does not exist.", filepath.string());
			return {};
		}

		int width, height, channels;

		s_Buffer buffer;
		{
			buffer.data = stbi_load(filepath.string().c_str(), &width, &height, &channels, 4);
			buffer.size = width * height * channels;
			buffer.deleter = stbi_image_free;
		}

		s_TextureAsset asset;
		asset.filepath = filepath;

		asset.width = static_cast<u32>(width);
		asset.height = static_cast<u32>(height);
		asset.channels = static_cast<u32>(channels);
		asset.buffer = std::move(buffer);

		return asset;
	}

	s_TextureAsset create_placeholder_texture()
	{
		s_TextureAsset asset;
		asset.filepath = "$DEFAULT_TEXTURE";
		asset.width = 2;
		asset.height = 2;
		asset.channels = 4;

		s_Buffer buffer;
		{
			buffer.size = asset.width * asset.height * asset.channels * sizeof(u8);
			buffer.data = (u8*)malloc(buffer.size);
			
			u8 data[4 * 4] =
			{
				210, 210, 210, 255,
				240, 240, 240, 255,
				240, 240, 240, 255,
				210, 210, 210, 255
			};

			if(buffer.data)
				memcpy(buffer.data, data, buffer.size);
		}

		asset.buffer = std::move(buffer);
		return asset;
	}
}