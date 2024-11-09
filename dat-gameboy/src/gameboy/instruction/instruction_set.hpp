#pragma once
#include <unordered_map>

#include "opcode.hpp"
#include "instruction.hpp"

namespace dat
{
	inline std::unordered_map<Opcode, s_Instruction> InstructionSet;

	void initialize_instruction_set(const nlohmann::json& j);

	inline s_Instruction get_instruction_from_byte(u8 instructionOpcode)
	{
		return InstructionSet.at(instructionOpcode);
	}
}