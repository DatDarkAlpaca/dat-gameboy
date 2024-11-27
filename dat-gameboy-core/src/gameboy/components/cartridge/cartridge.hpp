#pragma once
#include <core/buffer.hpp>
#include <filesystem>
#include <fstream>

#include "common.hpp"
#include "gameboy/constants.hpp"
#include "cartridge_header.hpp"

namespace dat
{
	struct ICartridge
	{
	public:
		ICartridge(const std::filesystem::path& filepath, const std::vector<u8>& rom, s_CartridgeHeader header)
			: rom(rom)
			, header(header)
			, filepath(filepath)
		{
			this->rom.resize(get_rom_size(header));
		}

		ICartridge() = default;

		virtual ~ICartridge() = default;

	public:
		virtual u8 read(u16 address) const = 0;

		virtual void write(u16 address, u8 value) = 0;

	public:
		u8* get_rom() { return rom.data(); }

		u8* get_ram() { return ram.data(); }

	protected:
		std::vector<u8> rom;
		std::vector<u8> ram;

	public:
		s_CartridgeHeader header;
		std::filesystem::path filepath;
	};

	struct s_TestMBC : public ICartridge
	{
	public:
		s_TestMBC()
			: ICartridge("", std::vector<u8>{}, { 0 })
		{
			ram.resize(KB(8));
		}

	public:
		u8 read(u16 address) const override
		{
			if (address >= 0x0000 && address <= 0x7FFF)
				return rom[address];

			if (address >= 0xA000 && address <= 0xBFFF)
				return ram[address - 0xA000];
		}

		void write(u16 address, u8 value) override
		{
			if (address >= 0x0000 && address <= 0x7FFF)
				rom[address] = value;

			if (address >= 0xA000 && address <= 0xBFFF)
				ram[address - 0xA000] = value;
		}
	};

	struct s_NoMBC : public ICartridge
	{
	public:
		s_NoMBC(const std::filesystem::path& filepath, const std::vector<u8>& rom, s_CartridgeHeader header)
			: ICartridge(filepath, rom, header)
		{

		}

	public:
		u8 read(u16 address) const override
		{
			return rom[address];
		}

		void write(u16 address, u8 value) override
		{
			DAT_LOG_WARN("Cartridges without MBC cannot be written to.");
		}
	};

	struct s_MBC1 : public ICartridge
	{
	public:
		s_MBC1(const std::filesystem::path& filepath, const std::vector<u8>& data, s_CartridgeHeader header)
			: ICartridge(filepath, data, header)
		{
			this->ram.resize(get_ram_size(header));
			m_RomBank = 0x1;
		}

	public:
		u8 read(u16 address) const override
		{
			if (address < 0x4000)
				return rom[address];

			else if (address < 0x8000)
			{
				u32 offset = m_RomBank * 0x4000 + (address - 0x4000);
				return rom[offset];
			}

			else if (address >= 0xA000 && address < 0xC000)
			{
				if (!m_RamEnabled)
					return 0xFF;

				uint32_t offset = (m_RamBank * 0x2000) + (address - 0xA000);
				return ram[offset];
			}

			DAT_LOG_WARN("Invalid write to cartridge. Check the MMU intervals.");
			return 0xFF;
		}

		void write(u16 address, u8 value) override
		{
			if (address < 0x2000)
			{
				m_RamEnabled = (value & 0x0F) == 0x0A;
				return;
			}

			else if (address < 0x4000)
			{
				m_RomBank = value & 0x1F;

				if (m_RomBank == 0)
					m_RomBank = 1;

				return;
			}

			else if (address >= 0xA000 && address < 0xC000)
			{
				if (m_RamEnabled)
				{
					uint32_t ram_offset = address - 0xA000;
					ram[ram_offset] = value;
					return;
				}
			}

			DAT_LOG_WARN("Invalid write to cartridge. Check the MMU intervals.");
		}

	private:
		u8 m_RomBank = 0x0;
		u8 m_RamBank = 0x0;
		bool m_RamEnabled = false;
	};

	dat_shared<ICartridge> read_cartrige(const std::filesystem::path& filepath);
}