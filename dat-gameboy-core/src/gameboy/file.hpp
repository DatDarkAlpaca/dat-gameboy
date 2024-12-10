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

		if (!std::filesystem::exists(filepath))
		{
			DAT_LOG_ERROR("File does not exist: {}", filepath.string().c_str());
			return {};
		}

		if (!file.is_open())
		{
			DAT_LOG_ERROR("Failed to read rom from file: {}", filepath.string().c_str());
			return {};
		}

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

		if (!std::filesystem::exists(filepath))
		{
			DAT_LOG_ERROR("Json file does not exist: {}", filepath.string().c_str());
			return {};
		}

		if (!file.is_open())
		{
			DAT_LOG_ERROR("Failed to json file: {}", filepath.string().c_str());
			return {};
		}

		nlohmann::json jsonObject;
		file >> jsonObject;

		return jsonObject;
	}
}