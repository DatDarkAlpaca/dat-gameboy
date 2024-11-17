#include "pch.hpp"
#include "gameboy.hpp"
#include "constants.hpp"
#include "utils/byte_utils.hpp"
#include "instruction/instruction_set.hpp"

#include <iostream>

namespace dat
{
	s_Gameboy::s_Gameboy()
	{
		cpu.initialize(&memory);
	}

	void s_Gameboy::run(u8 cycles)
	{
		constexpr static u8 tCyclesPerMcycle = 4;
		u8 currentMCycle = 0;

		while (true)
		{
			if (cycles == currentMCycle)
				break;

			m_Clock.start();
			
			if (m_Clock.duration >= SM83Period_ns * tCyclesPerMcycle)
			{
				m_Clock.restart();

				cpu.tick();
				
				++currentMCycle;
			}

			m_Clock.cycle();
		}
	}	
}