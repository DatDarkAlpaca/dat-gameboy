#include "pch.hpp"
#include "microcode_functions.hpp"
#include "gameboy/gameboy.hpp"
#include "gameboy/components/registers.hpp"

namespace dat
{
	void nop()
	{
	}

	void stop(s_Gameboy& gameboy)
	{
		// TODO: implement
	}

	void fetch_value(s_Gameboy& gameboy, const std::vector<s_Operand>& operands)
	{
		// TODO: 16-bit
		auto& second = operands[1];

		if (second.type == e_OperandType::REGISTER_8BIT)
		{
			u8 byte = gameboy.get_register(static_cast<u8>(second.value));
			gameboy.push_argument(byte);
		}
		else if (second.type == e_OperandType::IMMEDIATE_8BIT)
		{
			u8 byte = gameboy.fetch_byte();
			gameboy.push_argument(byte);
		}
	}

	void fetch_address(s_Gameboy& gameboy, const std::vector<s_Operand>& operands)
	{
		auto& second = operands[1];
		DAT_ASSERT(second.type == e_OperandType::REGISTER_16BIT, "Invalid operand");
		gameboy.push_argument(gameboy.dereference_register(second.value));
	}

	void assign(s_Gameboy& gameboy, const std::vector<s_Operand>& operands)
	{
		auto& first = operands[0];

		if (first.type == e_OperandType::REGISTER_8BIT)
		{
			u8& targetRegister = gameboy.get_register(static_cast<u8>(first.value));
			targetRegister = gameboy.arguments[0];
		}
		else if (first.type == e_OperandType::REGISTER_16BIT)
		{
			/* TODO: Implement */
		}

		gameboy.clear_arguments();

		// TODO: implement increment microcodes
	}

	void assign_at(s_Gameboy& gameboy, const std::vector<s_Operand>& operands)
	{
		auto& first = operands[0];
		DAT_ASSERT(first.type == e_OperandType::REGISTER_16BIT, "Invalid operand");

		// LD [BC], 0x01

		u8& target = gameboy.dereference_register(first.value);
		target = gameboy.arguments[0];
	
		gameboy.clear_arguments();

		// TODO: implement increment microcodes
	}

	void assign_from_register(s_Gameboy& gameboy, const std::vector<s_Operand>& operands)
	{
		// TODO: 16-bit
		auto& first  = operands[0];
		auto& second = operands[1];

		if (first.type == e_OperandType::REGISTER_8BIT)
		{
			if (second.type == e_OperandType::REGISTER_8BIT)
			{
				u8& target = gameboy.get_register(static_cast<u8>(first.value));
				u8 value = gameboy.get_register(static_cast<u8>(second.value));

				target = value;
			}
		}
	}
}

