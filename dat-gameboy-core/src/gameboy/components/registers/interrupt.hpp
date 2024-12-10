#pragma once
#include "hardware_register.hpp"

namespace dat
{
	constexpr inline u16 IF_Address = 0xFF0F;
	constexpr inline u16 IE_Address = 0xFFFF;

	enum class e_IF : u8
	{
		PAD0	= 0x7,
		PAD1	= 0x6,
		PAD2	= 0x5,
		JOYPAD  = 0x4,
		SERIAL  = 0x3,
		TIMER   = 0x2,
		LCD     = 0x1,
		VBLANK  = 0x0
	};

	using e_IE = e_IF;

	using s_IF = s_HardwareRegister<e_IF>;
	using s_IE = s_HardwareRegister<e_IE>;
}