#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

#include "common.hpp"
#include "gameboy/constants.hpp"

namespace dat
{
	inline void load_ROM_file(const std::filesystem::path& filepath, u8* rom)
	{
		std::ifstream file(filepath, std::ios::binary);
		file.read((char*)rom, ROMSize);
	}

	inline nlohmann::json load_opcodes_json(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath);
		nlohmann::json jsonObject;
		file >> jsonObject;

		return jsonObject;
	}
}