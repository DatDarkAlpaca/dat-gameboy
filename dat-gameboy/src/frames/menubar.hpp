#pragma once
#include <imgui.h>
#include "frames/frame.hpp"
#include "events/frame_events.hpp"
#include "actions/rom_actions.hpp"

namespace dat
{
	class s_Menubar : public IFrame
	{
	public:
		void register_frame(std::string_view name, const dat_shared<IFrame>& frame);

		void register_frame_separator();

	public:
		void render();

	private:
		void render_file();

		void render_view();

		void render_settings();

		void render_debug();

	private:
		void load_rom_action();

		void quit_action();

	private:
		std::unordered_map<u64, std::string_view> m_NameRegister;
		std::unordered_map<u64, bool*> m_SelectTable;
		std::vector<dat_shared<IFrame>> m_Frames;
	};
}
