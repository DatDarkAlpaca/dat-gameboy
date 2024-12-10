#include "pch.hpp"
#include "cartridge.hpp"

namespace dat
{
	static dat_shared<ICartridge> create_cartridge(dat::e_MBCType type, const std::filesystem::path& filepath, const s_CartridgeHeader& header, const std::vector<u8>& rom)
	{
		switch (type)
		{
			case e_MBCType::NO_MBC:
				return dat::make_shared<s_NoMBC>(filepath, rom, header);

			case e_MBCType::MBC1:
				return dat::make_shared<s_MBC1>(filepath, rom, header);

			case e_MBCType::MBC2:
				DAT_LOG_CRITICAL("MBC type not implemented");

			case e_MBCType::MBC3:
				DAT_LOG_CRITICAL("MBC type not implemented");

			default:
				DAT_LOG_CRITICAL("MBC type not implemented");
		}

		return nullptr;
	}

	dat_shared<ICartridge> read_cartrige(const std::filesystem::path& filepath)
	{
		if (!std::filesystem::exists(filepath))
		{
			DAT_LOG_ERROR("The ROM {} does not exist.", filepath.string());
			return {};
		}

		std::ifstream file(filepath, std::ios::binary | std::ios::ate);

		if (!file)
		{
			DAT_LOG_ERROR("Failed to load {}", filepath.string());
			return nullptr;
		}

		std::streamsize fileSize = file.tellg();
		if (fileSize <= sizeof(s_CartridgeHeader))
		{
			DAT_LOG_ERROR("The file '{}' contains an invalid ROM. [Size too small]", filepath.string());
			return {};
		}

		s_CartridgeHeader header = {};
		{
			file.seekg(0x100, std::ios::beg);
			file.read(reinterpret_cast<char*>(&header), sizeof(s_CartridgeHeader));
			file.clear();
		}

		e_MBCType type = get_cartridge_type(header);
		
		u32 romSize = get_rom_size(header);
		u32 ramSize = get_ram_size(header);

		std::vector<u8> rom(romSize, 0);
		{
			file.seekg(0x0);
			file.read(reinterpret_cast<char*>(rom.data()), romSize);
		}
		
		file.close();

		return create_cartridge(type, filepath, header, rom);
	}
}