#pragma once
#include "core/logger/logger.hpp"

#include <GLFW/glfw3.h>

namespace
{
	inline void GLFWErrorCallback(int errorCode, const char* description)
	{
		DAT_LOG_ERROR("[GLFW]: {} [{}]", description, errorCode);
	}
}

namespace dat
{
	inline void initialize_platform()
	{
		glfwSetErrorCallback(GLFWErrorCallback);

		if (!glfwInit())
			DAT_LOG_CRITICAL("Failed to initialize the platform system.");
	}
}