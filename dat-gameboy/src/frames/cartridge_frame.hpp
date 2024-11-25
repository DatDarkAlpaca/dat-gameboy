#pragma once
#include "image_frame.hpp"
#include <gameboy/gameboy.hpp>
#include "events/frame_events.hpp"

namespace dat
{
	class s_CartridgeFrameHolder;

	class s_CartridgeFrame : public s_ImageFrame
	{
	public:
		s_CartridgeFrame(const std::string& name, cartridge_handle handle);

	public:
		void render();

	private:
		void render_tooltip();

		void render_popup();

		void detect_focus();

	public:
		inline std::string get_name() const { return m_Name; }

	private:
		cartridge_handle m_Handle;
		std::string m_Name;
	};
}