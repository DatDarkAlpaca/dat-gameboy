#pragma once
#include "Common.hpp"

#if defined(DAT_PLATFORM_WINDOWS) || defined(DAT_PLATFORM_LINUX) || defined(DAT_PLATFORM_MACOSX)
	#include "platform/glfw/setup_glfw.hpp"
	#include "platform/glfw/events_glfw.hpp"
	#include "platform/glfw/window/window_glfw.hpp"
	#include "platform/glfw/window/monitor_glfw.hpp"

	#include "platform/glfw/opengl/setup_opengl.hpp"
#endif