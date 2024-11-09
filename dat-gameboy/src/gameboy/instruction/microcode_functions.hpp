#pragma once
#include "common.hpp"
#include "gameboy/instruction/operand.hpp"

namespace dat
{
	class s_Gameboy;

	void nop();

	void stop(s_Gameboy& gameboy);

	void fetch_value(s_Gameboy& gameboy, const std::vector<s_Operand>& operands);

	void fetch_address(s_Gameboy& gameboy, const std::vector<s_Operand>& operands);

	// Assign:
	void assign(s_Gameboy& gameboy, const std::vector<s_Operand>& operands);

	void assign_at(s_Gameboy& gameboy, const std::vector<s_Operand>& operands);

	void assign_from_register(s_Gameboy& gameboy, const std::vector<s_Operand>& operands);
}