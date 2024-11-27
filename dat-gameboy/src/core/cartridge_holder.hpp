#pragma once
#include "common.hpp"
#include "cartridge_handle.hpp"
#include "events/frame_events.hpp"
#include "gameboy/components/cartridge/cartridge.hpp"

namespace dat
{
	class s_CartridgeHolder
	{
	public:
		cartridge_handle insert_cartridge(const dat_shared<ICartridge>& cartridge);

		void remove_cartrige(cartridge_handle handle);;

		dat_shared<ICartridge> get_cartridge(cartridge_handle handle) const;

	public:
		void set_event_callback(const EventCallback& callback);

		void on_event(Event& event);

	private:
		bool is_already_loaded(const dat_shared<ICartridge>& cartridge);

		inline cartridge_handle get_handle(u64 index) { return static_cast<cartridge_handle>(index); }

	private:
		std::vector<dat_shared<ICartridge>> m_Cartridges;
		std::deque<cartridge_handle> m_FreeList;
		EventCallback m_EventCallback;
	};
}