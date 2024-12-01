#pragma once
#include "hardware_register.hpp"

namespace dat
{
	constexpr inline u16 SCY_Address = 0xFF42;
	constexpr inline u16 SCX_Address = 0xFF43;

	using s_SCY = s_HardwareRegister<DataRegister>;
	using s_SCX = s_HardwareRegister<DataRegister>;
}