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
	inline std::vector<u8> read_ROM(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath, std::ios::binary);
		file.unsetf(std::ios::skipws);

		file.seekg(0, file.end);
		u64 length = file.tellg();
		file.seekg(0, file.beg);

		std::vector<u8> data;
		data.reserve(length);

		data.insert(data.begin(),
			std::istream_iterator<u8>(file),
			std::istream_iterator<u8>());

		return data;
	}

	inline nlohmann::json load_json_file(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath);
		nlohmann::json jsonObject;
		file >> jsonObject;

		return jsonObject;
	}
}