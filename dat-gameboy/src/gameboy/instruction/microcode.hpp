#pragma once
#include <string>
#include "common.hpp"

namespace dat
{
	enum class e_Microcode
	{
		NOP = 0,
		STOP,
		FETCH_VALUE,
		FETCH_ADDRESS,
		ASSIGN,
		ASSIGN_AT,
		ASSIGN_FROM_REGISTER,
	};

	inline e_Microcode get_microcode_from_string(const std::string& value)
	{
		if (value == "NOP")
			return e_Microcode::NOP;

		if (value == "STOP")
			return e_Microcode::STOP;

		if (value == "FETCH_VALUE")
			return e_Microcode::FETCH_VALUE;

		if (value == "FETCH_ADDRESS")
			return e_Microcode::FETCH_ADDRESS;

		if (value == "ASSIGN")
			return e_Microcode::ASSIGN;

		if (value == "ASSIGN_AT")
			return e_Microcode::ASSIGN_AT;

		if (value == "ASSIGN_FROM_REGISTER")
			return e_Microcode::ASSIGN_FROM_REGISTER;

		DAT_LOG_ERROR("Invalid microcode: {}", value);
	}
}