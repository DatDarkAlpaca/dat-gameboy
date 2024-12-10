#include "pch.hpp"
#include "application.hpp"

#include <iostream>

// https://gbdev.io/gb-opcodes/optables/
// https://rgbds.gbdev.io/docs/v0.8.0/gbz80.7#ADC_A,r8
// Game Boy: Complete Technical Reference (https://gekkio.fi)
// https://gbdev.io/pandocs/CPU_Registers_and_Flags.html
// https://gbdev.gg8.se/wiki/articles/Gameboy_Bootstrap_ROM#The_.27Pokemon.27_CGB_bootstrap

int main(int argc, char** argv)
{
	dat::initialize_logger();
	dat::initialize_platform();	

	dat::s_DatApplication application;
	application.run();
}