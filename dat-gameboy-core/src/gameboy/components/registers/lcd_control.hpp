#pragma once
#include "hardware_register.hpp"

namespace dat
{
	constexpr inline u16 LCDC_Address = 0xFF40;

	enum class e_LCDC : u8
	{
		LCD_PPU_ENABLE				= 0x7,
		WINDOW_TILEMAP				= 0x6,
		WINDOW_ENABLE				= 0x5,
		BG_WINDOW_TILES				= 0x4,
		BG_TILEMAP					= 0x3,
		OBJ_SIZE					= 0x2,
		OBJ_ENABLE					= 0x1,
		BG_WINDOW_ENABLE_PRIORITY	= 0x0
	};

	using s_LCDC = s_HardwareRegister<e_LCDC>;
}