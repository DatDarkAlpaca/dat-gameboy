#include "pch.hpp"
#include "cartridge_frame.hpp"
#include "cartridge_frame_holder.hpp"

namespace dat
{
	s_CartridgeFrame::s_CartridgeFrame(const std::string& name, cartridge_handle handle)
		: m_Name(name)
		, m_Handle(handle)
	{
		bool allZeroes = true;
		for (const auto& character : m_Name)
		{
			if (character != '\0')
				allZeroes = false;
		}

		if (m_Name.empty() || allZeroes)
			m_Name = "$INVALID_CARTRIDGE";
	}

	void s_CartridgeFrame::render()
	{
		render_frame(m_Name.c_str(), false);

		detect_focus();

		render_tooltip();

		render_popup();

		ImGui::End();
	}

	void s_CartridgeFrame::render_tooltip()
	{
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Cartridge: %s", m_Name.c_str());
	}

	void s_CartridgeFrame::render_popup()
	{
		if (ImGui::BeginPopupContextItem(m_Name.c_str(), ImGuiMouseButton_Right))
		{
			ImGui::Text("Cartridge: %s", m_Name.c_str());

			if (ImGui::Button("Insert Cartridge\n"))
			{
				InsertCartridgeEvent event(m_Handle);
				callback(event);
			}

			if (ImGui::Button("Unload\n"))
			{
				CartridgeRemoveEvent event(m_Handle);
				callback(event);
			}

			ImGui::EndPopup();
		}
	}

	void s_CartridgeFrame::detect_focus()
	{
		if (ImGui::IsItemClicked())
		{
			CartridgeFocusEvent event(m_Handle);
			callback(event);
		}
	}
}