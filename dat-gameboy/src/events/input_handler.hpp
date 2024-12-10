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

					case Key::KEY_DOWN:
						r_Memory->joypad().set_button(e_Buttons::DOWN, true);
						break;

					case Key::KEY_LEFT:
						r_Memory->joypad().set_button(e_Buttons::LEFT, true);
						break;

					case Key::KEY_RIGHT:
						r_Memory->joypad().set_button(e_Buttons::RIGHT, true);
						break;

					case Key::KEY_A:
						r_Memory->joypad().set_button(e_Buttons::A, true);
						break;

					case Key::KEY_B:
						r_Memory->joypad().set_button(e_Buttons::B, true);
						break;

					case Key::KEY_SPACE:
						r_Memory->joypad().set_button(e_Buttons::SELECT, true);
						break;

					case Key::KEY_BACKSPACE:
						r_Memory->joypad().set_button(e_Buttons::START, true);
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

					case Key::KEY_DOWN:
						r_Memory->joypad().set_button(e_Buttons::DOWN, false);
						break;

					case Key::KEY_LEFT:
						r_Memory->joypad().set_button(e_Buttons::LEFT, false);
						break;

					case Key::KEY_RIGHT:
						r_Memory->joypad().set_button(e_Buttons::RIGHT, false);
						break;

					case Key::KEY_A:
						r_Memory->joypad().set_button(e_Buttons::A, false);
						break;

					case Key::KEY_B:
						r_Memory->joypad().set_button(e_Buttons::B, false);
						break;

					case Key::KEY_SPACE:
						r_Memory->joypad().set_button(e_Buttons::SELECT, false);
						break;

					case Key::KEY_BACKSPACE:
						r_Memory->joypad().set_button(e_Buttons::START, false);
						break;
				}

				return false;
			});
		}

	private:
		NON_OWNING s_MMU* r_Memory = nullptr;
	};
}