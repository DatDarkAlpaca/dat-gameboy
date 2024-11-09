#pragma once
#include <unordered_map>

namespace dat 
{
	// TODO: conditional operands

	enum class e_OperandType
	{
		NONE = 0,

		REGISTER_8BIT,
		REGISTER_16BIT,

		IMMEDIATE_8BIT,			// n8
		IMMEDIATE_16BIT,		// n16 (little-endian)

		IMMEDIATE_8BIT_ADDR,	// a8 (n8 + 0xFF00)	
		IMMEDIATE_16BIT_ADDR,	// a16 (little-endian)	
		IMMEDIATE_8BIT_SIGNED,	// s8

		NUMERIC
	};

	const std::unordered_map<std::string, e_OperandType> ValidOperandTypes =
	{
		{ "A", e_OperandType::REGISTER_8BIT },
		{ "F", e_OperandType::REGISTER_8BIT },
		{ "B", e_OperandType::REGISTER_8BIT },
		{ "C", e_OperandType::REGISTER_8BIT },
		{ "D", e_OperandType::REGISTER_8BIT },
		{ "E", e_OperandType::REGISTER_8BIT },
		{ "H", e_OperandType::REGISTER_8BIT },
		{ "L", e_OperandType::REGISTER_8BIT },

		{ "BC", e_OperandType::REGISTER_16BIT },
		{ "DE", e_OperandType::REGISTER_16BIT },
		{ "HL", e_OperandType::REGISTER_16BIT },
		{ "SP", e_OperandType::REGISTER_16BIT },
		{ "PC", e_OperandType::REGISTER_16BIT },

		{ "n8", e_OperandType::IMMEDIATE_8BIT },
		{ "n16", e_OperandType::IMMEDIATE_16BIT },

		{ "a8", e_OperandType::IMMEDIATE_8BIT_ADDR },
		{ "a16", e_OperandType::IMMEDIATE_16BIT_ADDR },

		{ "s8", e_OperandType::IMMEDIATE_8BIT_SIGNED },
	};

	struct s_Operand
	{
		e_OperandType type = e_OperandType::NONE;
		bool isImmediate = true;
		u16 value = 0;
	};

	inline bool is_valid_operand_string(const std::string& value)
	{
		for (const auto& [operand, operandValue] : ValidOperandTypes)
		{
			if (operand == value)
				return true;
		}

		return false;
	}

	inline e_OperandType get_operand_type_from_string(const std::string& value)
	{
		return ValidOperandTypes.at(value);
	}
}