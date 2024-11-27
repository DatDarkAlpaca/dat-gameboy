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

	void s_Gameboy::load_bootloader_rom(const std::vector<u8>& data) const
	{
		std::memcpy(memory.get_memory(), data.data(), data.size());
	}

	void s_Gameboy::load_cartridge(const dat_shared<ICartridge>& cartridge)
	{
		cpu.restart();
		memory.set_cartridge(cartridge);
	}
}