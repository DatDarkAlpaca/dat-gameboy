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
		void load_bootloader_rom(const std::vector<u8>& data);

		void load_cartridge(const dat_shared<ICartridge>& cartridge);

	public:
		inline bool isOn() const { return m_IsTurnedOn; }

		inline bool isPaused() const { return m_IsPaused; }

		void turnOff();

		void turnOn();

		void restart();

		void togglePower();

		void setPaused(bool value);

	public:
		s_SharpSM83 cpu;
		s_MMU memory;
		s_PPU ppu;
		s_Timer timer;

	private:
		std::vector<u8> m_LoadedBoot;

	private:
		bool m_IsTurnedOn = false;
		bool m_IsPaused = false;
	};
}