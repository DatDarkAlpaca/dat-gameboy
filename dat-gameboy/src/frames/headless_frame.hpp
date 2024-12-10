#pragma once
#include <imgui.h>
#include "frames/frame.hpp"

namespace dat
{
	struct IHeadlessFrame : public IFrame
	{
	public:
		IHeadlessFrame()
			: m_Flags(ImGuiWindowFlags_NoBackground | 
					  ImGuiWindowFlags_NoTitleBar   | 
					  ImGuiWindowFlags_NoDocking    | 
					  ImGuiWindowFlags_NoCollapse   )
		{
		}

		virtual ~IHeadlessFrame() = default;

	public:
		void begin_frame(const char* frameTitle, bool* isOpen = nullptr, ImGuiWindowFlags extraFlags = 0)
		{
			ImGui::Begin(frameTitle, isOpen, m_Flags | extraFlags);
		}

	public:
		virtual void render() = 0;

	private:
		const ImGuiWindowFlags m_Flags;
	};
}