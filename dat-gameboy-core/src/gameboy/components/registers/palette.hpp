#pragma once
#include "hardware_register.hpp"

namespace dat
{
	constexpr inline u16 BGP_Address = 0xFF47;
	constexpr inline u16 OBP0_Address = 0xFF48;
	constexpr inline u16 OBP1_Address = 0xFF49;

	enum class e_BGPColor : u8
	{
		COLOR0		= 0x0,
		COLOR1		= 0x1,
		COLOR2		= 0x2,
		COLOR3		= 0x3,
	};

	class s_PaletteRegister : public s_HardwareRegister<DataRegister>
	{
	public:
		u8 get_color(e_BGPColor color)
		{
			switch (color)
			{
			case e_BGPColor::COLOR0:
				return *data & 0x3;

			case e_BGPColor::COLOR1:
				return (*data >> 2) & 0x3;

			case e_BGPColor::COLOR2:
				return (*data >> 4) & 0x3;

			case e_BGPColor::COLOR3:
				return (*data >> 6) & 0x3;
			}

			DAT_LOG_CRITICAL("Invalid BGP color: {}", (int)color);
			return 0x0;
		}
	};

	using s_BGP  = s_PaletteRegister;
	using s_OBP0 = s_PaletteRegister;
	using s_OBP1 = s_PaletteRegister;
}