#include "pch.hpp"
#include "cartridge_holder.hpp"

namespace dat
{
	cartridge_handle dat::s_CartridgeHolder::insert_cartridge(const dat_shared<ICartridge>& cartridge)
	{
		if (is_already_loaded(cartridge))
		{
			DAT_LOG_INFO("This cartridge is already loaded.");
			return invalid_handle;
		}

		if (m_FreeList.empty())
		{
			m_Cartridges.push_back(std::move(cartridge));
			return static_cast<cartridge_handle>(m_Cartridges.size() - 1);
		}

		cartridge_handle handle = m_FreeList.back();
		m_FreeList.pop_back();
		m_Cartridges[handle] = std::move(cartridge);

		return handle;
	}

	void s_CartridgeHolder::remove_cartrige(cartridge_handle handle)
	{
		m_Cartridges[handle].reset();
		m_FreeList.push_back(handle);
	}

	dat_shared<ICartridge> s_CartridgeHolder::get_cartridge(cartridge_handle handle) const
	{
		return m_Cartridges[handle];
	}

	void s_CartridgeHolder::set_event_callback(const EventCallback& callback)
	{
		m_EventCallback = callback;
	}

	void s_CartridgeHolder::on_event(Event& event)
	{
		Subscriber subscriber(event);

		subscriber.subscribe<LoadROMFileEvent>([&](LoadROMFileEvent& romEvent) -> bool {
			cartridge_handle handle = insert_cartridge(romEvent.cartridge);

			if (handle == invalid_handle)
				return true;

			PropagateLoadROMFileEvent propagateEvent(handle);
			m_EventCallback(propagateEvent);

			return true;
		});

		subscriber.subscribe<CartridgeRemoveEvent>([&](const CartridgeRemoveEvent& event) -> bool {
			remove_cartrige(event.handle);
			return false;
		});
	}

	bool s_CartridgeHolder::is_already_loaded(const dat_shared<ICartridge>& cartridge)
	{
		for (const auto& loadedCartridge : m_Cartridges)
		{
			if (!loadedCartridge)
				continue;

			if (cartridge->filepath == loadedCartridge->filepath)
				return true;
		}

		return false;
	}
}
