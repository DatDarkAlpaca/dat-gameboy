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

	protected:
		void render_frame(const char* frameName, bool handleEnd = true);

	protected:
		void set_scale(float scale);

		inline float get_scale() const { return m_Scale; }

	private:
		ImGuiWindowFlags m_ExtraFlags = 0;
		gl_handle m_Texture = invalid_handle;
		float m_Width = 0;
		float m_Height = 0;
		bool m_IsVisible = true;
		float m_Scale = 1;
	};
}