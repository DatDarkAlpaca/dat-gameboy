#pragma once
#include <functional>

#include "event.hpp"

namespace dat
{
	class Subscriber
	{
	public:
		Subscriber(Event& event)
			: event(event)
		{

		}

	public:
		template<typename ImplementedEvent>
		void subscribe(std::function<bool(ImplementedEvent)> callback)
		{
			if (event.handled)
				return;

			if (event.type() == (EventType)ImplementedEvent::GetEventType())
			{
				ImplementedEvent* implementedEvent = static_cast<ImplementedEvent*>(&event);
				implementedEvent->handled = implementedEvent->handled || callback(*implementedEvent);
			}
		}

	private:
		Event& event;
	};
}