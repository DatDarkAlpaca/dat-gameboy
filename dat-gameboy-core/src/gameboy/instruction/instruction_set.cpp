#include "pch.hpp"
#include "src/common.hpp"
#include "utils/string.hpp"

#include "instruction_set.hpp"
#include "gameboy/components/cpu/registers.hpp"

namespace dat
{
	static inline opcode get_opcode(const nlohmann::json& key)
	{
		return static_cast<opcode>(get_hex_from_string(key));
	}

	static inline u8 get_bytes(const nlohmann::json& value)
	{
		return value["bytes"];
	}

	static inline std::string get_instruction_name(const nlohmann::json& value)
	{
		std::string instructionName = value["mnemonic"];

		if (!value.contains("operands"))
			return instructionName;

		const auto& operands = value["operands"];
		for (const auto& operand : operands)
		{
			bool isImmediate = operand.at("immediate");
			bool isIncrement = operand.value("increment", false);
			bool isDecrement = operand.value("decrement", false);
			instructionName += "_";

			if (isImmediate)
				instructionName += operand["name"];
			else
			{
				instructionName += "[";
				instructionName += operand["name"];
			}

			if (isIncrement)
				instructionName += "+";

			if (isDecrement)
				instructionName += "-";

			if (!isImmediate)
				instructionName += "]";
		}

		return instructionName;
	}

	static inline u8 get_t_states_true(const nlohmann::json& value)
	{
		return value["cycles"][0];
	}

	static inline u8 get_t_states_false(const nlohmann::json& value)
	{
		auto x = value["cycles"].size();

		if (value["cycles"].size() == 1)
			return 0;

		return value["cycles"].at(1);
	}

	static inline void create_instruction_set(const nlohmann::json& set, InstructionSet_t& instructionSet, bool isCB = false)
	{
		for (const auto& [key, value] : set.items())
		{
			opcode opcode = get_opcode(key);
			u8 bytes = get_bytes(value);

			std::string name = get_instruction_name(value);

			u8 tStates = get_t_states_true(value);
			u8 tStatesFalse = get_t_states_false(value);

			// CB instructions act like 2-byte opcodes. The table accounts for this fact already.
			if (isCB)
				tStates -= 4;

			instructionSet[opcode] = s_Instruction(
				name,
				opcode,
				bytes,
				tStates,
				tStatesFalse
			);
		}
	}
}

namespace dat
{
	void initialize_instruction_set(const nlohmann::json& j)
	{
		// Unprefixed:
		{
			const auto& unprefixed = j["unprefixed"];
			InstructionSet.reserve(unprefixed.size());
			create_instruction_set(unprefixed, InstructionSet);
		}

		// CB Prefixed:
		{
			const auto& prefixed = j["cbprefixed"];
			PrefixedInstructionSet.reserve(prefixed.size());
			create_instruction_set(prefixed, PrefixedInstructionSet, true);
		}
	}
}