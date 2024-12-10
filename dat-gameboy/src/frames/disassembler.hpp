#pragma once
#include "frames/frame.hpp"
#include "gameboy/gameboy.hpp"

namespace dat
{
	class s_Disassembler : public IFrame
	{
	public:
		void initialize(s_Gameboy* gameboy);

		void render();

	private:
		void render_header_opcode();

		void render_header_memory();
		
		void render_memory_entries();

		void render_registers();

		void render_flags();

		void render_debugger();

	private:
		NON_OWNING s_Gameboy* r_Gameboy = nullptr;

	private:
		static inline constexpr float MemoryTableHeight = 250.f;
		int m_MemoryIndex = 0;
	};
}