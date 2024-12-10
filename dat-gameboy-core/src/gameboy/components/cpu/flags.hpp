#pragma once
#include "common.hpp"

namespace dat
{
	struct s_SM83Flags
	{
	public:
		u8 get_register() const
		{
			return (Z << 7) | (N << 6) | (H << 5) | (C << 4);
		}

		void set(u8 value)
		{
			Z = (value >> 7) & 1;
			N = (value >> 6) & 1;
			H = (value >> 5) & 1;
			C = (value >> 4) & 1;
		}

	public:
		u8 Z : 1;
		u8 N : 1;
		u8 H : 1;
		u8 C : 1;
	};

	// TODO: move
	struct s_SpriteAttributeFlags
	{
	public:
		s_SpriteAttributeFlags(u8& value)
			: m_Register(value)
		{
			priority	= (value >> 7) & 1;
			yFlip		= (value >> 6) & 1;
			xFlip		= (value >> 5) & 1;
			dmgPalette  = (value >> 4) & 1;
			bank		= (value >> 3) & 1;
			cgbPalette  = (value >> 0) & 7;
		}

	public:
		void commit()
		{
			m_Register = get_value();
		}

		u8 get_value() const
		{
			return
				(priority << 7) |
				(yFlip << 6) |
				(xFlip << 5) |
				(dmgPalette << 4) |
				(bank << 3) |
				(cgbPalette << 0);
		}

	public:
		u8 priority : 1; // 7
		u8 yFlip : 1; // 6
		u8 xFlip : 1; // 5
		u8 dmgPalette : 1; // 4
		u8 bank : 1; // 3
		u8 cgbPalette : 3; // 0 - 2

	private:
		u8& m_Register;
	};
}