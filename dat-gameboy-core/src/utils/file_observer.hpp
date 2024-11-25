#pragma once
#include <unordered_map>
#include <functional>
#include <filesystem>
#include <string>
#include <thread>
#include <chrono>

namespace dat
{
	enum class e_FileStatus
	{
		MODIFIED,
		ERASED
	};

	class s_FileObserver
	{
	public:
		s_FileObserver(const std::filesystem::path& filepath, std::chrono::duration<int, std::milli> delay)
			: m_Filepath(filepath)
			, m_Delay(delay)
		{
			m_LastWriteTime = std::filesystem::last_write_time(filepath);
		}

	public:
		void start(const std::function<void(const std::filesystem::path&, e_FileStatus)>& action)
		{
			while (true)
			{
				std::this_thread::sleep_for(m_Delay);

				if (!std::filesystem::exists(m_Filepath))
					action(m_Filepath, e_FileStatus::ERASED);

				if (m_LastWriteTime != std::filesystem::last_write_time(m_Filepath))
					action(m_Filepath, e_FileStatus::MODIFIED);
			}
		}

	private:
		std::filesystem::path m_Filepath;
		std::filesystem::file_time_type m_LastWriteTime;
		std::chrono::duration<int, std::milli> m_Delay;
	};
}
