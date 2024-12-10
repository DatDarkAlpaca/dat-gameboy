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
		void load_bootloader_rom(const std::vector<u8>& data) const;

		void load_cartridge(const dat_shared<ICartridge>& cartridge);

	public:
		bool isOn() const { return m_IsTurnedOn; }

		void turnOff();

		void turnOn();

		void restart();

		void togglePower();

	public:
		s_SharpSM83 cpu;
		s_MMU memory;
		s_PPU ppu;
		s_Timer timer;

	private:
		bool m_IsTurnedOn = false;
	};
}