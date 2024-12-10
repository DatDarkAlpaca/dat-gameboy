#pragma once
#include <gameboy/components/cartridge/cartridge.hpp>
#include <utils/file.hpp>

namespace dat 
{
	inline dat_shared<ICartridge> load_rom()
	{
		auto filepath = open_file_dialog("Select Game Boy ROM", "Game Boy ROMs (*.gb;*.gbc)", { "*.gb" });
		if (filepath.empty())
			return nullptr;

		return read_cartrige(filepath);
	}
}