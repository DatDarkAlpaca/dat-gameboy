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
			m_StartTime = std::chrono::high_resolution_clock::now();
		}

		void cycle()
		{
			using namespace std::chrono;

			m_EndTime = high_resolution_clock::now();
			duration += abs(duration_cast<nanoseconds>(m_EndTime - m_StartTime).count()) + 1;
		}

		void restart()
		{
			duration = 0;
		}

	public:
		long long duration = 0;

	private:
		std::chrono::high_resolution_clock::time_point m_StartTime, m_EndTime;
	};
}