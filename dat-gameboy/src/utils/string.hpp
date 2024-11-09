#pragma once
#include <sstream>
#include "common.hpp"

namespace dat
{
	inline void remove_char(std::string& value, char character)
	{
		value.erase(remove(value.begin(), value.end(), character), value.end());
	}

	inline u64 get_hex_from_string(const std::string& value)
	{
		// For strings with the following format: 0x0A
		auto x = std::stoul(value, nullptr, 16);
		return x;
	}

	inline u64 get_hex_from_string_amperstand_initiated(const std::string& value)
	{
		// For strings with the following format: $10
		std::string temp = value;
		return std::stoul(temp.replace(0, 1, "0x"), nullptr, 16);
	}

	inline bool starts_with(const std::string& value, char c)
	{
		return value[0] == c;
	}
}