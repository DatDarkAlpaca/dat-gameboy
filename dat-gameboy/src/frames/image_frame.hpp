#pragma once
#include "headless_frame.hpp"
#include "graphics/common_graphics.hpp"

namespace dat
{
	class s_ImageFrame : public IHeadlessFrame
	{
	public:
		void initialize(gl_handle texture, float width, float height, ImGuiWindowFlags extraFlags = 0);

		void set_texture(gl_handle texture);

		void set_scalable(bool value);

	protected:
		void render_frame(const char* frameName, bool handleEnd = true);

	private:
		ImGuiWindowFlags m_ExtraFlags = 0;
		gl_handle m_Texture = invalid_handle;
		float m_Width = 0;
		float m_Height = 0;
		bool m_Scalable = false;
		bool m_IsVisible = true;
	};
}