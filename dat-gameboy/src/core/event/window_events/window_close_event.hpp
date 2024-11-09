#pragma once
#include "core/event/event.hpp"

namespace dat
{
	class WindowCloseevent : public Event
	{
		DEFINE_EVENT(EventType::WINDOW_CLOSE);
	};
}