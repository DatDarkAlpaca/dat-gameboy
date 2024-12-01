#include "pch.hpp"
#include "memory"
#include "memory.hpp"

namespace dat
{
	dat::s_MMU::s_MMU()
	{
		memory = (u8*)calloc(KB(64), sizeof(u8));
		if (!memory)
			DAT_LOG_CRITICAL("Failed to allocate gameboy memory");

		m_Joypad.initialize(&memory[JOYP_Address]);			// 0xFF00
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

		m_WY.initialize(&memory[WY_Address]);				// 0xFF4A
		m_WX.initialize(&memory[WX_Address]);				// 0xFF4B

		m_InterruptEnable.initialize(&memory[IE_Address]);	// 0xFFFF
	}

	s_MMU::~s_MMU()
	{
		free(memory);
	}

	void s_MMU::set_cartridge(const dat_shared<ICartridge>& cartridge)
	{
		this->cartridge = cartridge;
	}

	u8 s_MMU::read(u16 address) const
	{
		/* Bootloader & Cartridge ROM */
		// [0x0000, 0x7FFF)
		if (address >= 0x0000 && address <= 0x7FFF)
		{
			if (address >= 0x00 && address < 0xFF && bootloader_enabled())
				return memory[address];

			if (cartridge)
				return cartridge->read(address);

			return 0xFF;
		}

		/* VRAM */
		// [0x8000, 0x9FFF]
		else if (address >= 0x8000 && address <= 0x9FFF)
			return memory[address];

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
			return memory[address];

		/* Prohibited */
		// [FEA0, FEFF]
		else if (address >= 0xFEA0 && address <= 0xFEFF)
		{
			DAT_LOG_WARN("Invalid read at: {}", address);
			return memory[address];
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
			memory[address] = value;
			return;
		}

		/* Prohibited */
		// [FEA0, FEFF]
		else if (address >= 0xFEA0 && address <= 0xFEFF)
		{
			memory[address] = value;
			return;
		}

		/* IO Registers */
		// [0xFF00, 0xFF7F]
		else if (address >= 0xFF00 && address <= 0xFF7F)
		{
			// LY
			if (address == 0xFF44)
				memory[address] = 0x0;

			// DMA
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
}