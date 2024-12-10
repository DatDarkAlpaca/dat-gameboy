#include "pch.hpp"
#include "image_frame.hpp"

namespace dat
{
	void s_ImageFrame::initialize(gl_handle texture, float width, float height, ImGuiWindowFlags extraFlags)
	{
		m_Texture = texture;
		m_Width = width;
		m_Height = height;
		m_ExtraFlags = extraFlags;
	}

	void s_ImageFrame::set_texture(gl_handle texture)
	{
		m_Texture = texture;
	}

	void s_ImageFrame::set_scale(float scale)
	{
		m_Scale = scale;
	}

	void s_ImageFrame::render_frame(const char* frameName, bool handleEnd)
	{
		if (!isOpen)
			return;

		ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
		begin_frame(frameName, &m_IsVisible, m_ExtraFlags);

		ImGui::SetCursorPos(ImVec2(0, 0));
		ImGui::Image((ImTextureID)(intptr_t)m_Texture, { m_Width * m_Scale, m_Height * m_Scale });

		if(handleEnd)
			ImGui::End();
	}
}