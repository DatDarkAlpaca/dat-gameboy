#pragma once
#include <glad/glad.h>
#include "platform/platform.hpp"

#include "gameboy/constants.hpp"
#include "graphics/quad_renderer.hpp"
#include "gameboy/components/memory.hpp"

#include "utils/byte_utils.hpp"

namespace dat
{
	enum class e_Color : u8
	{
		WHITE			= 0x0,
		LIGHT_GRAY		= 0x1,
		DARK_GRAY		= 0x2,
		BLACK			= 0x3
	};

	inline u8 get_pixel_from_line(u8 pixels0, u8 pixels1, u8 x)
	{
		return (bit_value(pixels1, 7 - x) << 1) | bit_value(pixels0, 7 - x);
	}

	struct Palette
	{
		e_Color color0 = e_Color::WHITE;
		e_Color color1 = e_Color::LIGHT_GRAY;
		e_Color color2 = e_Color::DARK_GRAY;
		e_Color color3 = e_Color::BLACK;
	};

	struct s_Tile
	{
	public:
		s_Tile(u8* tileAddress, u32 tileSize)
			: m_TileSize(tileSize)
		{
			for (u16 y = 0; y < TilePixelSize * m_TileSize; ++y)
			{
				for (u8 x = 0; x < 8; ++x)
				{
					auto pixelLine0 = *(tileAddress + 2 * y);
					auto pixelLine1 = *(tileAddress + 2 * y + 1);

					u32 pixelIndex = y * TilePixelSize + x;
					m_TileData[pixelIndex] = (e_Color)get_pixel_from_line(pixelLine0, pixelLine1, x);
				}
			}
		}

	public:
		e_Color get_color(u8 x, u8 y) const { return m_TileData[y * TilePixelSize + x]; }

	private:
		std::array<e_Color, 2 * TilePixelSize * TilePixelSize> m_TileData;
		u32 m_TileSize;
	};
	
	class s_PPU
	{
	public:
		void initialize(s_MMU* memory)
		{
			r_Memory = memory;
		}

		void tick(u32 cycles)
		{
			m_Cycles += cycles;
			m_VideoMode = r_Memory->STAT().get_video_mode();

			switch (m_VideoMode)
			{
				case e_VideoMode::HBLANK:
				{
					if (m_Cycles < CyclesHBlank)
						return;

					m_Cycles = 0;

					write_line();
					++r_Memory->LY();

					if (r_Memory->LY() == 144)
					{
						r_Memory->IF().set(e_IF::VBLANK, true);
						switch_mode(e_VideoMode::VBLANK);
						return;
					}

					switch_mode(e_VideoMode::ACCESS_OAM);
					return;
				} break;

				case e_VideoMode::ACCESS_OAM:
				{
					if (m_Cycles < CyclesOAM)
						return;

					m_Cycles = 0;

					switch_mode(e_VideoMode::ACCESS_VRAM);
				} break;

				case e_VideoMode::ACCESS_VRAM:
				{
					if (m_Cycles < CyclesVRAM)
						return;

					m_Cycles = 0;

					bool hblankInterrupt = r_Memory->STAT().is_bit_set(e_STAT::MODE_0_INT_SELECT);
					if (hblankInterrupt)
						r_Memory->IF().set(e_IF::LCD, true);
					
					bool lycEquivalent = r_Memory->LYC() == r_Memory->LY();
					if (r_Memory->STAT().is_bit_set(e_STAT::LYC_EQ_LC) && lycEquivalent)
						r_Memory->IF().set(e_IF::LCD, true);

					r_Memory->STAT().set(e_STAT::LYC_EQ_LC, lycEquivalent);
					switch_mode(e_VideoMode::HBLANK);
				} break;

				case e_VideoMode::VBLANK:
				{
					if (m_Cycles < CyclesVBlank)
						return;

					m_Cycles = 0;
					++r_Memory->LY();

					if (r_Memory->LY() != 154)
						return;

					render_sprites();
									
					r_Memory->LY().write(0);

					switch_mode(e_VideoMode::ACCESS_OAM);
				} break;
			}
		}

	public:
		void restart()
		{
			std::fill(m_Framebuffer.begin(), m_Framebuffer.end(), e_Color::BLACK);
			m_VideoMode = e_VideoMode::ACCESS_OAM;
			m_Cycles = 0;
		}

	private:
		void write_line()
		{
			if (!r_Memory->lcdc().is_bit_set(e_LCDC::LCD_PPU_ENABLE))
				return;

			if (r_Memory->lcdc().is_bit_set(e_LCDC::BG_WINDOW_ENABLE_PRIORITY))
				render_background_line();

			if (r_Memory->lcdc().is_bit_set(e_LCDC::WINDOW_ENABLE))
				render_window_line();
		}
		
		void render_background_line()
		{
			bool useTileSet0 = r_Memory->lcdc().is_bit_set(e_LCDC::BG_WINDOW_TILES);
			bool useTileMap0 = !r_Memory->lcdc().is_bit_set(e_LCDC::BG_TILEMAP);

			Palette palette = get_palette_colors(r_Memory->BGP());

			u16 tileSetAddress = (useTileSet0) ? VRAM_Block0_Address : VRAM_Block1_Address;
			u16 tileMapAddress = (useTileMap0) ? VRAM_Tilemap_0_Address : VRAM_Tilemap_1_Address;

			u32 screenY = r_Memory->LY().get();

			if (screenY >= GameboyScreenHeight)
				return;

			for (u8 screenX = 0; screenX < GameboyScreenWidth; ++screenX)
			{
				u8 scrolledX = screenX + r_Memory->SCX().get();
				u8 scrolledY = screenY + r_Memory->SCY().get();

				u8 backgroundMapX = scrolledX % BackgroundMapTileSize;
				u8 backgroundMapY = scrolledY % BackgroundMapTileSize;

				u8 tileX = backgroundMapX / TilePixelSize;
				u8 tileY = backgroundMapY / TilePixelSize;

				u8 tilePixelX = backgroundMapX % TilePixelSize;
				u8 tilePixelY = backgroundMapY % TilePixelSize;

				u16 tileIDIndex = tileMapAddress + tileY * TilesPerLine + tileX;
				u8 tileID = r_Memory->read(tileIDIndex);

				// Addressing mode:
				u8 tileMemoryOffset = useTileSet0
					? tileID * BytesPerTile
					: (static_cast<i8>(tileID) + 128) * BytesPerTile;

				u16 pixelLineAddress = tileSetAddress + tileMemoryOffset + tilePixelY * 2;
				u8 pixelLine0 = r_Memory->read(pixelLineAddress);
				u8 pixelLine1 = r_Memory->read(pixelLineAddress + 1);

				u8 pixelColor = get_pixel_from_line(pixelLine0, pixelLine1, tilePixelX);
				e_Color screenColor = get_color_from_palette(pixelColor, palette);

				set_pixel(screenX, screenY, screenColor);
			}
		}

		void render_window_line()
		{
			bool useTileSet0 = r_Memory->lcdc().is_bit_set(e_LCDC::BG_WINDOW_TILES);
			bool useTileMap0 = !r_Memory->lcdc().is_bit_set(e_LCDC::WINDOW_TILEMAP);

			Palette palette = get_palette_colors(r_Memory->BGP());

			u16 tileSetAddress = (useTileSet0) ? VRAM_Block0_Address : VRAM_Block1_Address;
			u16 tileMapAddress = (useTileMap0) ? VRAM_Tilemap_0_Address : VRAM_Tilemap_1_Address;

			u32 screenY = r_Memory->LY().get();
			u32 scrolledY = screenY - r_Memory->WY();

			if (scrolledY >= GameboyScreenHeight)
				return;

			for (u8 screenX = 0; screenX < GameboyScreenWidth; ++screenX)
			{
				u8 scrolledX = screenX + r_Memory->WX() - 7;

				u8 tileX = scrolledX / TilePixelSize;
				u8 tileY = scrolledY / TilePixelSize;

				u8 tilePixelX = scrolledX % TilePixelSize;
				u8 tilePixelY = scrolledY % TilePixelSize;

				u16 tileIDIndex = tileMapAddress + tileY * TilesPerLine + tileX;
				u8 tileID = r_Memory->read(tileIDIndex);

				// Addressing:
				u8 tileMemoryOffset = useTileSet0
					? tileID * BytesPerTile
					: (static_cast<i8>(tileID) + 128) * BytesPerTile;

				u16 pixelLineAddress = tileSetAddress + tileMemoryOffset + tilePixelY * 2;
				u8 pixelLine0 = r_Memory->read(pixelLineAddress);
				u8 pixelLine1 = r_Memory->read(pixelLineAddress + 1);

				u8 pixelColor = get_pixel_from_line(pixelLine0, pixelLine1, tilePixelX);
				e_Color screenColor = get_color_from_palette(pixelColor, palette);

				set_pixel(screenX, screenY, screenColor);
			}
		}

		void render_sprites()
		{
			if (!r_Memory->lcdc().is_bit_set(e_LCDC::OBJ_ENABLE))
				return;
			
			for (u32 index = 0; index < SpriteAttrAmountOAM; ++index)
				render_sprite(index);
		}

		void render_sprite(u32 index)
		{
			const s_LCDC& lcdc = r_Memory->lcdc();

			// Sprite Attributes:
			u8* spriteAddress = r_Memory->oam(index * BytesPerSprite);
			u8 spriteYPos					  = *(spriteAddress + 0);
			u8 spriteXPos					  = *(spriteAddress + 1);
			u8 tileIndex					  = *(spriteAddress + 2);
			s_SpriteAttributeFlags attributes { *(spriteAddress + 3) };

			if (spriteYPos == 0 || spriteYPos >= GameboyScreenHeight + 16 || 
				spriteXPos == 0 || spriteXPos >= GameboyScreenWidth + 8)
				return;

			// Attributes:
			bool flipX     = attributes.xFlip;
			bool flipY     = attributes.yFlip;
			bool priority  = attributes.priority;
			u32 spriteSize = lcdc.is_bit_set(e_LCDC::OBJ_SIZE) ? 2 : 1;

			// Palette:
			Palette palette = (attributes.dmgPalette) ?
				get_palette_colors(r_Memory->OBP1()) :
				get_palette_colors(r_Memory->OBP0());

			// Tile:
			u8* tileAddress = r_Memory->vram(tileIndex * BytesPerTile);
			s_Tile spriteTile(tileAddress, spriteSize);

			u8 startY = spriteYPos - 16;
			u8 startX = spriteXPos - 8;

			for (u32 y = 0; y < TilePixelSize * spriteSize; ++y)
			{
				for (u32 x = 0; x < TilePixelSize; ++x)
				{
					u32 yPosition = !flipY ? y : (TilePixelSize * spriteSize) - y - 1;
					u32 xPosition = !flipX ? x : TilePixelSize - x - 1;

					e_Color tileColor = spriteTile.get_color(xPosition, yPosition);

					// Transparent:
					if (tileColor == e_Color::WHITE)
						continue;

					int screenX = startX + x;
					int screenY = startY + y;

					if (x > GameboyScreenWidth || y > GameboyScreenHeight)
						continue;

					auto currentPixel = m_Framebuffer[screenY * TilePixelSize + screenX];
					if (priority && currentPixel != e_Color::WHITE)
						continue;

					e_Color color = get_color_from_palette((u8)tileColor, palette);
					set_pixel(screenX, screenY, color);
				}
			}
		}

	private:
		void switch_mode(e_VideoMode mode)
		{
			m_VideoMode = mode;
			r_Memory->STAT().set_video_mode(m_VideoMode);
		}

		Palette get_palette_colors(s_PaletteRegister registerValue)
		{
			return {
				static_cast<e_Color>(registerValue.get_color(e_BGPColor::COLOR0)),
				static_cast<e_Color>(registerValue.get_color(e_BGPColor::COLOR1)),
				static_cast<e_Color>(registerValue.get_color(e_BGPColor::COLOR2)),
				static_cast<e_Color>(registerValue.get_color(e_BGPColor::COLOR3)),
			};
		}

		e_Color get_color_from_palette(u8 color, Palette palette)
		{
			const auto& paletteColor = static_cast<e_Color>(color);

			if (paletteColor == e_Color::WHITE)
				return palette.color0;

			if (paletteColor == e_Color::LIGHT_GRAY)
				return palette.color1;

			if (paletteColor == e_Color::DARK_GRAY)
				return palette.color2;

			if (paletteColor == e_Color::BLACK)
				return palette.color3;

			DAT_LOG_CRITICAL("Invalid color: {}", static_cast<u8>(color));
			return e_Color::BLACK;
		}

		void set_pixel(u32 x, u32 y, e_Color color)
		{
			m_Framebuffer[y * GameboyScreenWidth + x] = color;
		}

	public:
		std::array<e_Color, GameboyScreenWidth* GameboyScreenHeight> get_framebuffer() const { return m_Framebuffer; }

	private:
		std::array<e_Color, GameboyScreenWidth * GameboyScreenHeight> m_Framebuffer;
		NON_OWNING s_MMU* r_Memory = nullptr;

	private:
		e_VideoMode m_VideoMode = e_VideoMode::ACCESS_OAM;
		u64 m_Cycles = 0;
	};
}