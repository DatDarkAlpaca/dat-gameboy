#pragma once
#include <cstdlib>
#include "common.hpp"
#include "cpu/flags.hpp"
#include "cartridge/cartridge.hpp"
#include "registers/registers.hpp"

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

	// Hardware Registers:
	public:
		s_JOYP& joypad() { return m_Joypad; }

		s_IE& IE() { return m_InterruptEnable; }
		s_IF& IF() { return m_InterruptFlag; }

		s_LCDC& lcdc() { return m_LCDC; }
		s_STAT& STAT() { return m_STAT; }

		s_SCY& SCY() { return m_SCY; }
		s_SCX& SCX() { return m_SCX; }
		s_LY&  LY()  { return m_LY;  }
		s_LYC& LYC() { return m_LYC; }

		s_BGP&  BGP()  { return m_BGP; }
		s_OBP0& OBP0() { return m_OBP0; }
		s_OBP1& OBP1() { return m_OBP1; }

		s_WY& WY() { return m_WY; }
		s_WX& WX() { return m_WX; }

	// VRAM:
	public:
		inline u8* vram(u8 offset = 0) const { return &memory[0x8000 + offset]; }
		inline u8* oam(u8 offset = 0)  const { return &memory[0xFE00 + offset]; }

		inline u8* vram_block0() const { return &memory[0x8000]; }
		inline u8* vram_block1() const { return &memory[0x8800]; }
		inline u8* vram_block2() const { return &memory[0x9000]; }

		inline u8* vram_tilemap0() const { return &memory[0x9800]; }
		inline u8* vram_tilemap1() const { return &memory[0x9C00]; }

	public:
		inline bool bootloader_enabled() const { return memory[0xFF50] != 0x1; }
		
		inline u8* get_rom() const { return cartridge->get_rom(); }

		inline u8* get_ram() const { return cartridge->get_ram(); }

		inline u8* get_memory() const { return memory; }

	private:
		s_JOYP m_Joypad;
		s_IF m_InterruptFlag;

		s_LCDC m_LCDC;
		s_STAT m_STAT;
		s_SCY m_SCY;
		s_SCX m_SCX;
		s_LY m_LY;
		s_LYC m_LYC;

		s_BGP m_BGP;
		s_OBP0 m_OBP0;
		s_OBP1 m_OBP1;

		s_WY m_WY;
		s_WX m_WX;

		s_IE m_InterruptEnable;

	private:
		dat_shared<ICartridge> cartridge;
		u8* memory = nullptr;
	};
}