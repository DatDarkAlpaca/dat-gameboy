#pragma once
#include "common.hpp"
#include "gameboy/components/registers/hardware_register.hpp"

namespace dat
{
	enum class e_SpriteFlags
	{
		PRIORITY		= 7,
		Y_FLIP			= 6,
		X_FLIP			= 5,
		DMG_PALETTE		= 4,
		BANK			= 3,
		CGB_PALETTE_2	= 2,
		CGB_PALETTE_1	= 1,
		CGB_PALETTE_0	= 0
	};

	using s_SpriteAttributeFlags = s_HardwareRegister<e_SpriteFlags>;

	class s_Sprite
	{
	public:
		s_Sprite()
		{
			flags.initialize(&rawFlags);
		}

	public:
		u8 read(u16 address) const
		{
			switch (address & 0b11) 
			{
				case 0: return y;
				case 1: return x;
				case 2: return tileID;
				case 3: return rawFlags;
				default:
					DAT_LOG_CRITICAL("Invalid sprite read operation at: {}", address);
			}

			return 0;
		}

		void write(u16 address, u8 value)
		{
			switch (address & 0b11) 
			{
				case 0:
					y = value;
					break;

				case 1:
					x = value;
					break;

				case 2:
					tileID = value;
					break;

				case 3:
					rawFlags = value;
					break;

				default:
					DAT_LOG_CRITICAL("Invalid sprite write operation at: {} (value: {})", address, value);
					break;
			}
		}

	public:
		inline u8 get_x() const { return x - 8; }

		inline u8 get_y() const { return y - 16; }

		inline u8 get_tile_ID() const { return tileID; }

		inline u8 get_vram_bank() const { return static_cast<u8>(flags.is_bit_set(e_SpriteFlags::BANK)); }

		inline u8 get_cgb_palette() const { return rawFlags & 7; }

		inline bool is_priority() const { return flags.is_bit_set(e_SpriteFlags::PRIORITY); }

		inline bool is_x_flipped() const { return flags.is_bit_set(e_SpriteFlags::X_FLIP); }

		inline bool is_y_flipped() const { return flags.is_bit_set(e_SpriteFlags::Y_FLIP); }

		inline bool is_dmg_palette() const { return flags.is_bit_set(e_SpriteFlags::DMG_PALETTE); }

	private:
		u8 y = 0;
		u8 x = 0;
		u8 tileID = 0;
		u8 rawFlags = 0;
		s_SpriteAttributeFlags flags;
	};
}