#pragma once
#include <cstdlib>
#include "joypad.hpp"
#include "cpu/flags.hpp"
#include "cartridge/cartridge.hpp"

#include "common.hpp"

namespace dat
{
	struct s_MMU
	{
	public:
		s_MMU();

		~s_MMU();

	public:
		void set_cartridge(const dat_shared<ICartridge>& cartridge);

	public:
		u8 read(u16 address) const;

		void write(u16 address, u8 value);

	// Input:
	public:
		s_Joypad joypad() const { return s_Joypad { memory[0xFF00] }; }

		void set_joypad_input(bool value) const { }

	// VRAM:
	public:
		inline u8* vram(u8 offset = 0) const { return &memory[0x8000 + offset]; }
		inline u8* oam(u8 offset = 0)  const { return &memory[0xFE00 + offset]; }

		inline u8* vram_block0() const { return &memory[0x8000]; }
		inline u8* vram_block1() const { return &memory[0x8800]; }
		inline u8* vram_block2() const { return &memory[0x9000]; }

		inline u8* vram_tilemap0() const { return &memory[0x9800]; }
		inline u8* vram_tilemap1() const { return &memory[0x9C00]; }

		// TODO: breaking point. LCDC might need to be a reference.
		s_LCDC& lcdc() { return s_LCDC{ memory[0xFF40] }; }

		u8& LY() const { return memory[0xFF44]; }
		u8& LYC() const { return memory[0xFF45]; }
		s_STAT& STAT() { return s_STAT{ memory[0xFF41] }; }

		u8 SCY() const { return memory[0xFF42]; }
		u8 SCX() const { return memory[0xFF43]; }

		s_BGP& BGP() { return s_BGP{ memory[0xFF47] }; }
		u8& OBP0() const { return memory[0xFF48]; }
		u8& OBP1() const { return memory[0xFF49]; }

		u8 WY() const { return memory[0xFF4A]; }
		u8 WX() const { return memory[0xFF4B]; }

	// Interrupts:
	public:
		s_InterruptEnable& interrupt_enable() { return s_InterruptEnable { memory[0xFFFF] }; }
		s_InterruptEnable& interrupt_flag  () { return s_InterruptEnable { memory[0xFFFF] }; }

	public:
		inline u8* get_rom() const { return cartridge->get_rom(); }

		inline u8* get_ram() const { return cartridge->get_ram(); }

		inline u8* get_memory() const { return memory; }

	private:
		dat_shared<ICartridge> cartridge;
		u8* memory = nullptr;
		bool m_IsBootActive = true;
	};
}