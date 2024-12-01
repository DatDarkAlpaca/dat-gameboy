#pragma once
#include "hardware_register.hpp"

namespace dat
{
	constexpr inline u16 LY_Address = 0xFF44;
	constexpr inline u16 LYC_Address = 0xFF45;

	using s_LY = s_HardwareRegister<DataRegister>;
	using s_LYC = s_HardwareRegister<DataRegister>;
}