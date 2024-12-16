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
		cpu.initialize(this, &memory);
		memory.initialize(this);
		ppu.initialize(&memory);
		timer.initialize(&memory.DIV(), &memory.TAC(), &memory.TIMA(), &memory.TMA(), &memory.IF());
	}

	void s_Gameboy::tick(e_Component callerComponent)
	{
		for (u8 i = 0; i < 4; ++i)
		{
			if (callerComponent & e_Component::CPU)
				cpu.tick();

			if (callerComponent & e_Component::PPU)
				ppu.tick();

			if (callerComponent & e_Component::TIMER)
				timer.tick();
		}
	}

	void s_Gameboy::tick_all_except_cpu()
	{
		for (u8 i = 0; i < 4; ++i)
		{
			ppu.tick();	
			timer.tick();
		}
	}

	void s_Gameboy::load_bootloader_rom(const std::vector<u8>& data)
	{
		m_LoadedBoot = data;
		std::memcpy(memory.get_memory(), m_LoadedBoot.data(), m_LoadedBoot.size());
	}

	void s_Gameboy::load_cartridge(const dat_shared<ICartridge>& cartridge)
	{
		restart();
		memory.set_cartridge(cartridge);
	}

	void s_Gameboy::turnOff()
	{
		m_IsTurnedOn = false;
		restart();
	}

	void s_Gameboy::turnOn()
	{
		m_IsTurnedOn = true;
	}

	void s_Gameboy::restart()
	{
		cpu.restart();
		ppu.restart();
		timer.restart();

		memory.restart();
		if(!m_LoadedBoot.empty())
			std::memcpy(memory.get_memory(), m_LoadedBoot.data(), m_LoadedBoot.size());
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