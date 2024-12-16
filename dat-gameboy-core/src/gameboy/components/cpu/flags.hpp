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
}