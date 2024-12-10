#pragma once
#include "common.hpp"
#include "monitor_data.hpp"

#include <GLFW/glfw3.h>

namespace dat
{
	class Monitor
	{
	public:
		static uint32_t get_monitor_amount()
		{
			int count = 0;
			GLFWmonitor** _ = glfwGetMonitors(&count);

			if (!count)
				DAT_LOG_WARN("Failed to fetch monitor data.");

			return static_cast<uint32_t>(count);
		}
	
		static inline MonitorData fetch_monitor_data(uint32_t index)
		{
			if (get_monitor_amount() <= index)
			{
				DAT_LOG_ERROR("Invalid monitor index selected: {}.", index);
				return { };
			}
			
			return parse_monitor_data(get_monitor_pointer(index));
		}

	private:
		static inline GLFWmonitor* get_monitor_pointer(uint32_t index)
		{
			if (get_monitor_amount() <= index)
			{
				DAT_LOG_ERROR("Invalid monitor index selected: {}.", index);
				return nullptr;
			}

			int count = 0;
			GLFWmonitor* glfwMonitor = glfwGetMonitors(&count)[index];
			return glfwMonitor;
		}

		static inline MonitorData parse_monitor_data(GLFWmonitor* glfwMonitor)
		{
			MonitorData monitorData;

			{
				int physicalWidth, physicalHeight;
				glfwGetMonitorPhysicalSize(glfwMonitor, &physicalWidth, &physicalHeight);
				monitorData.physicalWidth = physicalWidth;
				monitorData.physicalHeight = physicalHeight;
			}

			{
				float xScale, yScale;
				glfwGetMonitorContentScale(glfwMonitor, &xScale, &yScale);
				monitorData.contentScaleX = xScale;
				monitorData.contentScaleY = yScale;
			}

			{
				int xPos, yPos, width, height;
				glfwGetMonitorWorkarea(glfwMonitor, &xPos, &yPos, &width, &height);

				monitorData.workWidth = width;
				monitorData.workHeight = height;
				monitorData.workX = xPos;
				monitorData.workY = yPos;
			}

			{
				const char* name = glfwGetMonitorName(glfwMonitor);
				monitorData.monitorName = name;
			}

			return monitorData;
		}
	};
}