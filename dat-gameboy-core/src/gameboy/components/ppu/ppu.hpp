#pragma once
#include "gameboy/components/memory.hpp"
#include "video_mode.hpp"
#include "sprite.hpp"

namespace dat
{
	// Color:
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

	// Palette:
	struct Palette
	{
		e_Color color0 = e_Color::WHITE;
		e_Color color1 = e_Color::LIGHT_GRAY;
		e_Color color2 = e_Color::DARK_GRAY;
		e_Color color3 = e_Color::BLACK;
	};
	
	class s_PPU
	{
	public:
		s_PPU();

		~s_PPU();

	public:
		void initialize(NON_OWNING s_MMU* memory);

		void tick();

		void restart();

		void turn_screen_off();

		void turn_screen_on();

	private:
		void read(u16 address);

		void write(u16 address, u8 value);

		void switch_mode(e_VideoMode mode);

	public:
		void handle_lyc();

		void handle_stat();

		void handle_stat_vblank();

	private:
		void render_background_line();

		void render_window_line();

		void render_sprite_line();

	private:
		Palette get_palette_colors(s_PaletteRegister registerValue);

		e_Color get_color_from_palette(u8 color, Palette palette);

	public:
		inline bool vram_accessible() const
		{
			return m_CurrentMode != e_VideoMode::ACCESS_VRAM;
		}

		inline bool oam_accessible() const
		{
			return m_CurrentMode != e_VideoMode::ACCESS_OAM && m_CurrentMode != e_VideoMode::ACCESS_VRAM;
		}

		inline bool palettes_accessible() const
		{
			return m_CurrentMode != e_VideoMode::ACCESS_VRAM;
		}

	public:
		inline void reset_screen_refresh()
		{
			m_RequestScreenRefresh = false;
		}

		inline bool is_requesting_screen_refresh() const { return m_RequestScreenRefresh; }

	public:
		e_VideoMode get_video_mode() const { return m_CurrentMode; }

		e_Color* get_framebuffer() const { return m_Framebuffer; }

	private:
		NON_OWNING s_MMU* r_Memory = nullptr;

	private:
		e_VideoMode m_CurrentMode = e_VideoMode::HBLANK;
		bool m_RequestScreenRefresh = false;
		u64 m_Cycles = 0;
		u64 m_OffscreenCycles = 0;

		bool m_STAT_Interrupt = false;
		u64 m_WindowScanline = 0;

		std::array<s_Sprite, 10> m_Sprites;

	private:
		e_Color* m_Framebuffer = nullptr;
	};
}