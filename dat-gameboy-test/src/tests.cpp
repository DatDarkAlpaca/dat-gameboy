#include "pch.hpp"
#include "tests.hpp"
#include <common.hpp>

namespace
{
	static std::vector<s_TestConditions> parse_tests(const std::filesystem::path& testFilepath)
	{
		// TODO: implement cycles
		auto j = dat::load_json_file(testFilepath);
		
		std::vector<s_TestConditions> conditions;
		conditions.reserve(j.size());

		for (const auto& entry : j)
		{
			s_TestConditions testConditions;

			for (const auto& cycleEntry : entry["cycles"])
			{
				if (cycleEntry.is_array())
				{
					if (cycleEntry.size() == 3)
						testConditions.cycles.push_back({
							cycleEntry[0],
							cycleEntry[1],
							s_TestConditions::get_cycle_type(cycleEntry[2])
						});
					else
						DAT_LOG_WARN("Ill-formed test condition [cycle]");
				}
				else
					testConditions.cycles.push_back({ 0, 0, s_TestConditions::e_CycleType::NULL_CYCLE });	
			}
				
			// Initial:
			{
				testConditions.testName = entry["name"];
				testConditions.initialCondition.a  = entry["initial"]["a"];
				testConditions.initialCondition.b  = entry["initial"]["b"];
				testConditions.initialCondition.c  = entry["initial"]["c"];
				testConditions.initialCondition.d  = entry["initial"]["d"];
				testConditions.initialCondition.e  = entry["initial"]["e"];
				testConditions.initialCondition.f  = entry["initial"]["f"];
				testConditions.initialCondition.h  = entry["initial"]["h"];
				testConditions.initialCondition.l  = entry["initial"]["l"];
				testConditions.initialCondition.pc = entry["initial"]["pc"];
				testConditions.initialCondition.sp = entry["initial"]["sp"];

				for (const auto& entry : entry["initial"]["ram"])
					testConditions.initialCondition.ram.push_back({ entry[0], entry[1] });
			}

			// Final:
			{
				testConditions.testName = entry["name"];
				testConditions.finalCondition.a =  entry["final"]["a"];
				testConditions.finalCondition.b =  entry["final"]["b"];
				testConditions.finalCondition.c =  entry["final"]["c"];
				testConditions.finalCondition.d =  entry["final"]["d"];
				testConditions.finalCondition.e =  entry["final"]["e"];
				testConditions.finalCondition.f =  entry["final"]["f"];
				testConditions.finalCondition.h =  entry["final"]["h"];
				testConditions.finalCondition.l =  entry["final"]["l"];
				testConditions.finalCondition.pc = entry["final"]["pc"];
				testConditions.finalCondition.sp = entry["final"]["sp"];

				for (const auto& entry : entry["final"]["ram"])
					testConditions.finalCondition.ram.push_back({ entry[0], entry[1] });
			}

			conditions.push_back(testConditions);
		}

		return conditions;
	}
}

GameboyTest::GameboyTest()
{
	using namespace dat;

	auto opcodes = dat::load_json_file("res/opcodes.json");
	dat::initialize_instruction_set(opcodes);

	gameboy.load_cartridge(dat::make_shared<s_TestMBC>());
}

void GameboyTest::run_test(const std::filesystem::path& filepath)
{
	if (!std::filesystem::exists(filepath))
		return;

	auto& test = parse_tests(filepath);
	u64 index = 0;
	for (const auto& testEntry : test)
	{
		gameboy.cpu.restart();
		gameboy.memory.write(0xFF50, 0x1);

		GTEST_LOG_(INFO) << testEntry.testName << " | Index: " << index;
		prepare_cpu_data(testEntry.initialCondition);

		// Prefetch:
		--gameboy.cpu.PC;
		gameboy.cpu.tick();

		for (const auto& [address, _, type] : testEntry.cycles)
			gameboy.cpu.tick();

		compare_cpu_data(testEntry.finalCondition);
		++index;
	}
}

void GameboyTest::prepare_cpu_data(const s_TestConditions::s_Conditions& conditions)
{
	gameboy.cpu.A = conditions.a;
	gameboy.cpu.F.set(conditions.f);

	gameboy.cpu.BC.set_msb(conditions.b);
	gameboy.cpu.BC.set_lsb(conditions.c);
			   
	gameboy.cpu.DE.set_msb(conditions.d);
	gameboy.cpu.DE.set_lsb(conditions.e);
			   
	gameboy.cpu.HL.set_msb(conditions.h);
	gameboy.cpu.HL.set_lsb(conditions.l);
			   
	gameboy.cpu.PC.set(conditions.pc);
	gameboy.cpu.SP.set(conditions.sp);

	for (const auto& entry : conditions.ram)
		gameboy.memory.write(entry.address, entry.value);
}

void GameboyTest::compare_cpu_data(const s_TestConditions::s_Conditions& conditions)
{
	auto& cpu = gameboy.cpu;

	EXPECT_EQ(cpu.A, conditions.a);
	EXPECT_EQ(cpu.F.get_register(), conditions.f);

	EXPECT_EQ(cpu.BC.get_msb(), conditions.b);
	EXPECT_EQ(cpu.BC.get_lsb(), conditions.c);

	EXPECT_EQ(cpu.DE.get_msb(), conditions.d);
	EXPECT_EQ(cpu.DE.get_lsb(), conditions.e);

	EXPECT_EQ(cpu.HL.get_msb(), conditions.h);
	EXPECT_EQ(cpu.HL.get_lsb(), conditions.l);

	EXPECT_EQ(cpu.SP.get(), conditions.sp);
	EXPECT_EQ(cpu.PC.get(), conditions.pc);

	for (const auto& [address, value] : conditions.ram)
		EXPECT_EQ(gameboy.memory.read(address), value);
}

TEST_P(GameboyTest, RunTest_File)
{
	std::string filepath = GetParam();
	run_test(filepath);
}

#define TEST_FILES(prefix)		\
	"res/v2/" #prefix "0.json",	\
	"res/v2/" #prefix "1.json",	\
	"res/v2/" #prefix "2.json",	\
	"res/v2/" #prefix "3.json",	\
	"res/v2/" #prefix "4.json",	\
	"res/v2/" #prefix "5.json",	\
	"res/v2/" #prefix "6.json",	\
	"res/v2/" #prefix "7.json",	\
	"res/v2/" #prefix "8.json", \
	"res/v2/" #prefix "9.json", \
	"res/v2/" #prefix "a.json",	\
	"res/v2/" #prefix "b.json",	\
	"res/v2/" #prefix "c.json",	\
	"res/v2/" #prefix "d.json",	\
	"res/v2/" #prefix "e.json",	\
	"res/v2/" #prefix "f.json"	\

INSTANTIATE_TEST_SUITE_P(GameboyFileTests_0X, GameboyTest, ::testing::Values(TEST_FILES(0)));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_1X, GameboyTest, ::testing::Values(TEST_FILES(1)));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_2X, GameboyTest, ::testing::Values(TEST_FILES(2)));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_3X, GameboyTest, ::testing::Values(TEST_FILES(3)));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_4X, GameboyTest, ::testing::Values(TEST_FILES(4)));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_5X, GameboyTest, ::testing::Values(TEST_FILES(5)));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_6X, GameboyTest, ::testing::Values(TEST_FILES(6)));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_7X, GameboyTest, ::testing::Values(TEST_FILES(7)));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_8X, GameboyTest, ::testing::Values(TEST_FILES(8)));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_9X, GameboyTest, ::testing::Values(TEST_FILES(9)));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_aX, GameboyTest, ::testing::Values(TEST_FILES(a)));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_bX, GameboyTest, ::testing::Values(TEST_FILES(b)));

INSTANTIATE_TEST_SUITE_P(GameboyFileTests_c0, GameboyTest, ::testing::Values("res/v2/c0.json"));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_c1, GameboyTest, ::testing::Values("res/v2/c1.json"));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_c2, GameboyTest, ::testing::Values("res/v2/c2.json"));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_c3, GameboyTest, ::testing::Values("res/v2/c3.json"));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_c4, GameboyTest, ::testing::Values("res/v2/c4.json"));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_c5, GameboyTest, ::testing::Values("res/v2/c5.json"));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_c6, GameboyTest, ::testing::Values("res/v2/c6.json"));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_c7, GameboyTest, ::testing::Values("res/v2/c7.json"));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_c8, GameboyTest, ::testing::Values("res/v2/c8.json"));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_c9, GameboyTest, ::testing::Values("res/v2/c9.json"));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_ca, GameboyTest, ::testing::Values("res/v2/ca.json"));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_cc, GameboyTest, ::testing::Values("res/v2/cc.json"));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_cd, GameboyTest, ::testing::Values("res/v2/cd.json"));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_ce, GameboyTest, ::testing::Values("res/v2/ce.json"));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_cf, GameboyTest, ::testing::Values("res/v2/cf.json"));

INSTANTIATE_TEST_SUITE_P(GameboyFileTests_dX, GameboyTest, ::testing::Values(TEST_FILES(d)));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_eX, GameboyTest, ::testing::Values(TEST_FILES(e)));
INSTANTIATE_TEST_SUITE_P(GameboyFileTests_fX, GameboyTest, ::testing::Values(TEST_FILES(f)));

INSTANTIATE_TEST_SUITE_P(GameboyFileTests_PREFIX, GameboyTest, ::testing::Values("res/v2/cb.json"));