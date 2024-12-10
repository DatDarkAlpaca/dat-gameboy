#pragma once
#include "common.hpp"

namespace dat
{
	constexpr static inline u64 SM83Period_ns = 238;	// 4.194304 MHz
	constexpr static inline u8 TCyclesPerMCycle = 4;
	constexpr static inline u32 CyclesPerFrame = 70224;

	constexpr static inline u32 ClockRate = 4194304;

	constexpr static inline u32 GameboyScreenWidth = 160;
	constexpr static inline u32 GameboyScreenHeight = 144;

	constexpr static inline u32 GameboySpriteWidth						= 128;
	constexpr static inline u32 GameboySpriteHeight						= 204;
	constexpr static inline u32 GameboySpriteScreenWidth				= 66;
	constexpr static inline u32 GameboySpriteScreenHeight				= 60;
	constexpr static inline u32 GameboySpritePixelsUntilScreenX			= 38;
	constexpr static inline u32 GameboySpritePixelsUntilScreenY			= 38;
	constexpr static inline u32 GameboySpriteScalingFactor				= 4;

	constexpr static inline u32 SpriteAttrAmountOAM = 40;
	constexpr static inline u32 BackgroundMapTileSize = 256;
	constexpr static inline u32 TilePixelSize = 8;
	constexpr static inline u32 TilesPerLine = 32;
	constexpr static inline u32 BytesPerSprite = 4;
	constexpr static inline u32 BytesPerTile = 16;

	constexpr static inline u32 NintendoLogoSize = 48;
	constexpr static inline u32 NintendoLogo[NintendoLogoSize] = {
		0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
		0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
		0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E,
	};

	constexpr static inline u64 CyclesHBlank	= 204;			// Mode 0
	constexpr static inline u64 CyclesOAM		= 80;			// Mode 1
	constexpr static inline u64 CyclesVRAM		= 172;			// Mode 2
	constexpr static inline u64 CyclesVBlank	= 4560;			// Mode 3
}