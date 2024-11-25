#include "pch.hpp"
#include "disassembler.hpp"

namespace dat
{
	void dat::s_Disassembler::initialize(s_Gameboy* gameboy)
	{
		r_Gameboy = gameboy;
	}

	void dat::s_Disassembler::render()
	{
		if (!isOpen)
			return;

		ImGuiTableFlags flags = ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY;

		ImGui::Begin("Disassembler", &isOpen);
		ImGui::Columns(2);

		{
			ImGui::BeginChild("OpcodeViewerTable", ImVec2(0.0f, MemoryTableHeight), true, ImGuiTableFlags_Resizable | ImGuiWindowFlags_AlwaysVerticalScrollbar);
			if (ImGui::BeginTable("OpcodeViewerTable", 3, flags))
			{
				render_header_opcode();

				render_memory_entries();

				ImGui::EndTable();
			}
			ImGui::EndChild();
		}
		
		{
			if (ImGui::BeginTable("MemoryViewerTable", 18, flags))
			{
				render_header_memory();

				ImGui::EndTable();
			}
		}

		ImGui::NextColumn();

		{
			ImGui::Text("Registers");

			render_registers();

			ImGui::Separator();

			render_flags();
		}
		
		ImGui::End();
	}

	void s_Disassembler::render_header_opcode()
	{
		ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, 80);
		ImGui::TableSetupColumn("Bytes");
		ImGui::TableSetupColumn("OpCode");
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableHeadersRow();
	}

	void s_Disassembler::render_header_memory()
	{
		ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, 80);
		
		for (u32 i = 0; i < 16; ++i)
		{
			char label[2];
			snprintf(label, sizeof(label), "%02x", i);
			ImGui::TableSetupColumn(label);
		}


		ImGui::TableSetupColumn("0123456789ABCDEF", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();
	}

	void s_Disassembler::render_memory_entries()
	{
		const int numEntries = 1 * 1024;
		for (int i = 0; i < numEntries; ++i)
		{
			char address[16];
			snprintf(address, sizeof(address), "0x%04X", i);

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s", address);

			if (r_Gameboy && r_Gameboy->cpu.PC.value == i)
			{
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::GetColorU32(ImGuiCol_Header));
			}

			u8 value;
			if (r_Gameboy)
				value = r_Gameboy->memory[static_cast<u16>(i)];

			ImGui::TableSetColumnIndex(1);
			if (r_Gameboy)
				ImGui::Text("%d", (int)value);
			else
				ImGui::Text("0");

			ImGui::TableSetColumnIndex(2);
			if (r_Gameboy)
				ImGui::Text("%d", (int)value);
			else
				ImGui::Text("0");
		}
	}

	void s_Disassembler::render_registers()
	{
		auto& cpu = r_Gameboy->cpu;
		auto& mem = r_Gameboy->memory;

		if (ImGui::BeginTable("RegisterTable", 2))
		{
			ImGui::TableNextRow();

			// AF | LCDC
			{
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("AF: %01x%01x", cpu.A, cpu.F);

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("LCDC: %01x", mem.lcdc().get_value());
			}

			ImGui::TableNextRow();

			// BC | STAT
			{
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("BC: %01x%01x", cpu.BC.get_msb(), cpu.BC.get_lsb());

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("STAT: IMPLEMENT");
			}

			ImGui::TableNextRow();

			// DE | LY
			{
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("BC: %01x%01x", cpu.DE.get_msb(), cpu.DE.get_lsb());

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("LY: IMPLEMENT");
			}

			ImGui::TableNextRow();

			// HL | CNT
			{
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("HL: %01x%01x", cpu.HL.get_msb(), cpu.HL.get_lsb());

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("CNT: IMPLEMENT");
			}

			ImGui::TableNextRow();

			// SP | IE
			{
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("SP: %01x%01x", cpu.SP.get_msb(), cpu.SP.get_lsb());

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("IE: IMPLEMENT");
			}

			ImGui::TableNextRow();

			// PC | IF
			{
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("PC: %01x%01x", cpu.PC.get_msb(), cpu.PC.get_lsb());

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("IF: IMPLEMENT");
			}

			ImGui::TableNextRow();

			// IME | SPD
			{
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("IME: IMPLEMENT");

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("SPD: IMPLEMENT");
			}

			ImGui::TableNextRow();

			// IMA | ROM
			{
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("IMA: IMPLEMENT");

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("ROm: IMPLEMENT");
			}

			ImGui::EndTable();
		}
	}

	void s_Disassembler::render_flags()
	{
		bool isZ = r_Gameboy->cpu.F.Z;
		bool isN = r_Gameboy->cpu.F.N;
		bool isH = r_Gameboy->cpu.F.H;
		bool isC = r_Gameboy->cpu.F.C;

		ImGui::Checkbox("Z", &isZ); ImGui::SameLine();
		ImGui::Checkbox("N", &isN); ImGui::SameLine();
		ImGui::Checkbox("H", &isH); ImGui::SameLine();
		ImGui::Checkbox("C", &isC); ImGui::SameLine();
	}
}
