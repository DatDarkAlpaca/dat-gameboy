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
		template<typename Implementedevent>
		void subscribe(std::function<bool(Implementedevent)> callback)
		{
			if (event.handled)
				return;

			if (event.type()  == (EventType)Implementedevent::GeteventType())
			{
				Implementedevent* implementedevent = static_cast<Implementedevent*>(&event);
				implementedevent->handled = implementedevent->handled || callback(*implementedevent);
			}
		}

	private:
		Event& event;
	};
}