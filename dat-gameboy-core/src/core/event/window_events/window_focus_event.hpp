#pragma once
#include "core/event/event.hpp"

namespace dat
{
	class WindowFocusEvent : public Event
	{
		DEFINE_EVENT(EventType::WINDOW_FOCUS_CHANGED);

	public:
		WindowFocusEvent(int focusState)
			: focusState(focusState)
		{

		}

	public:
		int focusState;
	};
}