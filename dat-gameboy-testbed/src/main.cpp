#include "pch.hpp"
#include <gameboy/gameboy.hpp>
#include <gameboy/file.hpp>
#include <gameboy/instruction/instruction_set.hpp>

// https://gbdev.io/gb-opcodes/optables/
// https://rgbds.gbdev.io/docs/v0.8.0/gbz80.7#ADC_A,r8
// Game Boy: Complete Technical Reference (https://gekkio.fi)
// https://gbdev.io/pandocs/CPU_Registers_and_Flags.html
// https://gbdev.gg8.se/wiki/articles/Gameboy_Bootstrap_ROM#The_.27Pokemon.27_CGB_bootstrap

int main(int argc, char** argv)
{
	using namespace dat;

	dat::initialize_logger();

	auto json = dat::load_json_file("res/opcodes.json");
	dat::initialize_instruction_set(json);

	dat::s_Gameboy gameboy;
	dat::load_ROM_file("res/DMG_ROM.bin", gameboy.memory.rom());
	gameboy.run(100);
}