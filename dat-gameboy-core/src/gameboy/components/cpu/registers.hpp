#pragma once
#include <numeric>
#include "common.hpp"
#include "flags.hpp"

namespace dat
{
	struct s_WordRegister
	{
	public:
		s_WordRegister() = default;

	public:
		void set_msb(u8 value) { reinterpret_cast<u8*>(&this->value)[1] = value; }
		void set_lsb(u8 value) { reinterpret_cast<u8*>(&this->value)[0] = value; }
		void set(u16 value) { this->value = value; }

	public:
		u8& get_msb() { return reinterpret_cast<u8*>(&value)[1]; }
		u8& get_lsb() { return reinterpret_cast<u8*>(&value)[0]; }
		u16& get() { return value; }

	public:
		u16& operator++() { ++value; return value; }
		u16& operator--() { --value; return value; }

		u16& operator+=(u16 value) { this->value += value; return this->value; }
		u16& operator-=(i8 value) { this->value -= value; return this->value; }

	public:
		u16 value = 0x00;
	};
}