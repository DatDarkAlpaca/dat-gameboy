#pragma once
#include <tinyfiledialogs.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "core/logger/logger.hpp"

namespace dat
{
	inline std::string read_file(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath);
		if (!file.is_open())
			DAT_LOG_ERROR("Failed to load file: {}", filepath.string());

		std::stringstream ss;

		ss << file.rdbuf();

		return ss.str();
	}

	inline std::filesystem::path open_file_dialog(const std::string& title, const std::string& description, const std::vector<const char*>& filters)
	{
		char const* selectedFilepath = tinyfd_openFileDialog(
			title.c_str(), nullptr,
			static_cast<int>(filters.size()), filters.data(),
			description.c_str(), 0
		);

		if (!selectedFilepath)
			return std::filesystem::path();

		return std::filesystem::path(selectedFilepath);
	}
}