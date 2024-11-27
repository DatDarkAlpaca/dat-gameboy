#pragma once
#include "common.hpp"
#include "gameboy/flags/video_mode.hpp"

namespace dat
{
	// TODO: rename to s_CPUFlags
	struct s_Flags
	{
	public:
		u8 get_register() const
		{
			return (Z << 7) | (N << 6) | (H << 5) | (C << 4);
		}

		void set(u8 value)
		{
			Z = (value >> 7) & 1;
			N = (value >> 6) & 1;
			H = (value >> 5) & 1;
			C = (value >> 4) & 1;
		}

	public:
		u8 Z : 1;
		u8 N : 1;
		u8 H : 1;
		u8 C : 1;
	};

	struct s_InterruptEnable
	{
	public:
		s_InterruptEnable(u8 value)
			: m_Register(value)
		{
			joypad	 = (value >> 4) & 1;
			serial	 = (value >> 3) & 1;
			timer	 = (value >> 2) & 1;
			lcd		 = (value >> 1) & 1;
			vblank	 = (value >> 0) & 1;
		}

	public:
		u8 get_value() const
		{
			return
				(__unused << 5) |
				(joypad << 4)   |
				(serial << 3)   |
				(timer << 2)    |
				(lcd << 1)      |
				vblank;
		}

		void write(u8 bit, u8 value)
		{
			if      (bit == 0) vblank = value;
			else if (bit == 1) lcd = value;
			else if (bit == 2) timer = value;
			else if (bit == 3) serial = value;
			else if (bit == 4) joypad = value;
		}

		void commit()
		{
			m_Register = get_value();
		}

	public:
		u8 __unused : 3; // 5-7
		u8 joypad	: 1; // 4
		u8 serial	: 1; // 3
		u8 timer	: 1; // 2
		u8 lcd		: 1; // 1
		u8 vblank	: 1; // 0

	private:
		u8& m_Register;
	};

	struct s_LCDC
	{
	public:
		s_LCDC(u8 value)
			: m_Register(value)
		{
			// TODO: disable LCD outside VBlank causes a black burn-in horizontal line.

			enableLCD					= (value >> 7) & 1;
			windowTilemapArea			= (value >> 6) & 1;
			enableWindow				= (value >> 5) & 1;
			backgroundWindowTileMapArea = (value >> 4) & 1;
			backgroundMapArea			= (value >> 3) & 1;
			objSize						= (value >> 2) & 1;
			enableObj					= (value >> 1) & 1;
			enablebackgroundWindow		= (value >> 0) & 1;
		}

	public:
		u8 get_value() const
		{
			return 
				(enableLCD << 7)						| 
				(windowTilemapArea << 6)				|
				(enableWindow << 5)						|
				(backgroundWindowTileMapArea << 4)		|
				(backgroundMapArea << 3)				|
				(objSize << 2)							|
				(enableObj << 1)						|
				enablebackgroundWindow;
		}

	public:
		u8 enableLCD : 1;							// 7
		u8 windowTilemapArea : 1;					// 6
		u8 enableWindow : 1;						// 5
		u8 backgroundWindowTileMapArea : 1;			// 4
		u8 backgroundMapArea : 1;					// 3
		u8 objSize : 1;								// 2
		u8 enableObj : 1;							// 1
		u8 enablebackgroundWindow : 1;				// 0

	private:
		u8& m_Register;
	};

	struct s_STAT
	{
	public:
		s_STAT(u8& value)
			: m_Register(value)
		{
			__unused		= (value >> 7) & 1;
			lycintSelect	= (value >> 6) & 1;
			mode2intSelect	= (value >> 5) & 1;
			mode1intSelect	= (value >> 4) & 1;
			mode0intSelect	= (value >> 3) & 1;
			LYC_EQ_LY		= (value >> 2) & 1;
			ppuMode 		= (value >> 0) & 2;
		}

	public:
		void commit()
		{
			m_Register = get_value();
		}

		u8 get_value() const
		{
			return
				(__unused << 7)			|
				(lycintSelect << 6)		|
				(mode2intSelect << 5)	|
				(mode1intSelect << 4)	|
				(mode0intSelect << 3)	|
				(LYC_EQ_LY << 2)		|
				(ppuMode << 0);
		}

	public:
		void set_ppu_mode(e_VideoMode mode) 
		{ 
			ppuMode = static_cast<u8>(mode);
			commit();
		}

	public:
		u8 __unused				: 1; // 7
		u8 lycintSelect			: 1; // 6
		u8 mode2intSelect		: 1; // 5
		u8 mode1intSelect		: 1; // 4
		u8 mode0intSelect		: 1; // 3
		u8 LYC_EQ_LY			: 1; // 2
		u8 ppuMode				: 2; // 0-1

	private:
		u8& m_Register;
	};

	struct s_BGP
	{
	public:
		s_BGP(u8& value)
			: m_Register(value)
		{
			colorID3 = (value >> 6) & 2;
			colorID2 = (value >> 4) & 2;
			colorID1 = (value >> 2) & 2;
			colorID0 = (value >> 0) & 2;
		}

	public:
		void commit()
		{
			m_Register = get_value();
		}

		u8 get_value() const
		{
			return
				(colorID3 << 6) |
				(colorID2 << 4) |
				(colorID1 << 2) |
				(colorID0 << 0);
		}

	public:
		u8 colorID3 : 2;	// ID 3 : 6-7
		u8 colorID2 : 2;	// ID 2 : 4-5
		u8 colorID1 : 2;	// ID 1 : 2-3
		u8 colorID0 : 2; // ID 0 : 0-1

	private:
		u8& m_Register;
	};

	struct s_SpriteAttributeFlags
	{
	public:
		s_SpriteAttributeFlags(u8& value)
			: m_Register(value)
		{
			priority	= (value >> 7) & 1;
			yFlip		= (value >> 6) & 1;
			xFlip		= (value >> 5) & 1;
			dmgPalette  = (value >> 4) & 1;
			bank		= (value >> 3) & 1;
			cgbPalette  = (value >> 0) & 7;
		}

	public:
		void commit()
		{
			m_Register = get_value();
		}

		u8 get_value() const
		{
			return
				(priority << 7) |
				(yFlip << 6) |
				(xFlip << 5) |
				(dmgPalette << 4) |
				(bank << 3) |
				(cgbPalette << 0);
		}

	public:
		u8 priority : 1; // 7
		u8 yFlip : 1; // 6
		u8 xFlip : 1; // 5
		u8 dmgPalette : 1; // 4
		u8 bank : 1; // 3
		u8 cgbPalette : 3; // 0 - 2

	private:
		u8& m_Register;
	};
}