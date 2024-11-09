#include "pch.hpp"
#include "src/common.hpp"
#include "utils/string.hpp"

#include "instruction_set.hpp"
#include "gameboy/components/registers.hpp"

namespace dat
{
	static inline Opcode get_opcode(const nlohmann::json& key)
	{
		return static_cast<Opcode>(get_hex_from_string(key));
	}

	static inline u8 get_length(const nlohmann::json& value)
	{
		return value["bytes"];
	}

	static inline std::string get_instruction_name(const nlohmann::json& value)
	{
		std::string instructionName = value["mnemonic"];

		if (!value.contains("operands"))
			return instructionName;

		auto operands = value["operands"];
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
		u8 tStatesFalse;

		try {
			tStatesFalse = value["cycles"].at(1);
		}
		catch (nlohmann::json::out_of_range e) {
			tStatesFalse = 0;
		}

		return tStatesFalse;
	}

	static inline std::vector<e_Microcode> get_microcodes(const nlohmann::json& value, const std::string& instructionName)
	{
		if (!value.contains("microcodes"))
		{
			DAT_LOG_WARN("Instruction '{}' does not have microcodes", instructionName);
			return {};
		}

		std::vector<e_Microcode> microcodes;
		for (const auto& microcode : value["microcodes"])
		{
			std::string sanitized = nlohmann::to_string(microcode);
			remove_char(sanitized, '"');
			microcodes.push_back(get_microcode_from_string(sanitized));
		}

		if (microcodes.size())
		{
			/* TODO: Check if the amount of cycles match. */
		}

		return microcodes;
	}

	static inline std::vector<s_Operand> get_operands(const nlohmann::json& value)
	{
		// todo: implement "increment" and "decrement"

		std::vector<s_Operand> operands;
		for (const auto& operandJson : value["operands"])
		{
			s_Operand operand;

			for (const auto& [key, value] : operandJson.items())
			{
				if (key == "name")
				{
					if (is_valid_operand_string(value))
					{
						operand.type = get_operand_type_from_string(value);

						switch (operand.type)
						{
						case e_OperandType::REGISTER_8BIT:
							operand.value = static_cast<u16>(get_register_name_from_string(value));
							break;

						case e_OperandType::REGISTER_16BIT:
							operand.value = static_cast<u16>(get_register_name_from_string(value));
							break;
						}
					}

					if (starts_with(nlohmann::to_string(value), '$'))
					{
						operand.type = e_OperandType::NUMERIC;
						operand.value = get_hex_from_string_amperstand_initiated(nlohmann::to_string(value));
					}
				}

				else if (key == "immediate")
					operand.isImmediate = value;
			}

			operands.push_back(operand);
		}

		return operands;
	}
}

namespace dat
{
	void initialize_instruction_set(const nlohmann::json& j)
	{
		auto unprefixed = j["unprefixed"];

		InstructionSet.reserve(unprefixed.size());

		for (const auto& [key, value] : unprefixed.items())
		{
			Opcode opcode = get_opcode(key);
			u8 length = get_length(value);

			std::string name = get_instruction_name(value);

			u8 tStatesTrue = get_t_states_true(value);
			u8 tStatesFalse = get_t_states_false(value);

			std::vector<e_Microcode> microcodes = get_microcodes(value, name);

			std::vector<s_Operand> operands = get_operands(value);
			
			InstructionSet[opcode] = s_Instruction(
				opcode, 
				name, 
				operands,
				microcodes, 
				length, 
				tStatesTrue,
				tStatesFalse
			);
		}
	}
}