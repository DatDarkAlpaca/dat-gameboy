#pragma once
#include <nlohmann/json.hpp>
#include <unordered_map>

#include "opcode.hpp"
#include "instruction.hpp"

namespace dat
{
	using InstructionSet_t = std::unordered_map<opcode, s_Instruction>;

	inline InstructionSet_t InstructionSet;
	inline InstructionSet_t PrefixedInstructionSet;

	void initialize_instruction_set(const nlohmann::json& j);

	inline s_Instruction get_instruction_from_opcode(opcode instructionOpcode, bool isPrefixinstruction = false)
	{
		if (isPrefixinstruction)
			return PrefixedInstructionSet.at(instructionOpcode);
		else
			return InstructionSet.at(instructionOpcode);
	}
}