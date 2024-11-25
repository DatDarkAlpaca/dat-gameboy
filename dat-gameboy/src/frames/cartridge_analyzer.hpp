#pragma once
#include "common.hpp"
#include "frames/frame.hpp"
#include "gameboy/components/cartridge.hpp"

namespace dat
{
	class s_CartridgeAnalyzer : public IFrame
	{
	public:
		void on_event(Event& event);

		void render();

	public:
		void set_cartridge(const dat_shared<s_Cartridge>& cartridge);

	private:
		void render_empty();

	private:
		dat_weak<s_Cartridge> m_Cartridge;
	};
}