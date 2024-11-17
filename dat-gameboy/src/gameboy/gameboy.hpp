#pragma once
#include "components/components.hpp"
#include "instruction/instruction.hpp"

namespace dat
{
	class s_Gameboy
	{	
	public:
		s_Gameboy();

	public:
		void run(u8 cycles = 1);

	public:
		s_SharpSM83 cpu;
		s_Memory memory;
		s_Clock m_Clock;
	};
}