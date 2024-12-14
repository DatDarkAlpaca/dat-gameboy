#pragma once
#include <cstdlib>
#include "common.hpp"
#include "cpu/flags.hpp"
#include "cartridge/cartridge.hpp"
#include "registers/registers.hpp"

namespace dat
{
	class s_Gameboy;

	struct s_MMU
	{
	public:
		s_MMU();

		~s_MMU();

	public:
		void initialize(NON_OWNING s_Gameboy* gameboy);

	public:
		void set_cartridge(const dat_shared<ICartridge>& cartridge);

		void restart();

	public:
		u8 read(u16 address);

		void write(u16 address, u8 value);

	// Hardware Registers:
	public:
		s_JOYP& joypad() { return m_Joypad; }
		s_SB& SB() { return m_SB; }
		s_SC& SC() { return m_SC; }

		s_DIV& DIV()	{ return m_DIV; }
		s_TIMA& TIMA()	{ return m_TIMA; }
		s_TMA& TMA()	{ return m_TMA; }
		s_TAC& TAC()	{ return m_TAC; }

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

		s_BANK& BANK() { return m_BANK; }

		s_WY& WY() { return m_WY; }
		s_WX& WX() { return m_WX; }

	// VRAM:
	public:
		inline u8* vram(u8 offset = 0) const { return &memory[VRAM_Block0_Address + offset]; }
		inline u8* oam(u8 offset = 0)  const { return &memory[OAM_Address + offset]; }

		inline u8* vram_block0() const { return &memory[VRAM_Block0_Address]; }
		inline u8* vram_block1() const { return &memory[VRAM_Block1_Address]; }
		inline u8* vram_block2() const { return &memory[VRAM_Block2_Address]; }

		inline u8* vram_tilemap0() const { return &memory[VRAM_Tilemap_0_Address]; }
		inline u8* vram_tilemap1() const { return &memory[VRAM_Tilemap_1_Address]; }

	public:		
		inline u8* get_rom() const { return cartridge->get_rom(); }

		inline u8* get_ram() const { return cartridge->get_ram(); }

		inline u8* get_memory() const { return memory; }

	private:
		void tick_components();

	private:
		s_JOYP m_Joypad;
		s_SB m_SB;
		s_SC m_SC;

		s_DIV m_DIV;
		s_TIMA m_TIMA;
		s_TMA m_TMA;
		s_TAC m_TAC;

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

		s_BANK m_BANK;

		s_WY m_WY;
		s_WX m_WX;

		s_IE m_InterruptEnable;

	private:
		dat_shared<ICartridge> cartridge;
		u8* memory = nullptr;

	private:
		NON_OWNING s_Gameboy* r_Gameboy = nullptr;
	};
}