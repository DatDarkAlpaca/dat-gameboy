#pragma once
#include "components/components.hpp"
#include "instruction/instruction.hpp"

namespace dat
{
	class s_Gameboy
	{	
	public:
		s_Gameboy();

	public:
		void run(u8 cycles = 1);

	public:
		void load_bootloader_rom(const std::vector<u8>& data);

		void load_cartridge(const s_Cartridge& cartridge);

	public:
		s_PPU ppu;
		s_SharpSM83 cpu;
		s_Memory memory;
		s_Clock clock;
	};
}