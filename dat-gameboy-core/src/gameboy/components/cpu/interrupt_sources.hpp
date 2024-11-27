#pragma once
#include "common.hpp"

namespace dat::interrupt
{
	constexpr inline u16 VBlank	    = 0x40;
	constexpr inline u16 LCDCStatus = 0x48;
	constexpr inline u16 Timer      = 0x50;
	constexpr inline u16 Serial     = 0x58;
	constexpr inline u16 Joypad     = 0x60;
}