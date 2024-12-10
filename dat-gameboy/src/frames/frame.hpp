#pragma once
#include <core/event/events.hpp>

namespace dat
{
	class IFrame
	{
	public:
		virtual ~IFrame() = default;

	public:
		void set_event_callback(const EventCallback& callback) 
		{
			this->callback = callback;
		}

	public:		
		bool isOpen = true;

	protected:
		EventCallback callback;
	};
}