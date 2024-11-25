#pragma once
#include "common.hpp"
#include "utils/byte_utils.hpp"

namespace dat
{
	enum class e_Button
	{
		START	= 0x0,
		SELECT	= 0x1,
		B		= 0x2,
		A		= 0x3,

		UP		= 0x4,
		LEFT	= 0x5,
		RIGHT	= 0x6,
		DOWN	= 0x7
	};

	constexpr inline u8 get_button_bit_position(e_Button button)
	{
		if (button == e_Button::A || button == e_Button::RIGHT)
			return 0;

		else if (button == e_Button::B || button == e_Button::LEFT)
			return 1;

		else if (button == e_Button::SELECT || button == e_Button::UP)
			return 2;

		else if (button == e_Button::START || button == e_Button::DOWN)
			return 3;
	}

	// P1/JOYP
	struct s_Joypad
	{
	public:
		s_Joypad(u8& value)
			: r_RegisterValue(value)
		{
			A_Right = (value >> 0) & 1;
			B_Left = (value >> 1) & 1;
			Select_Up = (value >> 2) & 1;
			Start_Down = (value >> 3) & 1;
			select_DPad = (value >> 4) & 1;
			selectButtons = (value >> 5) & 1;
		}

		s_Joypad() = default;

	public:
		void set_value(e_Button button, bool value)
		{
			u8 numericBtn = static_cast<u8>(button);
			
			DAT_ASSERT(numericBtn >= 0 && numericBtn <= 7, "Invalid button");
			
			if (numericBtn >= 0 && numericBtn <= 3)
				set_button(button, value);

			else
				set_dpad(button, value);
		}

	private:
		void set_button(e_Button button, bool value)
		{
			set_bit(r_RegisterValue, 5);
			reset_bit(r_RegisterValue, 4);

			set_bit_value(r_RegisterValue, get_button_bit_position(button), value);
		}

		void set_dpad(e_Button button, bool value)
		{
			reset_bit(r_RegisterValue, 5);
			set_bit(r_RegisterValue, 4);

			set_bit_value(r_RegisterValue, get_button_bit_position(button), value);
		}

	public:
		u8 __padding : 2;
		u8 selectButtons : 1;
		u8 select_DPad : 1;
		u8 Start_Down : 1;
		u8 Select_Up : 1;
		u8 B_Left : 1;
		u8 A_Right : 1;

	private:
		u8& r_RegisterValue;
	};
}