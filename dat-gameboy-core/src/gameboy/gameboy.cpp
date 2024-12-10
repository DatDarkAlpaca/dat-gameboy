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
		timer.initialize(&memory.DIV(), &memory.TAC(), &memory.TIMA(), &memory.TMA(), &memory.IF());
	}

	void s_Gameboy::load_bootloader_rom(const std::vector<u8>& data) const
	{
		std::memcpy(memory.get_memory(), data.data(), data.size());
	}

	void s_Gameboy::load_cartridge(const dat_shared<ICartridge>& cartridge)
	{
		restart();
		memory.set_cartridge(cartridge);
	}

	void s_Gameboy::turnOff()
	{
		m_IsTurnedOn = false;

		cpu.restart();
		ppu.restart();
		timer.restart();
		memory.restart();
	}

	void s_Gameboy::turnOn()
	{
		m_IsTurnedOn = true;
	}

	void s_Gameboy::restart()
	{
		turnOff();
		turnOn();
	}

	void s_Gameboy::togglePower()
	{
		if (m_IsTurnedOn)
			turnOff();
		else
			turnOn();
	}

	void s_Gameboy::setPaused(bool value)
	{
		m_IsPaused = value;
	}
}