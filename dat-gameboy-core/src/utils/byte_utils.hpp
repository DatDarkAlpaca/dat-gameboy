#pragma once
#include "common.hpp"

namespace dat
{
	inline u8 msb(u16 value)
	{
		return static_cast<u8>(value & 0x0F);
	}

	inline u8 lsb(u16 value)
	{
		return static_cast<u8>(value >> 8);
	}

	inline u8 bit_value(u8 value, u8 bitPosition)
	{
		return (value >> bitPosition) & 1;
	}

	inline bool check_bit(u8 value, u8 bitPosition)
	{
		return value & (1 << bitPosition);
	}

	inline void set_bit(u8& target, u8 bitPosition)
	{
		target |= (1 << bitPosition);
	}

	inline void reset_bit(u8& target, u8 bitPosition)
	{
		target &= ~(1 << bitPosition);
	}

	inline void set_bit_value(u8& target, u8 bitPosition, bool value)
	{
		if (value)
			set_bit(target, bitPosition);
		else
			reset_bit(target, bitPosition);
	}	

	// 8-bit:
	inline bool check_half_overflow(u8 targetBefore, u8 valueAdded, u8 carry = 0)
	{
		return ((targetBefore & 0xF) + (valueAdded & 0xF) + carry) > 0xF;
	}

	inline bool check_overflow(u16 value)
	{
		return static_cast<u16>(value) > 0xFF;
	}

	inline bool check_half_borrow(u8 targetBefore, u8 valueSubtracted, u8 carry = 0)
	{
		return static_cast<int>((targetBefore & 0xF) - (valueSubtracted & 0xF) - carry) < 0;
	}

	inline bool check_full_borrow(u8 targetBefore, u8 value)
	{
		return targetBefore < value;
	}

	// 16-bit:
	inline bool check_half_overflow_16(u16 targetBefore, u16 valueAdded)
	{
		return (targetBefore & 0xFFF) + (valueAdded & 0xFFF) > 0xFFF;
	}

	inline bool check_overflow_16(u16 targetBefore, u16 valueAdded)
	{
		return ((targetBefore + valueAdded) & 0x10000) != 0;
	}
}