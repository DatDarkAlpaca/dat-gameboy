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

	void s_ImageFrame::set_scalable(bool value)
	{
		m_Scalable = value;
	}

	void s_ImageFrame::render_frame(const char* frameName, bool handleEnd)
	{
		if (!isOpen)
			return;

		ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
		begin_frame(frameName, &m_IsVisible, m_ExtraFlags);

		if(!m_Scalable)
			ImGui::Image((ImTextureID)(intptr_t)m_Texture, { m_Width, m_Height });

		else
		{
			ImVec2 screenSize = ImGui::GetContentRegionAvail();

			float textureRatio = m_Height / m_Width;
			float scaledHeight = textureRatio * screenSize.x;

			ImGui::Image((ImTextureID)(intptr_t)m_Texture, { screenSize.x, scaledHeight });
		}

		if(handleEnd)
			ImGui::End();
	}
}