#pragma once
#include "hardware_register.hpp"

namespace dat
{
	constexpr inline u16 SB_Address = 0xFF01;
	constexpr inline u16 SC_Address = 0xFF02;

	enum class e_SC : u8
	{
		TRANSFER_ENABLE = 0x7,
		PAD0			= 0x6,
		PAD1			= 0x5,
		PAD2			= 0x4,
		PAD3			= 0x3,
		PAD4			= 0x2,
		CLOCK_SPEED		= 0x1,
		CLOCK_SELECT	= 0x0
	};

	using s_SB = s_HardwareRegister<DataRegister>;
	using s_SC = s_HardwareRegister<e_SC>;
}