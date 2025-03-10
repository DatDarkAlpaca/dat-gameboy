#pragma once
#include "core/event/event.hpp"

namespace dat
{
	struct WindowResizeEvent : public Event
	{
		DEFINE_EVENT(EventType::WINDOW_RESIZE);

	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: width(width)
			, height(height)
		{

		}

	public:
		uint32_t width = 0, height = 0;
	};
}