#pragma once
#include "common.hpp"

namespace dat
{
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

	struct s_LCDC
	{
	public:
		s_LCDC(u8 value)
		{
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
		u8 enableLCD : 1;							/* 7 */
		u8 windowTilemapArea : 1;					/* 6 */
		u8 enableWindow : 1;						/* 5 */
		u8 backgroundWindowTileMapArea : 1;			/* 4 */
		u8 backgroundMapArea : 1;					/* 3 */
		u8 objSize : 1;								/* 2 */
		u8 enableObj : 1;							/* 1 */
		u8 enablebackgroundWindow : 1;				/* 0 */
	};
}