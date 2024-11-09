#pragma once
#include <numeric>
#include "common.hpp"
#include "flags.hpp"

namespace dat
{
	enum class e_RegisterName : u32
	{
		UNKNOWN = std::numeric_limits<u32>::max(),
		A = 0,
		B,
		C,
		D,
		E,
		H,
		L,
		F,

		BC,
		DE,
		HL,
		SP,
		PC
	};

	inline const std::unordered_map<std::string_view, e_RegisterName> ValidRegisters =
	{
		{ "A", e_RegisterName::A },
		{ "B", e_RegisterName::B },
		{ "C", e_RegisterName::C },
		{ "D", e_RegisterName::D },
		{ "E", e_RegisterName::E },
		{ "H", e_RegisterName::H },
		{ "L", e_RegisterName::L },
		{ "F", e_RegisterName::F },

		{ "BC", e_RegisterName::BC },
		{ "DE", e_RegisterName::DE },
		{ "HL", e_RegisterName::HL },
		{ "SP", e_RegisterName::SP },
		{ "PC", e_RegisterName::PC },
	};

	inline bool is_valid_register_string(const std::string& value)
	{
		for (const auto& [registerName, _] : ValidRegisters)
		{
			if (registerName == value)
				return true;
		}

		return false;
	}

	inline e_RegisterName get_register_name_from_string(const std::string& value)
	{
		return ValidRegisters.at(value);
	}

	struct s_Registers
	{
	public:
		inline u8& B() { return reinterpret_cast<u8*>(&BC)[0]; }
		inline u8& C() { return reinterpret_cast<u8*>(&BC)[1]; }

		inline u8& D() { return reinterpret_cast<u8*>(&DE)[0]; }
		inline u8& E() { return reinterpret_cast<u8*>(&DE)[1]; }

		inline u8& H() { return reinterpret_cast<u8*>(&HL)[0]; }
		inline u8& L() { return reinterpret_cast<u8*>(&HL)[1]; }

	public:
		u8		 A = 0x0;
		s_Flags  F;

		u16 BC = 0x0;
		u16 DE = 0x0;
		u16 HL = 0x0;
		u16 SP = 0x0;		/* Stack Pointer */
		u16 PC = 0x0;		/* Program Counter */
	};	
}