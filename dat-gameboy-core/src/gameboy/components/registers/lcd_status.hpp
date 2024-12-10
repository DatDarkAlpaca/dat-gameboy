#pragma once
#include "hardware_register.hpp"

namespace dat
{
	constexpr inline u16 STAT_Address = 0xFF41;

	enum e_VideoMode : u8
	{
		HBLANK = 0x0,
		VBLANK = 0x1,
		ACCESS_OAM = 0x2,
		ACCESS_VRAM = 0x3,
	};

	enum class e_STAT : u8
	{
		PAD0				= 0x7,
		LYC_INT_SELECT		= 0x6,
		MODE_2_INT_SELECT	= 0x5,
		MODE_1_INT_SELECT	= 0x4,
		MODE_0_INT_SELECT	= 0x3,
		LYC_EQ_LC			= 0x2,
		PPU_MODE1			= 0x1,
		PPU_MODE0			= 0x0
	};

	class s_STAT : public s_HardwareRegister<e_STAT>
	{
	public:
		e_VideoMode get_video_mode() const
		{
			u8 mode0 = static_cast<u8>(is_bit_set(e_STAT::PPU_MODE0));
			u8 mode1 = static_cast<u8>(is_bit_set(e_STAT::PPU_MODE1));

			return static_cast<e_VideoMode>((mode1 << 1) | mode0);
		}

		void set_video_mode(e_VideoMode mode)
		{
			switch (mode)
			{
				case e_VideoMode::HBLANK:
				{
					set(e_STAT::PPU_MODE0, 0);
					set(e_STAT::PPU_MODE1, 0);
				} break;

				case e_VideoMode::VBLANK:
				{
					set(e_STAT::PPU_MODE0, 1);
					set(e_STAT::PPU_MODE1, 0);
				} break;

				case e_VideoMode::ACCESS_OAM:
				{
					set(e_STAT::PPU_MODE0, 0);
					set(e_STAT::PPU_MODE1, 1);
				} break;

				case e_VideoMode::ACCESS_VRAM:
				{
					set(e_STAT::PPU_MODE0, 1);
					set(e_STAT::PPU_MODE1, 1);
				} break;

				default:
					DAT_LOG_CRITICAL("Invalid Video mode: {}", (int)mode);
			}
		}
	};
}