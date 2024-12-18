#pragma once
#include "gameboy/constants.hpp"
#include "pch.hpp"
#include "ppu.hpp"

static u64 get_hblank_ticks(u8 scrollX)
{
	if ((scrollX & 7) == 0)
		return dat::CyclesHBlank_First;

	else if ((scrollX & 7) < 4)
		return dat::CyclesHBlank_Second;

	else if ((scrollX & 7) < 7)
		return dat::CyclesHBlank_Third;

	return 0;
}

namespace dat
{
	s_PPU::s_PPU()
	{
		m_Framebuffer = (e_Color*)malloc(sizeof(e_Color) * GameboyScreenWidth * GameboyScreenHeight);
	}

	s_PPU::~s_PPU()
	{
		free(m_Framebuffer);
	}

	void s_PPU::initialize(NON_OWNING s_MMU* memory)
	{
		r_Memory = memory;
	}

	void s_PPU::tick()
	{
		if (!r_Memory->lcdc().is_bit_set(e_LCDC::LCD_PPU_ENABLE))
		{
			++m_OffscreenCycles;

			if (m_OffscreenCycles >= CyclesPerFrame)
			{
				m_OffscreenCycles = 0;
				m_RequestScreenRefresh = true;
			}

			return;
		}

		s_SCX& SCX = r_Memory->SCX();
		s_LY& LY = r_Memory->LY();

		++m_Cycles;

		switch (m_CurrentMode)
		{
			case e_VideoMode::HBLANK:
			{
				if (m_Cycles < get_hblank_ticks(SCX.get()))
					return;

				m_Cycles = 0;
				++LY;

				handle_lyc();

				if (LY == 144)
				{
					switch_mode(e_VideoMode::VBLANK);

					r_Memory->IF().set(e_IF::VBLANK, true);
					handle_stat_vblank();

					m_RequestScreenRefresh = true;
				}
				else
				{
					switch_mode(e_VideoMode::ACCESS_OAM);
					handle_stat();
				}

			} break;

			case e_VideoMode::VBLANK:
			{
				if (m_Cycles < CyclesVBlank)
					return;

				m_Cycles = 0;
				++LY;

				handle_lyc();
				handle_stat();

				if (LY != 154)
					return;

				switch_mode(e_VideoMode::ACCESS_OAM);
				LY.write(0);
				m_WindowScanline = 0;

				handle_lyc();
				handle_stat();
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
				switch_mode(e_VideoMode::HBLANK);

				handle_stat();

				render_background_line();
				// render_window_line();
				// render_sprites_line();

			} break;
		}
	}

	void s_PPU::restart()
	{
		m_CurrentMode = e_VideoMode::HBLANK;
		m_RequestScreenRefresh = false;
		m_Cycles = 0;
		m_OffscreenCycles = 0;

		m_STAT_Interrupt = false;
		m_WindowScanline = 0;

		u32 framebufferSize = GameboyScreenHeight * GameboyScreenWidth;
		std::fill(m_Framebuffer, m_Framebuffer + framebufferSize, e_Color::BLACK);
	}

	void s_PPU::turn_screen_off()
	{
		switch_mode(e_VideoMode::HBLANK);
		r_Memory->LY().write(0);
		m_OffscreenCycles = 0;
		
		u32 framebufferSize = GameboyScreenHeight * GameboyScreenWidth;
		std::fill(m_Framebuffer, m_Framebuffer + framebufferSize, e_Color::BLACK);
	}

	void s_PPU::turn_screen_on()
	{
		handle_lyc();
		handle_stat();
		m_Cycles = 0;
	}

	void s_PPU::read(u16 address)
	{
		// TODO: VRAM BANK (VBK)  - chose bank
		r_Memory->read(address);
	}

	void s_PPU::write(u16 address, u8 value)
	{
	}

	void s_PPU::switch_mode(e_VideoMode mode)
	{
		m_CurrentMode = mode;
		r_Memory->STAT().set_video_mode(m_CurrentMode);
	}

	void s_PPU::handle_lyc()
	{
		u8 LY  = r_Memory->LY().get();
		u8 LYC = r_Memory->LYC().get();
		r_Memory->STAT().set(e_STAT::LYC_EQ_LC, LYC == LY);
	}

	void s_PPU::handle_stat()
	{
		s_STAT& stat = r_Memory->STAT();

		if (m_CurrentMode == e_VideoMode::HBLANK     && stat.is_bit_set(e_STAT::MODE_0_INT_SELECT) ||
			m_CurrentMode == e_VideoMode::VBLANK     && stat.is_bit_set(e_STAT::MODE_1_INT_SELECT) ||
			m_CurrentMode == e_VideoMode::ACCESS_OAM && stat.is_bit_set(e_STAT::MODE_2_INT_SELECT) ||
			stat.is_bit_set(e_STAT::LYC_INT_SELECT)  && stat.is_bit_set(e_STAT::LYC_EQ_LC)			)
		{
			if (m_STAT_Interrupt)
				return;

			r_Memory->IF().set(e_IF::LCD, true);
			m_STAT_Interrupt = true;
		}

		else
			m_STAT_Interrupt = false;
	}

	void s_PPU::handle_stat_vblank()
	{
		s_STAT& stat = r_Memory->STAT();
		if (stat.is_bit_set(e_STAT::MODE_2_INT_SELECT))
		{
			if (m_STAT_Interrupt)
				return;

			r_Memory->IF().set(e_IF::LCD, true);
			m_STAT_Interrupt = true;
		}
		else
			m_STAT_Interrupt = false;
	}

	void s_PPU::render_background_line()
	{
		const s_LCDC& lcdc = r_Memory->lcdc();
		
		if (!lcdc.is_bit_set(e_LCDC::BG_WINDOW_ENABLE_PRIORITY))
			return;

		Palette palette = get_palette_colors(r_Memory->BGP());

		u8 LY  = r_Memory->LY().get();
		u8 SCY = r_Memory->SCY().get();
		u8 SCX = r_Memory->SCX().get();

		bool useTileSetZero = lcdc.is_bit_set(e_LCDC::BG_WINDOW_TILES);
		bool useTileMapZero = !lcdc.is_bit_set(e_LCDC::BG_TILEMAP);
		
		u16 tileSetAddress = useTileSetZero ? VRAM_Block0_Address : VRAM_Block1_Address;
		u16 tileMapAddress = useTileMapZero ? VRAM_Tilemap_0_Address : VRAM_Tilemap_1_Address;

		u16 backgroundY = LY + SCY;
		u16 tileY = backgroundY / TilePixelSize;

		for (u16 screenX = 0; screenX < GameboyScreenWidth; ++screenX)
		{
			u16 backgroundX = screenX + SCX;

			u16 backgroundMapIndexX = backgroundX % BackgroundMapTileSize;
			u16 backgroundMapIndexY = backgroundY % BackgroundMapTileSize;

			u16 tileX = backgroundMapIndexX / TilePixelSize;
			u16 tileY = backgroundMapIndexY / TilePixelSize;

			u16 tilePixelX = backgroundMapIndexX % TilePixelSize;
			u16 tilePixelY = backgroundMapIndexY % TilePixelSize;

			u16 tileIndex = tileY * TilesPerLine + tileX;
			u16 tileIDAddress = tileMapAddress + tileIndex;

			u8 tileID = r_Memory->read(tileIDAddress);

			// Addressing:
			u16 tileDataOffset = useTileSetZero
				? tileID * BytesPerTile
				: (static_cast<i8>(tileID) + 128) * BytesPerTile;

			u16 tileDataLineOffset = tilePixelY * 2;
			u16 tileDataAddress = tileSetAddress + tileDataOffset + tileDataLineOffset;

			u8 pixelLine0 = r_Memory->read(tileDataAddress);
			u8 pixelLine1 = r_Memory->read(tileDataAddress + 1);

			u8 color = get_pixel_from_line(pixelLine0, pixelLine1, tilePixelX);
			e_Color screenColor = get_color_from_palette(color, palette);

			m_Framebuffer[LY * GameboyScreenWidth + screenX] = screenColor;
		}
	}

	void s_PPU::render_window_line()
	{
	}

	void s_PPU::render_sprite_line()
	{
	}

	Palette s_PPU::get_palette_colors(s_PaletteRegister registerValue)
	{
		return {
			static_cast<e_Color>(registerValue.get_color(e_BGPColor::COLOR0)),
			static_cast<e_Color>(registerValue.get_color(e_BGPColor::COLOR1)),
			static_cast<e_Color>(registerValue.get_color(e_BGPColor::COLOR2)),
			static_cast<e_Color>(registerValue.get_color(e_BGPColor::COLOR3)),
		};
	}

	e_Color s_PPU::get_color_from_palette(u8 color, Palette palette)
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
}