#pragma once
#include "graphics/graphics_specification.hpp"
#include "platform/glfw/window/window_glfw.hpp"

namespace dat
{
	void setup_opengl_windowing(Window& window, const WindowProperties&, const GraphicsSpecification&);

	void present(Window& window);
}