#include "pch.hpp"
#include "memory"
#include "memory.hpp"
#include "gameboy/gameboy.hpp"
#include "gameboy/components/ppu/ppu.hpp"

namespace dat
{
	dat::s_MMU::s_MMU()
	{
		memory = (u8*)calloc(KB(64), sizeof(u8));
		if (!memory)
			DAT_LOG_CRITICAL("Failed to allocate gameboy memory");

		m_Joypad.initialize(&memory[JOYP_Address]);			// 0xFF00
		m_SB.initialize(&memory[SB_Address]);				// 0xFF01
		m_SC.initialize(&memory[SC_Address]);				// 0xFF02

		m_DIV.initialize(&memory[DIV_Address]);				// 0xFF04
		m_TIMA.initialize(&memory[TIMA_Address]);			// 0xFF05
		m_TMA.initialize(&memory[TMA_Address]);				// 0xFF06
		m_TAC.initialize(&memory[TAC_Address]);				// 0xFF07

		m_InterruptFlag.initialize(&memory[IF_Address]);	// 0xFF0F

		m_LCDC.initialize(&memory[LCDC_Address]);			// 0xFF40
		m_STAT.initialize(&memory[STAT_Address]);			// 0xFF41
		m_SCY.initialize(&memory[SCY_Address]);				// 0xFF42
		m_SCX.initialize(&memory[SCX_Address]);				// 0xFF43
		m_LY.initialize(&memory[LY_Address]);				// 0xFF44
		m_LYC.initialize(&memory[LYC_Address]);				// 0xFF45

		m_BGP.initialize(&memory[BGP_Address]);				// 0xFF47
		m_OBP0.initialize(&memory[OBP0_Address]);			// 0xFF48
		m_OBP1.initialize(&memory[OBP1_Address]);			// 0xFF49
		
		m_BANK.initialize(&memory[BANK_Address]);			// 0xFF50

		m_WY.initialize(&memory[WY_Address]);				// 0xFF4A
		m_WX.initialize(&memory[WX_Address]);				// 0xFF4B

		m_InterruptEnable.initialize(&memory[IE_Address]);	// 0xFFFF

		// Unloaded util registers:
		m_UnloadedPalette.initialize(&m_UnloadedData);
	}

	s_MMU::~s_MMU()
	{
		free(memory);
	}

	void s_MMU::initialize(NON_OWNING s_Gameboy* gameboy)
	{
		r_Gameboy = gameboy;
	}

	void s_MMU::set_cartridge(const dat_shared<ICartridge>& cartridge)
	{
		this->cartridge = cartridge;
	}

	void s_MMU::restart()
	{
		std::fill(memory, memory + KB(64), 0);
		cartridge = nullptr;
	}

	u8 s_MMU::read(u16 address)
	{
		/* Bootloader & Cartridge ROM */
		// [0x0000, 0x7FFF)
		if (address >= 0x0000 && address <= 0x7FFF)
		{
			if (address >= 0x00 && address <= 0xFF && m_BANK.is_bootloader_enabled())
				return memory[address];

			if (cartridge)
				return cartridge->read(address);

			return 0xFF;
		}

		/* VRAM */
		// [0x8000, 0x9FFF]
		else if (address >= 0x8000 && address <= 0x9FFF)
		{
			if(r_Gameboy->ppu.vram_accessible())
				return memory[address];

			return 0xFF;
		}

		/* Cartridge RAM */
		// [0xA000, 0xBFFF]
		else if (address >= 0xA000 && address <= 0xBFFF)
		{
			if (cartridge)
				return cartridge->read(address);
			return 0xFF;
		}

		/* Work RAM */
		// [0xC000, 0xDFFF]
		else if (address >= 0xC000 && address <= 0xDFFF)
			return memory[address];

		/* Mirrored Work RAM */
		// [0xE000, 0xFDFF]
		else if (address >= 0xE000 && address <= 0xFDFF)
			return memory[address - 0x2000];
	
		/* OAM */
		// [0xFE00, 0xFE9F]
		else if (address >= 0xFE00 && address <= 0xFE9F)
		{
			// TODO: check DMA transferring
			if (r_Gameboy->ppu.oam_accessible())
				return memory[address];

			return 0xFF;
		}

		/* Prohibited */
		// [FEA0, FEFF]
		else if (address >= 0xFEA0 && address <= 0xFEFF)
		{
#if defined(DAT_BYPASS_PROHIBITED)
			return memory[address];
#else
			DAT_LOG_WARN("Prohibited read at: {}", address);
			return 0xFF;
#endif
		}

		/* IO Registers */
		// [0xFF00, 0xFF7F]
		else if (address >= 0xFF00 && address <= 0xFF7F)
			return memory[address];

		/* HRAM */
		// [0xFF80, 0xFFFE]
		else if (address >= 0xFF80 && address <= 0xFFFE)
			return memory[address];

		/* Interrupt Enable register */
		else if (address == 0xFFFF)
			return memory[address];

		DAT_LOG_CRITICAL("Read from unmapped memory at address: {}", address);
		return 0xFF;
	}

	void s_MMU::write(u16 address, u8 value)
	{
		/* Bootloader & Cartridge*/
		// [0x0000, 0x7FFF)
		if (address >= 0x0000 && address <= 0x7FFF)
		{
			if (cartridge)
				return cartridge->write(address, value);

			memory[address] = value;
			return;
		}

		/* VRAM */
		// [0x8000, 0x9FFF]
		else if (address >= 0x8000 && address <= 0x9FFF)
		{
			if (!r_Gameboy->ppu.vram_accessible())
				return;

			memory[address] = value;
			return;
		}

		/* Cartridge RAM */
		// [0xA000, 0xBFFF]
		else if (address >= 0xA000 && address <= 0xBFFF)
		{
			if (cartridge)
				return cartridge->write(address, value);		

			memory[address] = value;
			return;
		}

		/* Work RAM */
		// [0xC000, 0xDFFF]
		else if (address >= 0xC000 && address <= 0xDFFF)
		{
			memory[address] = value;
			return;
		}

		/* Mirrored Work RAM */
		// [0xE000, 0xFDFF]
		else if (address >= 0xE000 && address <= 0xFDFF)
		{
			memory[address - 0x2000] = value;
			return;
		}

		/* OAM */
		// [0xFE00, 0xFE9F]
		else if (address >= 0xFE00 && address <= 0xFE9F)
		{
			if (!r_Gameboy->ppu.oam_accessible())
				return;

			memory[address] = value;
			return;
		}

		/* Prohibited */
		// [FEA0, FEFF]
		else if (address >= 0xFEA0 && address <= 0xFEFF)
		{
#if defined(DAT_BYPASS_PROHIBITED)
			memory[address] = value;
			return;
#else
			DAT_LOG_WARN("Prohibited write at: {}", address);
			return;
#endif
		}

		/* IO Registers */
		// [0xFF00, 0xFF7F]
		else if (address >= 0xFF00 && address <= 0xFF7F)
		{
			// LCDC:
			if (address == LCDC_Address)
			{
				bool lcdcOn = m_LCDC.is_bit_set(e_LCDC::LCD_PPU_ENABLE);
				memory[address] = value;

				// Display Disabled:
				if (lcdcOn && !m_LCDC.is_bit_set(e_LCDC::LCD_PPU_ENABLE))
				{
					// Screen can only be turned off during VBlank:
					if (r_Gameboy->ppu.get_video_mode() != e_VideoMode::VBLANK)
					{
						// TODO: simulate burn-in on the current scanline.
						DAT_LOG_INFO("Burn-in on line: {}", m_LY.get());

						r_Gameboy->ppu.turn_screen_off();
					}
				}

				// Display Re-enabled:
				else if(!lcdcOn && m_LCDC.is_bit_set(e_LCDC::LCD_PPU_ENABLE))
					r_Gameboy->ppu.turn_screen_on();
			}

			// STAT:
			if (address == STAT_Address)
			{
				memory[address] = value & 0xF8;

				if (m_LCDC.is_bit_set(e_LCDC::LCD_PPU_ENABLE))
					r_Gameboy->ppu.handle_stat();

				return;
			}

			// LY
			else if (address == LY_Address)
			{
				memory[address] = 0x0;
				return;
			}

			// LYC
			else if (address == LYC_Address)
			{
				memory[address] = value;

				if (m_LCDC.is_bit_set(e_LCDC::LCD_PPU_ENABLE))
				{
					r_Gameboy->ppu.handle_lyc();
					r_Gameboy->ppu.handle_stat();
				}

				return;
			}

			// DMA:
			// TODO: DMA register.
			if (address == 0xFF46)
			{
				u16 address = value * 0x100;

				for (u8 i = 0x0; i <= 0x9F; ++i) 
				{
					u16 sourceAddr = value * 0x100 + i;
					u16 targetAddr = 0xFE00 + i;

					u8 dereferencedValue = read(sourceAddr);
					write(targetAddr, dereferencedValue);
				}

				return;
			}
			
			memory[address] = value;
			return;
		}
		
		/* HRAM */
		// [0xFF80, 0xFFFE]
		else if (address >= 0xFF80 && address <= 0xFFFE)
		{
			memory[address] = value;
			return;
		}

		/* Interrupt Enable register */
		else if (address == 0xFFFF)
		{
			memory[address] = value;
			return;
		}

		DAT_LOG_ERROR("Write to unmapped memory at address: {}", address);
	}

	s_BGP& s_MMU::BGP()
	{
		if (!r_Gameboy->ppu.palettes_accessible())
			return m_UnloadedPalette;

		return m_BGP;
	}

	s_OBP0& s_MMU::OBP0()
	{
		if (!r_Gameboy->ppu.palettes_accessible())
			return m_UnloadedPalette;

		return m_OBP0;
	}

	s_OBP1& s_MMU::OBP1()
	{
		if (!r_Gameboy->ppu.palettes_accessible())
			return m_UnloadedPalette;

		return m_OBP1;
	}
}