#pragma once
#include "hardware_register.hpp"

namespace dat
{
	constexpr inline u16 BANK_Address = 0xFF50;

	class s_BANK : public s_HardwareRegister<DataRegister>
	{
	public:
		bool is_bootloader_enabled() const
		{
			return *data != 0x1;
		}
	};
}