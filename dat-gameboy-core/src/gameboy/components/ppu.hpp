#pragma once
#include <glad/glad.h>
#include "platform/platform.hpp"

#include "gameboy/constants.hpp"
#include "graphics/quad_renderer.hpp"
#include "gameboy/components/memory.hpp"

#include "utils/byte_utils.hpp"

namespace dat
{
	enum class e_Color
	{
		WHITE,
		LIGHT_GRAY,
		DARK_GRAY,
		BLACK
	};

	inline e_Color get_pixel_from_line(u8 pixels0, u8 pixels1, u8 x)
	{
		return static_cast<e_Color>((bit_value(pixels1, 7 - x) << 1) | bit_value(pixels0, 7 - x));
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
		s_Tile(u8 pixels0, u8 pixels1, u32 tileSize)
			: m_TileSize(tileSize)
		{
			clear();

			for (u16 y = 0; y < TilePixelSize * m_TileSize; ++y)
			{
				u16 index_into_tile = 2 * y;

				for (u8 x = 0; x < 8; ++x)
				{
					u32 pixelIndex = y * TilePixelSize + x;
					m_TileData[pixelIndex] = get_pixel_from_line(pixels0, pixels1, x);
				}
			}
		}

	public:
		e_Color get_color(u8 x, u8 y) const { return m_TileData[y * TilePixelSize + x]; }

	private:
		void clear()
		{
			std::fill(m_TileData.begin(), m_TileData.end(), e_Color::WHITE);
		}

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
			// TODO: check interrupts
			// TODO: cpu. handle interrupts
			m_Cycles += cycles;

			switch (m_VideoMode)
			{
				case e_VideoMode::HBLANK:
				{
					if (m_Cycles < CyclesHBlank)
						return;

					m_Cycles %= CyclesHBlank;

					write_line();
					++r_Memory->LY();

					if (r_Memory->LY() != 144)
					{
						switch_mode(e_VideoMode::ACCESS_OAM);
						return;
					}

					switch_mode(e_VideoMode::VBLANK);
					r_Memory->interrupt_enable().vblank = 1;
					r_Memory->interrupt_enable().commit();
					return;

				} break;

				case e_VideoMode::ACCESS_OAM:
				{
					if (m_Cycles < CyclesOAM)
						return;

					m_Cycles %= CyclesOAM;

					switch_mode(e_VideoMode::ACCESS_VRAM);
				} break;

				case e_VideoMode::ACCESS_VRAM:
				{
					if (m_Cycles < CyclesVRAM)
						return;

					m_Cycles %= CyclesVRAM;

					bool hblankInterrupt = r_Memory->STAT().mode0intSelect;
					if (hblankInterrupt)
					{
						r_Memory->interrupt_enable().lcd = 1;
						r_Memory->interrupt_enable().commit();
					}
					
					if (r_Memory->STAT().LYC_EQ_LY && r_Memory->LYC() == r_Memory->LY())
					{
						r_Memory->interrupt_enable().lcd = 1;
						r_Memory->interrupt_enable().commit();
					}

					r_Memory->STAT().LYC_EQ_LY = 1;
					r_Memory->STAT().commit();

					switch_mode(e_VideoMode::HBLANK);
				} break;

				case e_VideoMode::VBLANK:
				{
					if (m_Cycles < CyclesVBlank)
						return;

					m_Cycles %= VBLANK;

					auto& lineIndex = ++r_Memory->LY();
					
					if (lineIndex != 154)
						return;

					// render.

					render_sprites();

					switch_mode(e_VideoMode::ACCESS_OAM);
					
					/*if (line == 154) 
					{
						write_sprites();
						draw();
						buffer.reset();
						line.reset();
						current_mode = VideoMode::ACCESS_OAM;
						lcd_status.set_bit_to(1, true);
						lcd_status.set_bit_to(0, false);
					};*/
				} break;
			}
		}

	private:
		void write_line()
		{
			if (!r_Memory->lcdc().enableLCD)
				return;

			if (r_Memory->lcdc().enablebackgroundWindow)
				render_background_line();

			if (r_Memory->lcdc().enableWindow)
				render_window_line();
		}
		
		void render_background_line()
		{
			bool useTileSet0 = r_Memory->lcdc().backgroundWindowTileMapArea;
			bool useTileMap0 = !r_Memory->lcdc().backgroundMapArea;

			Palette palette = get_palette_colors(r_Memory->BGP().get_value());

			u8* tileSet = (useTileSet0) ? r_Memory->vram_block0() : r_Memory->vram_block1();
			u8* tileMap = (useTileMap0) ? r_Memory->vram_tilemap0() : r_Memory->vram_tilemap1();

			u32 screenY = r_Memory->LY();

			for (u8 screenX = 0; screenX < GameboyScreenWidth; ++screenX)
			{
				u8 scrolledX = screenX + r_Memory->SCX();
				u8 scrolledY = screenY + r_Memory->SCY();

				u8 backgroundMapX = scrolledX % BackgroundMapTileSize;
				u8 backgroundMapY = scrolledY % BackgroundMapTileSize;

				u8 tileX = backgroundMapX / TilePixelSize;
				u8 tileY = backgroundMapY / TilePixelSize;

				u8 tilePixelX = backgroundMapX % TilePixelSize;
				u8 tilePixelY = backgroundMapY % TilePixelSize;

				u8 tileID = *(tileMap + (tileY * TilesPerLine + tileX));

				// Addressing:
				u8 tileMemoryOffset = useTileSet0
					? tileID * BytesPerTile
					: (static_cast<i8>(tileID) + 128) * BytesPerTile;

				u8 pixelLine0 = *(tileSet + tileMemoryOffset + 2 * tilePixelY);
				u8 pixelLine1 = *(tileSet + tileMemoryOffset + 2 * tilePixelY + 1);

				e_Color pixelColor = get_pixel_from_line(pixelLine0, pixelLine1, tilePixelX);
				e_Color screenColor = get_color_from_palette(pixelColor, palette);

				m_Framebuffer[screenY + GameboyScreenWidth + screenX] = screenColor;
			}
		}

		void render_window_line()
		{
			bool useTileSet0 = r_Memory->lcdc().backgroundWindowTileMapArea;
			bool useTileMap0 = !r_Memory->lcdc().windowTilemapArea;

			Palette palette = get_palette_colors(r_Memory->BGP().get_value());

			u8* tileSet = (useTileSet0) ? r_Memory->vram_block0() : r_Memory->vram_block1();
			u8* tileMap = (useTileMap0) ? r_Memory->vram_tilemap0() : r_Memory->vram_tilemap1();

			u32 screenY = r_Memory->LY();
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

				u8 tileID = *(tileMap + (tileY * TilesPerLine + tileX));

				// Addressing:
				u8 tileMemoryOffset = useTileSet0
					? tileID * BytesPerTile
					: (static_cast<i8>(tileID) + 128) * BytesPerTile;

				u8 pixelLine0 = *(tileSet + tileMemoryOffset + 2 * tilePixelY);
				u8 pixelLine1 = *(tileSet + tileMemoryOffset + 2 * tilePixelY + 1);

				e_Color pixelColor = get_pixel_from_line(pixelLine0, pixelLine1, tilePixelX);
				e_Color screenColor = get_color_from_palette(pixelColor, palette);

				m_Framebuffer[screenY + GameboyScreenWidth + screenX] = screenColor;
			}
		}

		void render_sprites()
		{
			if (!r_Memory->lcdc().enableObj)
				return;
			
			for (u32 index = 0; index < SpriteAttrAmountOAM; ++index)
				render_sprite(index);
		}

		void render_sprite(u32 index)
		{
			const s_LCDC& lcdc = r_Memory->lcdc();

			// Sprite Attributes:
			u8* spriteAddress = r_Memory->oam() + index * BytesPerSprite;
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
			u32 spriteSize = lcdc.objSize ? 2 : 1;

			// Palette:
			Palette palette = (attributes.dmgPalette) ?
				get_palette_colors(r_Memory->OBP1()) :
				get_palette_colors(r_Memory->OBP0());

			// Tile:
			u8 pixel0 = *r_Memory->vram(tileIndex * BytesPerTile + 0);
			u8 pixel1 = *r_Memory->vram(tileIndex * BytesPerTile + 1);
			s_Tile spriteTile(pixel0, pixel1, spriteSize);

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

					if (x < GameboyScreenWidth && y < GameboyScreenHeight)
						continue;

					auto currentPixel = m_Framebuffer[screenY * TilePixelSize + screenX];
					if (priority && currentPixel != e_Color::WHITE)
						continue;

					e_Color color = get_color_from_palette(tileColor, palette);
					set_pixel(screenX, screenY, color);
				}
			}
		}

	private:
		void switch_mode(e_VideoMode mode)
		{
			m_VideoMode = mode;
			r_Memory->STAT().set_ppu_mode(m_VideoMode);
		}

		Palette get_palette_colors(u8 registerValue)
		{
			u8 color0 = (check_bit(registerValue, 1) << 1) || check_bit(registerValue, 0);
			u8 color1 = (check_bit(registerValue, 1) << 3) || check_bit(registerValue, 2);
			u8 color2 = (check_bit(registerValue, 1) << 5) || check_bit(registerValue, 4);
			u8 color3 = (check_bit(registerValue, 1) << 7) || check_bit(registerValue, 6);

			auto get_palette_color = [](u8 colorCode) {
				return static_cast<e_Color>(colorCode);
			};

			return {
				get_palette_color(color0),
				get_palette_color(color1),
				get_palette_color(color2),
				get_palette_color(color3),
			};
		}

		e_Color get_color_from_palette(e_Color color, Palette palette)
		{
			if (color == e_Color::WHITE)
				return palette.color0;

			if (color == e_Color::LIGHT_GRAY)
				return palette.color1;

			if (color == e_Color::DARK_GRAY)
				return palette.color2;

			if (color == e_Color::BLACK)
				return palette.color3;

			DAT_LOG_CRITICAL("Invalid color: {}", static_cast<u8>(color));
			return e_Color::WHITE;
		}

		void set_pixel(u32 x, u32 y, e_Color color)
		{
			u32 index = y * GameboyScreenWidth + x;
			m_Framebuffer[index] = color;
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