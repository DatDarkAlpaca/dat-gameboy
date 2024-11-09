#include "pch.hpp"
#include "gameboy.hpp"
#include "constants.hpp"
#include "utils/byte_utils.hpp"
#include "instruction/instruction_set.hpp"
#include "instruction/microcode_functions.hpp"

#include <iostream>

namespace dat
{
	u8& s_Gameboy::get_register(u8 registerName)
	{
		return get_register(static_cast<e_RegisterName>(registerName));
	}

	u8& s_Gameboy::get_register(e_RegisterName registerName)
	{
		switch (registerName)
		{
			case e_RegisterName::A:
				return cpu.registers.A;

			case e_RegisterName::B:
				return cpu.registers.B();

			case e_RegisterName::C:
				return cpu.registers.C();

			case e_RegisterName::D:
				return cpu.registers.D();

			case e_RegisterName::E:
				return cpu.registers.E();

			case e_RegisterName::H:
				return cpu.registers.H();

			case e_RegisterName::L:
				return cpu.registers.L();
		}

		DAT_LOG_CRITICAL("Invalid register name: {}", static_cast<u32>(registerName));	
	}

	u16& s_Gameboy::get_compound_register(u16 registerName)
	{
		return get_compound_register(static_cast<e_RegisterName>(registerName));
	}

	u16& s_Gameboy::get_compound_register(e_RegisterName registerName)
	{
		switch (registerName)
		{
			case e_RegisterName::BC:
				return cpu.registers.BC;

			case e_RegisterName::DE:
				return cpu.registers.DE;

			case e_RegisterName::HL:
				return cpu.registers.HL;

			case e_RegisterName::SP:
				return cpu.registers.SP;

			case e_RegisterName::PC:
				return cpu.registers.PC;
		}

		DAT_LOG_CRITICAL("Invalid 16-bit register name: {}", static_cast<u32>(registerName));
	}

	u8& s_Gameboy::dereference_register(u16 registerName)
	{
		return memory.memory[get_compound_register(registerName)];
	}

	u8& s_Gameboy::dereference_register(e_RegisterName registerName)
	{
		return memory.memory[get_compound_register(registerName)];
	}

	void s_Gameboy::execute_microcode(e_Microcode microcode, const std::vector<s_Operand>& operands)
	{
		switch (microcode)
		{
			case e_Microcode::NOP:
				nop();
				break;

			case e_Microcode::STOP: 
				stop(*this);
				break;

			case e_Microcode::FETCH_VALUE:
				fetch_value(*this, operands);
				break;

			case e_Microcode::FETCH_ADDRESS:
				fetch_address(*this, operands);
				break;

			case e_Microcode::ASSIGN:
				assign(*this, operands);
				break;

			case e_Microcode::ASSIGN_FROM_REGISTER:
				assign_from_register(*this, operands);
				break;
		}
	}

	void s_Gameboy::run(u8 cycles)
	{
		constexpr static u8 tCyclesPerMcycle = 4;
		u8 currentMCycle = 1;
	
		s_Instruction instruction;
		u8 microcodeIndex = 0;

		bool isFirstFetch = true;
		bool isLastMicrocode = false;

		while (true)
		{
			if (cycles == currentMCycle)
				break;

			m_Clock.start();

			SM83Period_ns * tCyclesPerMcycle;
			if (m_Clock.duration >= 5e+7)
			{
				m_Clock.restart();

				if (isFirstFetch)
				{
					instruction = fetch_instruction();
					isFirstFetch = false;
					++currentMCycle;
					continue;
				}

				e_Microcode microcode = instruction.microcodes[microcodeIndex];
				++microcodeIndex;

				if (instruction.tStatesRequired - tCyclesPerMcycle <= 0)
					isLastMicrocode = true;

				execute_microcode(microcode, instruction.operands);
				instruction.tStatesRequired -= tCyclesPerMcycle;

				if (isLastMicrocode)
				{
					instruction = fetch_instruction();
					if (instruction.microcodes.size() == 0)
					{
						DAT_LOG_ERROR("Invalid instruction: {} (No microcodes detected).", instruction.name);
						return;
					}

					isLastMicrocode = false;
					microcodeIndex = 0;
				}
				
				++currentMCycle;
			}

			m_Clock.cycle();
		}
	}

	u8 s_Gameboy::fetch_byte()
	{
		u8 byte = memory.memory[cpu.registers.PC];
		cpu.registers.PC++;
		return byte;
	}

	s_Instruction s_Gameboy::fetch_instruction()
	{
		u8 byte = fetch_byte();
		return get_instruction_from_byte(byte);
	}
}