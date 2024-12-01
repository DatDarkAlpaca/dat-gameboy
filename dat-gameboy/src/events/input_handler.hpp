#pragma once
#include "frame_events.hpp"
#include <gameboy/components/memory.hpp>

namespace dat
{
	class s_InputHandler
	{
	public:
		void initialize(NON_OWNING s_MMU* memory)
		{
			r_Memory = memory;
		}

	public:
		void on_event(Event& event)
		{
			// TODO: key mapper.
			// Requires: settings

			Subscriber subscriber(event);
			subscriber.subscribe<KeyPressedEvent>([&](const KeyPressedEvent& event) -> bool {

				switch (event.key)
				{
					case Key::KEY_UP:
						r_Memory->joypad().set_button(e_Buttons::UP, true);
						break;
				}

				return false;
			});

			subscriber.subscribe<KeyReleasedEvent>([&](const KeyReleasedEvent& event) -> bool {

				switch (event.key)
				{
					case Key::KEY_UP:
						r_Memory->joypad().set_button(e_Buttons::UP, false);
						break;
				}

				return false;
			});
		}

	private:
		NON_OWNING s_MMU* r_Memory = nullptr;
	};
}