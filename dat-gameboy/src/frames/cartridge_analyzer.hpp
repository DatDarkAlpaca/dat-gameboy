#pragma once
#include "common.hpp"
#include "frames/frame.hpp"
#include "gameboy/components/cartridge/cartridge.hpp"

namespace dat
{
	class s_CartridgeHolder;

	class s_CartridgeAnalyzer : public IFrame
	{
	public:
		void on_event(Event& event);

		void render();

	public:
		void set_cartridge(const dat_shared<ICartridge>& cartridge);

		void set_cartridge_holder(s_CartridgeHolder* holder);

	private:
		void render_empty();

	private:
		NON_OWNING s_CartridgeHolder* r_Holder = nullptr;
		dat_weak<ICartridge> m_Cartridge;
	};
}