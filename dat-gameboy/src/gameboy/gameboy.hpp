#pragma once
#include "components/components.hpp"
#include "instruction/instruction.hpp"

namespace dat
{
	class s_Gameboy
	{	
	public:
		void run(u8 cycles = 1);

	public:
		u8 fetch_byte();

		s_Instruction fetch_instruction();

	public:
		u8& get_register(u8 registerName);

		u8& get_register(e_RegisterName registerName);

		u16& get_compound_register(u16 registerName);

		u16& get_compound_register(e_RegisterName registerName);

		u8& dereference_register(u16 registerName);

		u8& dereference_register(e_RegisterName registerName);

	private:
		void execute_microcode(e_Microcode microcode, const std::vector<s_Operand>& operands);

	public:
		inline void push_argument(u8 argument)
		{
			arguments[m_ArgumentIndex] = argument;
			++m_ArgumentIndex;
		}
		inline void clear_arguments()
		{
			arguments[0] = 0;
			arguments[1] = 0;
			m_ArgumentIndex = 0;
		}

	public:
		s_SharpSM83 cpu;
		s_Memory memory;
		s_Clock m_Clock;

		std::array<u8, 2> arguments;
		
	private:
		u8 m_ArgumentIndex = 0;
	};
}