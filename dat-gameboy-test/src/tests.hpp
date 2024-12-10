#pragma once
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
#include <nlohmann/json.hpp>

#include <gameboy/gameboy.hpp>
#include <gameboy/file.hpp>
#include <gameboy/instruction/instruction_set.hpp>

struct s_TestConditions
{
public:
	std::string testName;

	enum class e_CycleType
	{
		READ,
		WRITE,
		NULL_CYCLE
	};

	struct s_CycleInfo
	{
		u16 address;
		u8 value;
		e_CycleType type;
	};

	std::vector<s_CycleInfo> cycles;

	struct s_RamConditionEntry
	{
		u16 address;
		u8 value;
	};

	struct s_Conditions
	{
		u8 a;
		u8 b;
		u8 c;
		u8 d;
		u8 e;
		u8 f;
		u8 h;
		u8 l;
		u16 pc;
		u16 sp;
		std::vector<s_RamConditionEntry> ram;
	} initialCondition, finalCondition;

public:
	static inline e_CycleType get_cycle_type(const std::string& cycleString)
	{
		if (cycleString == "read")
			return e_CycleType::READ;

		else if (cycleString == "write")
			return e_CycleType::WRITE;

		else if (cycleString == "null")
			return e_CycleType::NULL_CYCLE;

		DAT_LOG_ERROR("Invalid cycle type");
	}
};

class GameboyTest : public ::testing::TestWithParam<std::string>
{
protected:
	explicit GameboyTest();

protected:
	void run_test(const std::filesystem::path& filepath);

protected:
	void prepare_cpu_data(const s_TestConditions::s_Conditions& conditions);

	void compare_cpu_data(const s_TestConditions::s_Conditions& conditions);

protected:
	dat::s_Gameboy gameboy;
};