#pragma once
#include "hardware_register.hpp"

namespace dat
{
	constexpr inline u16 WY_Address = 0xFF4A;
	constexpr inline u16 WX_Address = 0xFF4B;

	using s_WY = s_HardwareRegister<DataRegister>;
	using s_WX = s_HardwareRegister<DataRegister>;
}