#pragma once
#include "hardware_register.hpp"

namespace dat
{
	constexpr inline u16 JOYP_Address = 0xFF00;

	enum class e_JOYP : u8
	{
		PAD0				= 0x7,
		PAD1				= 0x6,
		SELECT_BUTTONS		= 0x5,
		SELECT_DPAD			= 0x4,
		START_DOWN			= 0x3,
		SELECT_UP			= 0x2,
		B_LEFT				= 0x1,
		A_RIGHT				= 0x0
	};

	enum class e_Buttons : u8
	{
		SELECT = 0x0,
		START,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		A,
		B,
	};

	class s_JOYP : public s_HardwareRegister<e_JOYP>
	{
	public:
		void set_button(e_Buttons button, bool value)
		{
			bool isActionButton =
				button == e_Buttons::SELECT || 
				button == e_Buttons::START ||
				button == e_Buttons::A || 
				button == e_Buttons::B;

			set(e_JOYP::SELECT_BUTTONS, isActionButton ? !value : value);
			set(e_JOYP::SELECT_DPAD, isActionButton ? value : !value);

			switch (button)
			{
				case e_Buttons::SELECT:
				case e_Buttons::UP:
					set(e_JOYP::SELECT_UP, !value);
					break;

				case e_Buttons::START:
				case e_Buttons::DOWN:
					set(e_JOYP::START_DOWN, !value);
					break;

				case e_Buttons::B:
				case e_Buttons::LEFT:
					set(e_JOYP::B_LEFT, !value);
					break;

				case e_Buttons::A:
				case e_Buttons::RIGHT:
					set(e_JOYP::A_RIGHT, !value);
					break;	

				default:
					DAT_LOG_CRITICAL("Invalid joypad button: {}", (int)button);
					break;
			}
		}
	};
}