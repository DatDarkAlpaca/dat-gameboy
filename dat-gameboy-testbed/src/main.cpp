#include "pch.hpp"
#include <gameboy/gameboy.hpp>
#include <gameboy/file.hpp>
#include <gameboy/instruction/instruction_set.hpp>

int main(int argc, char** argv)
{
	using namespace dat;

	dat::initialize_logger();

	auto json = dat::load_opcodes_json("res/opcodes.json");
	dat::initialize_instruction_set(json);

	dat::s_Gameboy gameboy;
	dat::load_ROM_file("res/test.bin", gameboy.memory.rom());
	gameboy.run(4);
}