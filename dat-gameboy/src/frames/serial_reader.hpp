#pragma once
#include <imgui.h>

#include "common.hpp"
#include "frames/frame.hpp"
#include <gameboy/components/memory.hpp>

namespace dat
{
	class s_SerialReader : public IFrame
	{
	public:
		void initialize(NON_OWNING s_MMU* memory)
		{
			r_Memory = memory;
			m_Items.push_back("");
		}

	public:
		void render()
		{
			if (!isOpen)
				return;

			if (r_Memory->SC().is_bit_set(e_SC::TRANSFER_ENABLE))
			{
				char output = static_cast<char>(r_Memory->SB().get());
				m_Items.push_back(std::string(1, output));
				r_Memory->SC().set(e_SC::TRANSFER_ENABLE, false);
			}
			
			ImGui::Begin("Serial Reader");
			
			if (ImGui::BeginListBox("Serial Data"))
			{
				for (const auto& item : m_Items)
					ImGui::Text(item.c_str());
				ImGui::EndListBox();
			}
			

			ImGui::End();
		}

	private:
		NON_OWNING s_MMU* r_Memory = nullptr;
		std::vector<std::string> m_Items;
		int m_Current = 0;
	};
}