#pragma once
#include "image_frame.hpp"
#include <gameboy/gameboy.hpp>
#include "events/frame_events.hpp"

namespace dat
{
	class s_GameboyFrame : public s_ImageFrame
	{
	public:
		s_GameboyFrame(s_Gameboy& gameboy)
			: gameboy(gameboy)
		{

		}

	public:
		void render()
		{
			render_frame("Gameboy");
		}

	private:
		s_Gameboy& gameboy;
	};
}