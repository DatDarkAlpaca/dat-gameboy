#pragma once
#include "mouse_event.hpp"

namespace dat
{
	struct MouseReleasedEvent : public Event
	{
		DEFINE_EVENT(EventType::MOUSE_RELEASED);

	public:
		MouseReleasedEvent(MouseButton button)
			: button(button)
		{

		}

	public:
		MouseButton button;
	};
}