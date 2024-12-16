#pragma once
#include "common.hpp"

namespace dat
{
	enum class e_VideoMode : u8
	{
		HBLANK = 0x0,
		VBLANK = 0x1,
		ACCESS_OAM = 0x2,
		ACCESS_VRAM = 0x3,
	};
}