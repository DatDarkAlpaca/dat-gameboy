#pragma once
#include <chrono>
#include "common.hpp"

namespace dat
{
	class s_Clock
	{
	public:
		void start()
		{
			m_StartTime = std::chrono::steady_clock::now();
		}

		void end()
		{
			m_EndTime = std::chrono::steady_clock::now();
		}

		long long delta() const
		{
			using namespace std::chrono;
			return abs(duration_cast<nanoseconds>(m_EndTime - m_StartTime).count()) + 1;
		}

		void cycle()
		{
			using namespace std::chrono;

			m_EndTime = steady_clock::now();
			duration += abs(duration_cast<nanoseconds>(m_EndTime - m_StartTime).count()) + 1;
		}

		void restart()
		{
			duration = 0;
		}

	public:
		long long duration = 0;

	private:
		std::chrono::steady_clock::time_point m_StartTime, m_EndTime;
	};
}