#include "pch.hpp"
#include "menubar.hpp"

namespace dat
{
	void s_Menubar::register_frame(std::string_view name, const dat_shared<IFrame>& frame)
	{
		m_NameRegister[m_Frames.size()] = name;
		m_SelectTable[m_Frames.size()] = &frame->isOpen;
		m_Frames.push_back(frame);
	}

	void s_Menubar::register_frame_separator()
	{
		m_Frames.push_back(nullptr);
	}

	void dat::s_Menubar::render()
	{
		ImGui::BeginMainMenuBar();

		render_file();

		render_view();

		render_settings();

		render_debug();

		ImGui::EndMainMenuBar();
	}

	void s_Menubar::render_file()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load ROM"))
				load_rom_action();

			ImGui::Separator();

			if (ImGui::MenuItem("Quit"))
				quit_action();
			

			ImGui::EndMenu();
		}
	}

	void s_Menubar::render_view()
	{
		if (ImGui::BeginMenu("View"))
		{
			for (u32 i = 0; i < m_Frames.size(); ++i)
			{
				if (!m_Frames[i])
					ImGui::Separator();
				else
					if (ImGui::MenuItem(m_NameRegister[i].data(), nullptr, m_SelectTable[i])) {}
			}

			ImGui::EndMenu();
		}
	}

	void s_Menubar::render_settings()
	{
		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::EndMenu();
		}
	}

	void s_Menubar::render_debug()
	{
		if (ImGui::BeginMenu("Debug"))
		{
			ImGui::EndMenu();
		}
	}

	void s_Menubar::load_rom_action()
	{
		dat_shared<ICartridge> cartridge = load_rom();
		if (!cartridge)
			return;

		LoadROMFileEvent event(cartridge);
		callback(event);
	}

	void s_Menubar::quit_action()
	{
		callback(WindowCloseEvent{ });
	}
}