#pragma once
#include "core/event/event.hpp"

namespace dat
{
	class WindowCloseEvent : public Event
	{
		DEFINE_EVENT(EventType::WINDOW_CLOSE);
	};
}