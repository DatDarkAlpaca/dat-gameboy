#pragma once
#include <cstdlib>
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
		u8* rom() const { return memory; }

	public:
		u8* memory = nullptr;
	};
}