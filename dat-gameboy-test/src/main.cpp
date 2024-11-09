#include "pch.hpp"
#include <gtest/gtest.h>
#include <gameboy/gameboy.hpp>
#include <gameboy/file.hpp>
#include <gameboy/instruction/instruction_set.hpp>

class GameboyTest : public ::testing::Test
{
protected:
	GameboyTest()
	{
		using namespace dat;

		dat::initialize_logger();

		auto json = dat::load_opcodes_json("res/opcodes.json");
		dat::initialize_instruction_set(json);
	}

protected:
	void run()
	{
		gameboy.run();
	}

protected:
	dat::s_Gameboy gameboy;
};

TEST_F(GameboyTest, Test_Control)
{
	gameboy.memory.memory[0] = 0x0F;
	EXPECT_EQ(1, 1);
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}