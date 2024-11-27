#pragma once
#include "image_frame.hpp"
#include <gameboy/gameboy.hpp>
#include "events/frame_events.hpp"

namespace dat
{
	struct s_ScreenFrame : public s_ImageFrame
	{
	public:
		s_ScreenFrame()
		{
			TextureDescriptor descriptor;
			descriptor.width = 160;
			descriptor.height = 144;
			descriptor.generateMipmaps = false;

			texture = create_texture(descriptor, nullptr);
			initialize(texture, 160, 144);
		}

	public:
		void render() override
		{
			render_frame("GameboyScreen", false);
			ImGui::SetWindowPos({ position.x, position.y }, ImGuiCond_None);
			ImGui::End();
		}

	public:
		glm::vec2 position = { 0,0 };
		gl_handle texture = invalid_handle;
	};

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
			// Gameboy:
			render_frame("Gameboy", true);
			auto gameboyPosition = ImGui::GetWindowPos();
			return;
			// Size:
			ImVec2 screenSize = ImGui::GetContentRegionAvail();
			float scaledHeight = (160 / 144) * screenSize.x;

			ImGui::End();
			return;
			// Screen:
			m_ScreenFrame.position = {
				gameboyPosition.x + 31,
				gameboyPosition.y + 30
			};

			m_ScreenFrame.render();
		}
		
		void update(const std::array<e_Color, 160 * 144>& data)
		{
			u8 rgbBuffer[160 * 144 * 3];
			glm::vec3 colorWhite = { 255, 255, 255 };
			glm::vec3 colorLight = { 155, 155, 155 };
			glm::vec3 colorDark = { 040, 040, 040 };
			glm::vec3 colorBlack = { 000, 000, 000 };

			size_t index = 0;
			for (const e_Color& color : data)
			{
				switch (color)
				{
					case e_Color::WHITE:
					{
						rgbBuffer[index * 3 + 0] = colorWhite.r;
						rgbBuffer[index * 3 + 1] = colorWhite.g;
						rgbBuffer[index * 3 + 2] = colorWhite.b;
					} break;

					case e_Color::LIGHT_GRAY:
					{
						rgbBuffer[index * 3 + 0] = colorLight[0];
						rgbBuffer[index * 3 + 1] = colorLight[1];
						rgbBuffer[index * 3 + 2] = colorLight[2];
					} break;

					case e_Color::DARK_GRAY:
					{
						rgbBuffer[index * 3 + 0] = colorDark[0];
						rgbBuffer[index * 3 + 1] = colorDark[1];
						rgbBuffer[index * 3 + 2] = colorDark[2];
					} break;

					case e_Color::BLACK:
					{
						rgbBuffer[index * 3 + 0] = colorBlack[0];
						rgbBuffer[index * 3 + 1] = colorBlack[1];
						rgbBuffer[index * 3 + 2] = colorBlack[2];
					} break;
				}

				++index;
			}

			update_texture(m_ScreenFrame.texture, 160, 144, GL_RGB, GL_UNSIGNED_BYTE, rgbBuffer);
		}

	private:
		s_ScreenFrame m_ScreenFrame;
		s_Gameboy& gameboy;
	};
}