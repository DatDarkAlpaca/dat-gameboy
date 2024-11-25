#pragma once
#include <any>
#include <GLFW/glfw3.h>

#include "core/event/event.hpp"

#include "common.hpp"
#include "window_properties.hpp"

namespace dat
{
	class GLFWWindowDestroyer
	{
	public:
		void operator() (GLFWwindow* window);
	};
}

namespace dat
{
	class Window
	{
	public:
		void initialize(const WindowProperties& properties);

		void shutdown();

	public:
		void set_event_callback(const EventCallback& callback);

		void set_properties(const WindowProperties& properties);

		void poll_events();

	public:
		void hide();

		void show();

		bool is_open() const;

	public:
		inline WindowProperties get_properties() const { return m_Properties; }

		inline GLFWwindow* get_handle() const { return m_Window.get(); };

	private:
		void set_window_data_pointer();

		void set_window_callbacks();

	private:
		dat_unique<GLFWwindow, GLFWWindowDestroyer> m_Window;
		WindowProperties m_Properties;

		struct WindowData
		{
			EventCallback eventCallback;
		} m_WindowData;
	};
}