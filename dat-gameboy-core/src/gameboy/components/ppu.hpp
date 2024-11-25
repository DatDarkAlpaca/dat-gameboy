#pragma once
#include <glad/glad.h>
#include "platform/platform.hpp"

#include "gameboy/constants.hpp"
#include "graphics/quad_renderer.hpp"
#include "gameboy/components/memory.hpp"

namespace dat
{
	class s_PPU
	{
	public:
		void initialize(s_Memory* memory)
		{
			r_Memory = memory;
		}

	public:
		void tick()
		{

		}

	public:
		u8* get_tiledata() const
		{
			return &r_Memory->memory[0x8000];
		}

		u8* get_tilemap()
		{
			bool backgroundMapArea = r_Memory->lcdc().backgroundMapArea;			
			return backgroundMapArea ? &r_Memory->memory[0x9c00] : &r_Memory->memory[0x9800];
		}

		u8* get_tiledata()
		{
			bool backgroundWindowTileMapArea = r_Memory->lcdc().backgroundWindowTileMapArea;
			return backgroundWindowTileMapArea ? &r_Memory->memory[0x9c00] : &r_Memory->memory[0x9800];
		}
		
	private:
		s_Memory* r_Memory = nullptr;
	};
}