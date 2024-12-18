#pragma once
#include "core/event/event.hpp"

namespace dat
{
	struct WindowMovedEvent : public Event
	{
		DEFINE_EVENT(EventType::WINDOW_MOVED);

	public:
		WindowMovedEvent(uint32_t x, uint32_t y)
			: x(x)
			, y(y)
		{

		}

	public:
		uint32_t x = 0, y = 0;
	};
}