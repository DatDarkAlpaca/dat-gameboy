#pragma once
#include <gameboy/components/cartridge.hpp>
#include <utils/file.hpp>

namespace dat 
{
	inline s_Cartridge load_rom()
	{
		auto filepath = open_file_dialog("Select Game Boy ROM", "Game Boy ROMs (*.gb;*.gbc)", { "*.gb" });
		return read_cartrige(filepath);
	}
}