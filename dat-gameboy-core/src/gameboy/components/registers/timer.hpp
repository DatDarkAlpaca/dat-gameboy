#pragma once
#include "gameboy/constants.hpp"
#include "hardware_register.hpp"
#include "interrupt.hpp"

namespace dat
{
	constexpr inline u16 DIV_Address = 0xFF04;
	constexpr inline u16 TIMA_Address = 0xFF05;
	constexpr inline u16 TMA_Address = 0xFF06;
	constexpr inline u16 TAC_Address = 0xFF07;
	
	using s_DIV = s_HardwareRegister<DataRegister>;
	using s_TIMA = s_HardwareRegister<DataRegister>;
	using s_TMA = s_HardwareRegister<DataRegister>;
	
	enum class e_TAC
	{
		PAD0			= 0x7,
		PAD1			= 0x6,
		PAD2			= 0x5,
		PAD3			= 0x4,
		PAD4			= 0x3,
		ENABLE			= 0x2,
		CLOCK_SELECT1	= 0x1,
		CLOCK_SELECT0	= 0x0,
	};

	class s_TAC : public s_HardwareRegister<e_TAC>
	{
	public:
		u8 get_clock_select() const
		{
			return get() & 0x3;
		}
	};	
}