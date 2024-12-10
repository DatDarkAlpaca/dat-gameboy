#include "pch.hpp"
#include "cartridge_analyzer.hpp"
#include "events/frame_events.hpp"
#include "core/cartridge_holder.hpp"

namespace dat
{
	void s_CartridgeAnalyzer::on_event(Event& event)
	{
		Subscriber subscriber(event);

		subscriber.subscribe<PropagateLoadROMFileEvent>([&](const PropagateLoadROMFileEvent& event) -> bool {
			auto cartridge = r_Holder->get_cartridge(event.handle);
			set_cartridge(cartridge);
			return false;
		});
	}

	void s_CartridgeAnalyzer::render()
	{
		if (!isOpen)
			return;

		if (m_Cartridge.expired())
		{
			render_empty();
			return;
		}

		auto& cartridge = m_Cartridge.lock();
		const auto& header = cartridge->header;

		ImGui::Begin("Cartridge Analyzer", &isOpen);
		ImGui::Text("Title: %s", header.title);
		ImGui::Text("Game's Publisher: %s", get_cartridge_licensee_code(header));
		ImGui::Separator();

		// Flags:
		{
			bool usesCGB = header.cgbFlag == 0x80;
			ImGui::Text("CGB: %s", (usesCGB) ? "Yes" : "No");

			bool usesSGB = header.sgbFlag == 0x03;
			ImGui::Text("SGB: %s", (usesSGB) ? "Yes" : "No");

			ImGui::Separator();
		}

		// Type | RAM | ROM:
		{
			ImGui::Text("Cartridge Type: %s", get_cartridge_type_name(header));
			ImGui::Text("Destination: %s", (header.destinationCode == 0x00) ? "Japan" : "Other");

			ImGui::Text("ROM Size: %ix KiB", get_rom_size(header) / KB(1));
			ImGui::Text("RAM Size: %ix KiB", get_ram_size(header) / KB(1));

			ImGui::Separator();
		}

		// Checksum
		{
			auto g0 = header.globalChecksum[0];
			auto g1 = header.globalChecksum[1];

			ImGui::Text("Valid: %s", is_cartridge_logo_valid(header) ? "Yes" : "No");
			ImGui::Text("Checksum: 0x%i", (int)header.headerChecksum);
			ImGui::Text("Global Checksum: 0x%i%i", (int)g0, (int)g1);

			ImGui::Separator();
		}

		ImGui::End();
	}

	void s_CartridgeAnalyzer::set_cartridge(const dat_shared<ICartridge>& cartridge)
	{
		m_Cartridge = cartridge;
	}

	void s_CartridgeAnalyzer::set_cartridge_holder(s_CartridgeHolder* holder)
	{
		r_Holder = holder;
	}

	void s_CartridgeAnalyzer::render_empty()
	{
		ImGui::Begin("Cartridge Analyzer", &isOpen);

		ImGui::Text("Title: None");
		ImGui::Text("Game's Publisher: None");
		ImGui::Separator();

		// Flags:
		{
			ImGui::Text("CGB: No");
			ImGui::Text("SGB: No");

			ImGui::Separator();
		}

		// Type | RAM | ROM:
		{
			ImGui::Text("Cartridge Type: None");
			ImGui::Text("Destination: None");

			ImGui::Text("ROM Size: 0x KiB");
			ImGui::Text("RAM Size: 0x KiB");

			ImGui::Separator();
		}

		// Checksum
		{
			ImGui::Text("Valid: No");
			ImGui::Text("Checksum: 0x0");
			ImGui::Text("Global Checksum: 0x00");

			ImGui::Separator();
		}

		ImGui::End();
	}
}