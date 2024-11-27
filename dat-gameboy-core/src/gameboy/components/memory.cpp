#include "pch.hpp"
#include "memory"
#include "memory.hpp"

namespace dat
{
	dat::s_MMU::s_MMU()
	{
		memory = (u8*)calloc(KB(64), sizeof(u8));
	}

	s_MMU::~s_MMU()
	{
		free(memory);
	}

	void s_MMU::set_cartridge(const dat_shared<ICartridge>& cartridge)
	{
		this->cartridge = cartridge;
		std::memcpy(memory, cartridge->get_rom(), KB(16));

		m_IsBootActive = false;
	}

	u8 s_MMU::read(u16 address) const
	{
		/* Bootloader & Cartridge ROM */
		// [0x0000, 0x7FFF)
		if (address >= 0x0000 && address <= 0x7FFF)
		{
			if (address >= 0x00 && address < 0xFF && m_IsBootActive)
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
		}

		/* Work RAM */
		// [0xC000, 0xDFFF]
		else if (address >= 0xC000 && address <= 0xDFFF)
			return memory[address];

		/* Mirrored Work RAM */
		// [0xE000, 0xFDFF]
		else if (address >= 0xE000 && address <= 0xFDFF)
		{
			DAT_LOG_WARN("Read from mirrored work ram at: {}", address);
			return memory[address];
		}

		/* OAM */
		// [0xFE00, 0xFE9F]
		else if (address >= 0xFE00 && address <= 0xFE9F)
			return memory[address];

		/* Prohibited */
		// [FEA0, FEFF]
		else if (address >= 0xFEA0 && address <= 0xFEFF)
		{
			DAT_LOG_WARN("Read from prohibited address: {}", address);
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
			return cartridge->write(address, value);
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
			return cartridge->write(address, value);
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
			DAT_LOG_WARN("Write to mirrored work ram at address: {}", address);
			memory[address] = value;
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
			DAT_LOG_WARN("Write to prohibited address: {}", address);
			memory[address] = value;
			return;
		}

		/* IO Registers */
		// [0xFF00, 0xFF7F]
		else if (address >= 0xFF00 && address <= 0xFF7F)
		{
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