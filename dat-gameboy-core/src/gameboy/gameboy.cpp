#include "pch.hpp"
#include "gameboy.hpp"
#include "constants.hpp"
#include "utils/byte_utils.hpp"
#include "core/event/events.hpp"
#include "instruction/instruction_set.hpp"

#include <iostream>

namespace dat
{
	s_Gameboy::s_Gameboy()
	{
		cpu.initialize(&memory);
		ppu.initialize(&memory);
	}

	void s_Gameboy::run(u8 cycles)
	{
		constexpr static u8 tCyclesPerMcycle = 4;
		u8 currentMCycle = 0;

		while (true)
		{
			if (cycles == currentMCycle)
				break;

			clock.start();
			
			if (clock.duration >= SM83Period_ns * tCyclesPerMcycle)
			{
				clock.restart();

				cpu.tick();
				ppu.tick();
				
				++currentMCycle;
			}

			clock.cycle();
		}
	}

	void s_Gameboy::load_bootloader_rom(const std::vector<u8>& data)
	{
		std::memcpy(memory.memory, data.data(), data.size());
	}

	void s_Gameboy::load_cartridge(const s_Cartridge& cartridge)
	{
		std::memcpy(memory.memory, cartridge.buffer.data, KB(64) * sizeof(u8));
	}
}