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
			TextureDescriptor descriptor;
			descriptor.width = 160;
			descriptor.height = 144;
			descriptor.generateMipmaps = false;

			m_ScreenTexture = create_texture(descriptor, nullptr);
		}

	public:
		void render()
		{
			// Gameboy:
			render_frame("Gameboy", false);
			auto gameboyPosition = ImGui::GetWindowPos();

			render_popup();
			
			ImGui::SetCursorScreenPos({ 
				gameboyPosition.x, 
				gameboyPosition.y 
			});
			ImGui::Image((ImTextureID)(intptr_t)m_ScreenTexture, { 160 * 2, 144 * 2});
			
			ImGui::End();
		}
		
		void update(const std::array<e_Color, 160 * 144>& data)
		{
			u8 rgbBuffer[160 * 144 * 3] = {};
			glm::u8vec3 colorWhite = { 255, 255, 255 };
			glm::u8vec3 colorLight = { 155, 155, 155 };
			glm::u8vec3 colorDark  = {  90,  90,  90 };
			glm::u8vec3 colorBlack = { 000, 000, 000 };

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
						rgbBuffer[index * 3 + 0] = colorLight.r;
						rgbBuffer[index * 3 + 1] = colorLight.g;
						rgbBuffer[index * 3 + 2] = colorLight.b;
					} break;

					case e_Color::DARK_GRAY:
					{
						rgbBuffer[index * 3 + 0] = colorDark.r;
						rgbBuffer[index * 3 + 1] = colorDark.g;
						rgbBuffer[index * 3 + 2] = colorDark.b;
					} break;

					case e_Color::BLACK:
					{
						rgbBuffer[index * 3 + 0] = colorBlack.r;
						rgbBuffer[index * 3 + 1] = colorBlack.g;
						rgbBuffer[index * 3 + 2] = colorBlack.b;
					} break;
				}

				++index;
			}

			update_texture(m_ScreenTexture, 160, 144, GL_RGB, GL_UNSIGNED_BYTE, rgbBuffer);
		}

	private:
		void render_popup()
		{
			if (ImGui::BeginPopupContextItem("GameboyPopup", ImGuiMouseButton_Right))
			{
				ImGui::Text("Gameboy");

				std::string turnText = gameboy.isOn() ? "Turn Off" : "Turn On";
				if (ImGui::Button(turnText.c_str()))
					gameboy.togglePower();


				ImGui::EndPopup();
			}
		}

	private:
		gl_handle m_ScreenTexture;
		s_Gameboy& gameboy;
	};
}