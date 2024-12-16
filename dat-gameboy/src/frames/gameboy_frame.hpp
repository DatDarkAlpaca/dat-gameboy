#pragma once
#include <gameboy/gameboy.hpp>

#include "constants.hpp"
#include "image_frame.hpp"
#include "events/frame_events.hpp"
#include "graphics/graphics_device.hpp"

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
			initialize_screen();
		}

	public:
		void render()
		{
			// Gameboy:
			render_frame("Gameboy", false);
			auto gameboyPosition = ImGui::GetWindowPos();

			render_popup();
			
			ImGui::SetCursorScreenPos({ 
				gameboyPosition.x + 30 * get_scale(),
				gameboyPosition.y + 30 * get_scale()
			});
			ImGui::Image((ImTextureID)(intptr_t)m_ScreenTexture, { 
				GameboySpriteScreenWidth * get_scale(),
				GameboySpriteScreenHeight * get_scale()
			});
			
			ImGui::End();
		}
		
		void update(e_Color* data)
		{
			u8 rgbBuffer[160 * 144 * 3] = {};
			glm::u8vec3 colorWhite = { 255, 255, 255 };
			glm::u8vec3 colorLight = { 155, 155, 155 };
			glm::u8vec3 colorDark  = {  90,  90,  90 };
			glm::u8vec3 colorBlack = { 000, 000, 000 };

			size_t index = 0;
			for (u64 i = 0; i < GameboyScreenWidth * GameboyScreenHeight; ++i)
			{
				e_Color color = data[i];

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
		void initialize_screen() 
		{
			u8 rgbBuffer[160 * 144 * 3] = {};
			for (u64 i = 0; i < 160 * 144 * 3; ++i)
				rgbBuffer[i] = 0;

			update_texture(m_ScreenTexture, 160, 144, GL_RGB, GL_UNSIGNED_BYTE, rgbBuffer);
		}

		void render_popup()
		{
			if (ImGui::BeginPopupContextItem("GameboyPopup", ImGuiMouseButton_Right))
			{
				ImGui::Text("Gameboy");

				// Power
				{
					std::string turnText = gameboy.isOn() ? "Turn Off" : "Turn On";
					if (ImGui::Button(turnText.c_str()))
					{
						gameboy.togglePower();

						GameboyTogglePowerEvent powerEvent;
						callback(powerEvent);
					}
				}
				
				// Scale:
				{
					render_popup_scale();
				}

				ImGui::EndPopup();
			}
		}

	private:
		void render_popup_scale()
		{
			if (ImGui::Combo("Scaling Factor", &m_ScalingFactorOption, ScalingFactorOptions, 9))
			{
				float scale = 1;
				switch (m_ScalingFactorOption)
				{
					case 0: scale =  0.5f; break;
					case 1: scale = 0.75f; break;
					case 2: scale =  1.0f; break;
					case 3: scale = 1.25f; break;
					case 4: scale =  1.5f; break;
					case 5: scale = 1.75f; break;
					case 6: scale =  2.0f; break;
					case 7: scale =  4.0f; break;
					case 8: scale =  8.0f; break;
				}

				set_scale(scale);

				GameboyFrameResizeEvent resizeEvent(scale);
				callback(resizeEvent);
			}
		}

	private:
		gl_handle m_ScreenTexture;
		s_Gameboy& gameboy;

	private:
		int m_ScalingFactorOption = 2;
		static inline const char* ScalingFactorOptions[] = {
			"0.5x",
			"0.75x", 
			"1.0x", 
			"1.25x", 
			"1.5x", 
			"1.75x", 
			"2x", 
			"4x",
			"8x"
		};
	};
}