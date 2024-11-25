#pragma once
#include <cstdlib>
#include "flags.hpp"
#include "joypad.hpp"

#include "common.hpp"

namespace dat
{
	struct s_Memory
	{
	public:
		s_Memory()
		{
			memory = (u8*)calloc(KB(64), sizeof(u8));
		}

	public:
		u8& operator[] (u16 address) const { return memory[address]; }

	public:
		s_Joypad joypad() const { return s_Joypad { memory[0xFF00] }; }

		void set_joypad_input(bool value) const { }

	public:
		s_LCDC lcdc() const { return s_LCDC{ memory[0xFF40] }; }

	public:
		u8* rom() const { return memory; }

	public:
		u8* memory = nullptr;
	};
}