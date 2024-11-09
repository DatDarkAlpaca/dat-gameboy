#pragma once
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <functional>
#include <vector>
#include <any>

#include "opcode.hpp"
#include "operand.hpp"
#include "microcode.hpp"
#include "microcode.hpp"

#include "common.hpp"

namespace dat
{
	struct s_Instruction
	{
	public:
		s_Instruction(
			Opcode opcode, 
			const std::string& name,
			const std::vector<s_Operand>& operands, 
			const std::vector<e_Microcode>& microcodes, 
			u8 length, u8 tStates, u8 tStatesFalse = 0)
			: opcode(opcode)
			, name(name)
			, operands(operands)
			, microcodes(microcodes)
			, tStatesRequired(tStates)
			, length(length)
		{

		}

		s_Instruction() = default;

	public:
		std::vector<e_Microcode> microcodes = {};
		std::vector<s_Operand> operands = {};
		std::string name;
		Opcode opcode = 0x0;
		u8 length = 0;
		u8 tStatesRequired = 0;
		u8 tStatesRequiredFalse = 0;
	};
}