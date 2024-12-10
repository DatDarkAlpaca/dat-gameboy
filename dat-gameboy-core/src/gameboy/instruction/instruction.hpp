#pragma once
#include <string>
#include "common.hpp"
#include "opcode.hpp"

namespace dat
{
	struct s_Instruction
	{
	public:
		s_Instruction(const std::string& name, opcode opcode, u8 bytes, u8 tStates, u8 tStatesFalse = 0)
			: opcode(opcode)
			, name(name)
			, bytes(bytes)
			, tStates(tStates)
			, tStatesFalse(tStatesFalse)
		{

		}

		s_Instruction() = default;

	public:
		std::string name;
		opcode opcode = 0x0;
		u8 bytes = 0;
		u8 tStates = 0;
		u8 tStatesFalse = 0;
	};
}