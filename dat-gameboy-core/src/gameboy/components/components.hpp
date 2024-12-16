#pragma once
#include "cpu/registers.hpp"
#include "cpu/sharp_sm83.hpp"
#include "cpu/flags.hpp"
#include "ppu/ppu.hpp"
#include "memory.hpp"
#include "timer.hpp"
#include "cartridge/cartridge.hpp"
#include "cartridge/cartridge_header.hpp"

namespace dat
{
	enum class e_Component
	{
		NONE	= 0,
		CPU		= 1,
		PPU		= 2,
		APU		= 4,
		MEMORY	= 8,
		TIMER	= 16,
		DMA		= 32,
		SERIAL	= 64,

		ALL_NO_CPU	= PPU | APU | MEMORY | TIMER | DMA | SERIAL,
		ALL			= CPU | PPU | APU | MEMORY | TIMER | DMA | SERIAL,
	};

	inline u64 operator&(e_Component lhs, e_Component rhs) 
	{
		return static_cast<u64>(lhs) & static_cast<u64>(rhs);
	}
}