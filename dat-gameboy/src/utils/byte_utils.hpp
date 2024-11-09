#pragma once
#include "common.hpp"

namespace dat
{
	inline bool half_carry_overflow(u8 a, u8 b)
	{
		return ((a & 0x0F) + (b & 0x0F)) > 0x0F;
	}

	inline bool half_carry_overflow_16(uint16_t a, uint16_t b) 
	{
		return ((a & 0x0FFF) + (b & 0x0FFF)) > 0x0FFF;
	}

	inline bool carry_overflow(u8 a, u8 b)
	{
		return b > a;
	}

	inline bool carry_overflow_16(uint16_t a, uint16_t b) 
	{
		return static_cast<uint32_t>(a) + static_cast<uint32_t>(b) > 0xFFFF;
	}

	inline bool half_borrow(u8 a, u8 b)
	{
		return (a & 0x0F) < (b & 0x0F);
	}

	inline bool full_borrow(u8 a, u8 b)
	{
		return a < b;
	}

	// u8 - u16 concatenate:
	inline u16 concatenate_u8_little_endian(u8 a, u8 b)
	{
		return a | (b << 8);
	}
}