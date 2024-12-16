#include "pch.hpp"
#include "sharp_sm83.hpp"
#include "interrupt_sources.hpp"

#include "utils/byte_utils.hpp"
#include "gameboy/gameboy.hpp"
#include "gameboy/components/memory.hpp"
#include "gameboy/instruction/instruction_set.hpp"

#define CASE_OPCODE(row)																																				\
	case 0x##row##0: run_##row##0(); break;				\
	case 0x##row##1: run_##row##1(); break;				\
	case 0x##row##2: run_##row##2(); break;				\
	case 0x##row##3: run_##row##3(); break;				\
	case 0x##row##4: run_##row##4(); break;				\
	case 0x##row##5: run_##row##5(); break;				\
	case 0x##row##6: run_##row##6(); break;				\
	case 0x##row##7: run_##row##7(); break;				\
	case 0x##row##8: run_##row##8(); break;				\
	case 0x##row##9: run_##row##9(); break;				\
	case 0x##row##A: run_##row##A(); break;				\
	case 0x##row##B: run_##row##B(); break;				\
	case 0x##row##C: run_##row##C(); break;				\
	case 0x##row##D: run_##row##D(); break;				\
	case 0x##row##E: run_##row##E(); break;				\
	case 0x##row##F: run_##row##F(); break;				\

#define CASE_CB_OPCODE(row)																																											\
	case 0x##row##0: run_cb_##row##0(); break;			\
	case 0x##row##1: run_cb_##row##1(); break;			\
	case 0x##row##2: run_cb_##row##2(); break;			\
	case 0x##row##3: run_cb_##row##3(); break;			\
	case 0x##row##4: run_cb_##row##4(); break;			\
	case 0x##row##5: run_cb_##row##5(); break;			\
	case 0x##row##6: run_cb_##row##6(); break;			\
	case 0x##row##7: run_cb_##row##7(); break;			\
	case 0x##row##8: run_cb_##row##8(); break;			\
	case 0x##row##9: run_cb_##row##9(); break;			\
	case 0x##row##A: run_cb_##row##A(); break;			\
	case 0x##row##B: run_cb_##row##B(); break;			\
	case 0x##row##C: run_cb_##row##C(); break;			\
	case 0x##row##D: run_cb_##row##D(); break;			\
	case 0x##row##E: run_cb_##row##E(); break;			\
	case 0x##row##F: run_cb_##row##F(); break;			\

namespace dat
{
	s_SharpSM83::s_SharpSM83()
	{
		restart();
	}

	void s_SharpSM83::initialize(s_Gameboy* gameboy, s_MMU* memory)
	{
		r_Gameboy = gameboy;
		r_Memory = memory;
	}

	void s_SharpSM83::tick()
	{
		bool interruptRequested = false;

		switch (m_CurrentMode)
		{
			case e_SharpMode::STANDARD:
			{
				if (m_Instruction.bytes == 0)
				{
					m_Instruction = fetch_instruction();
					return;
				}

				if (m_isPrefixActive)
					execute_cb_instruction();
				else
					execute_instruction();

				m_Instruction = fetch_instruction();

				interruptRequested = m_IME && interrupts_requested();
			} break;

			case e_SharpMode::HALTED:
			case e_SharpMode::STOPPED:
			{
				tick_components();
				interruptRequested = interrupts_requested();
			} break;

			case e_SharpMode::HALT_DI:
			{
				tick_components();
				if (interrupts_requested())
					m_CurrentMode = e_SharpMode::STANDARD;
			} break;

			case e_SharpMode::HALT_BUG:
			{
				m_Instruction = fetch_instruction();
				--PC;

				if (m_isPrefixActive)
					execute_cb_instruction();
				else
					execute_instruction();

				m_CurrentMode = e_SharpMode::STANDARD;
				interruptRequested = m_IME && interrupts_requested();
			} break;

			case e_SharpMode::IME_ENABLED:
			{
				m_IME = true;
				interruptRequested = m_IME && interrupts_requested();

				m_CurrentMode = e_SharpMode::STANDARD;
				m_Instruction = fetch_instruction();

				if (m_isPrefixActive)
					execute_cb_instruction();
				else
					execute_instruction();
			} break;
		}

		if (interruptRequested)
			execute_interrupts();
	}

	void s_SharpSM83::restart()
	{
		m_isPrefixActive = false;

		m_CurrentMode = e_SharpMode::STANDARD;
		m_IME = false;

		A = 0;
		F.set(0);
		BC.set(0x0000);
		DE.set(0x0000);
		HL.set(0x0000);
		SP.set(0x0000);
		PC.set(0x0000);

		m_Instruction = {};
	}

	void s_SharpSM83::tick_components()
	{
		r_Gameboy->tick_all_except_cpu();
	}

	bool s_SharpSM83::interrupts_requested() const
	{
		return r_Memory->IE().get() & r_Memory->IF().get() & 0x1F;
	}

	void s_SharpSM83::execute_interrupts()
	{
		if (!m_IME)
		{
			m_CurrentMode = e_SharpMode::STANDARD;
			return;
		}

		u8 interruptEnable = r_Memory->IE().get();
		u8 interruptFlags = r_Memory->IF().get();
		u8 interruptsRequested = interruptEnable & interruptFlags;

		u16 vector = 0x00;

		if (check_bit(interruptsRequested, 0))
		{
			interruptFlags &= ~bit_value(interruptsRequested, 0);
			vector = interrupt::VBlank;
		}
		else if (check_bit(interruptsRequested, 1))
		{
			interruptFlags &= ~bit_value(interruptsRequested, 1);
			vector = interrupt::LCDCStatus;
		}
		else if (check_bit(interruptsRequested, 2))
		{
			interruptFlags &= ~bit_value(interruptsRequested, 2);
			vector = interrupt::Timer;
		}
		else if (check_bit(interruptsRequested, 3))
		{
			interruptFlags &= ~bit_value(interruptsRequested, 3);
			vector = interrupt::Serial;
		}
		else if (check_bit(interruptsRequested, 4))
		{
			interruptFlags &= ~bit_value(interruptsRequested, 4);
			vector = interrupt::Joypad;
		}

		r_Memory->IF().write(interruptFlags);

		push(PC);
		PC.set(vector);

		m_IME = false;

		tick_components();
		tick_components();
		tick_components();	
	}

	void s_SharpSM83::execute_instruction()
	{
#ifdef DAT_LOG_CPU
		DAT_LOG_DEBUG("    C: {}x | {}", m_MCyclesRequired, m_Instruction.name);
#endif

		switch (m_Instruction.opcode)
		{
			CASE_OPCODE(0);
			CASE_OPCODE(1);
			CASE_OPCODE(2);
			CASE_OPCODE(3);
			CASE_OPCODE(4);
			CASE_OPCODE(5);
			CASE_OPCODE(6);
			CASE_OPCODE(7);
			CASE_OPCODE(8);
			CASE_OPCODE(9);
			CASE_OPCODE(A);
			CASE_OPCODE(B);
			CASE_OPCODE(C);
			CASE_OPCODE(D);
			CASE_OPCODE(E);
			CASE_OPCODE(F);
		}
	}

	void s_SharpSM83::execute_cb_instruction()
	{
#ifdef DAT_LOG_CPU
		DAT_LOG_DEBUG("    C: {}x | {}", m_MCyclesRequired, m_Instruction.name);
#endif

		switch (m_Instruction.opcode)
		{
			CASE_CB_OPCODE(0);
			CASE_CB_OPCODE(1);
			CASE_CB_OPCODE(2);
			CASE_CB_OPCODE(3);
			CASE_CB_OPCODE(4);
			CASE_CB_OPCODE(5);
			CASE_CB_OPCODE(6);
			CASE_CB_OPCODE(7);
			CASE_CB_OPCODE(8);
			CASE_CB_OPCODE(9);
			CASE_CB_OPCODE(A);
			CASE_CB_OPCODE(B);
			CASE_CB_OPCODE(C);
			CASE_CB_OPCODE(D);
			CASE_CB_OPCODE(E);
			CASE_CB_OPCODE(F);
		}

		m_isPrefixActive = false;
	}

	void s_SharpSM83::write_to(u16 reg, u8 value)
	{
		tick_components();
		r_Memory->write(reg, value);
	}

	u8 s_SharpSM83::read_at(u16 reg)
	{
		tick_components();
		return r_Memory->read(reg);
	}

	u8 s_SharpSM83::fetch_byte()
	{
		u8 byte = read_at(PC.get());
		++PC;
		return byte;
	}

	i8 s_SharpSM83::fetch_signed()
	{
		return static_cast<i8>(fetch_byte());
	}

	u16 s_SharpSM83::fetch_word()
	{
		u8 lsb = fetch_byte();
		u8 msb = fetch_byte();

		return static_cast<u16>((msb << 8) | lsb);
	}

	s_Instruction s_SharpSM83::fetch_instruction()
	{		
		u8 opcode = fetch_byte();
		return get_instruction_from_opcode(opcode, m_isPrefixActive);
	}
}

namespace dat
{
	void s_SharpSM83::nop()
	{
		/* NOP */
	}

	void s_SharpSM83::stop()
	{
		m_CurrentMode = e_SharpMode::STOPPED;
	}

	void s_SharpSM83::halt()
	{
		if (m_IME)
			m_CurrentMode = e_SharpMode::HALTED;
		else
		{
			if (r_Memory->IE().get() & r_Memory->IF().get() & 0x1F)
				m_CurrentMode = e_SharpMode::HALT_BUG;
			else
				m_CurrentMode = e_SharpMode::HALT_DI;
		}
	}

	void s_SharpSM83::di()
	{
		m_IME = false;
	}

	void s_SharpSM83::ei()
	{
		m_CurrentMode = e_SharpMode::IME_ENABLED;
	}

	void s_SharpSM83::load(u8& target, u8 value)
	{
		target = value;
	}

	void s_SharpSM83::load(u16& target, u16 value)
	{
		target = value;
	}

	void s_SharpSM83::load_memory(u16 address, u8 value)
	{
		write_to(address, value);
	}

	void s_SharpSM83::add(u16& target, u16 value)
	{
		u16 originalTarget = target;
		target += value;

		F.N = 0;
		F.H = check_half_overflow_16(originalTarget, value);
		F.C = check_overflow_16(originalTarget, value);

		tick_components();
	}

	void s_SharpSM83::add(u16& target, i8 value)
	{	
		u16 originalTarget = target;
		int result = target + value;

		F.Z = 0;
		F.N = 0;
		F.H = (((originalTarget ^ value ^ (result & 0xFFFF)) & 0x10) == 0x10);
		F.C = (((originalTarget ^ value ^ (result & 0xFFFF)) & 0x100) == 0x100);

		target = result;

		tick_components();
	}

	void s_SharpSM83::add(u8& target, u8 value)
	{
		u8 originalTarget = target;
		target += value;

		F.Z = (target == 0);
		F.N = 0;
		F.H = check_half_overflow(originalTarget, value);
		F.C = check_overflow(originalTarget + value);

		tick_components();
	}

	void s_SharpSM83::adc(u8& target, u8 value)
	{
		u8 originalTarget = target;
		u16 result = target + value + F.C;

		target = static_cast<u8>(result);
		
		F.Z = (target == 0);
		F.N = 0;
		F.H = check_half_overflow(originalTarget, value, F.C);
		F.C = check_overflow(result);
	}

	void s_SharpSM83::sub(u8& target, u8 value)
	{
		u8 originalTarget = target;
		target = static_cast<u8>(target - value);

		F.Z = (target == 0);
		F.N = 1;
		F.H = check_half_borrow(originalTarget, value);
		F.C = check_full_borrow(originalTarget, value);
	}

	void s_SharpSM83::sbc(u8& target, u8 value)
	{
		u8 originalTarget = target;
		int result = target - value - F.C;

		target = static_cast<u8>(result);

		F.Z = (target == 0);
		F.N = 1;
		F.H = check_half_borrow(originalTarget, value, F.C);
		F.C = result < 0;
	}

	void s_SharpSM83::inc(u8& target)
	{
		u8 originalTarget = target;
		++target;

		F.Z = (target == 0);
		F.N = 0;
		F.H = check_half_overflow(originalTarget, 1);

		tick_components();
	}

	void s_SharpSM83::inc(u16& target)
	{
		tick_components();
		++target;
	}

	void s_SharpSM83::inc_memory(u16 address)
	{
		u8 value = read_at(address);
		inc(value);
		write_to(address, value);
	}

	void s_SharpSM83::dec(u8& target)
	{
		u8 originalTarget = target;
		--target;

		F.Z = (target == 0);
		F.N = 1;
		F.H = (target & 0x0F) == 0x0F;

		tick_components();
	}

	void s_SharpSM83::dec(u16& target)
	{
		--target;
		tick_components();
	}

	void s_SharpSM83::dec_memory(u16 address)
	{
		u8 value = read_at(address);
		dec(value);
		write_to(address, value);
	}

	void s_SharpSM83::_and(u8& target, u8 value)
	{
		target &= value;
		
		F.Z = (target == 0);
		F.N = 0;
		F.H = 1;
		F.C = 0;
	}

	void s_SharpSM83::_xor(u8& target, u8 value)
	{
		target ^= value;

		F.Z = (target == 0);
		F.N = 0;
		F.H = 0;
		F.C = 0;
	}

	void s_SharpSM83::_or(u8& target, u8 value)
	{
		target |= value;

		F.Z = (target == 0);
		F.N = 0;
		F.H = 0;
		F.C = 0;
	}

	void s_SharpSM83::cp(u8 value)
	{
		u8 previousA = A;
		u8 result = A - value;

		F.Z = (result == 0);
		F.N = 1;
		F.H = check_half_borrow(previousA, value);
		F.C = check_full_borrow(previousA, value);
	}

	void s_SharpSM83::cpl()
	{
		A = ~A;

		F.N = 1;
		F.H = 1;
	}

	void s_SharpSM83::scf()
	{
		F.N = 0;
		F.H = 0;
		F.C = 1;
	}

	void s_SharpSM83::ccf()
	{
		F.N = 0;
		F.H = 0;
		F.C = ~F.C;
	}

	void s_SharpSM83::rrca()
	{
		u8 carryBit = check_bit(A, 0);
		A = (A >> 1) | (carryBit << 7);

		F.Z = 0;
		F.N = 0;
		F.H = 0;
		F.C = carryBit == 1;
	}

	void s_SharpSM83::rra()
	{
		bool carryBit = check_bit(A, 0);
		A = (A >> 1) | (F.C << 7);

		F.Z = (A == 0);
		F.N = 0;
		F.H = 0;
		F.C = carryBit;
	}

	void s_SharpSM83::rlca()
	{
		rlc(A);
		F.Z = 0;
	}

	void s_SharpSM83::rla()
	{
		rl(A);
		F.Z = 0;
	}

	void s_SharpSM83::daa()
	{
		u8 prevA = A;

		u16 correction = F.C ? 0x60 : 0x00;

		if (F.H || (!F.N && ((prevA & 0x0F) > 9)))
			correction |= 0x06;
	
		if (F.C || !F.N && (prevA > 0x99))
			correction |= 0x60;

		if (F.N)
			prevA = static_cast<u8>(prevA - correction);
		else
			prevA = static_cast<u8>(prevA + correction);

		if (((correction << 2) & 0x100) != 0)
			F.C = true;

		F.H = false;
		F.Z = (prevA == 0);

		A = prevA;
	}

	void s_SharpSM83::jp(const condition& condition, u16 address)
	{
		if (!condition())
			return;
		
		jp(address);
	}

	void s_SharpSM83::jp(u16 address)
	{
		tick_components();
		PC.set(address);
	}

	void s_SharpSM83::jr(const condition& condition, i8 offset)
	{
		if (!condition())
			return;
		
		jr(offset);
	}

	void s_SharpSM83::jr(i8 offset)
	{
		tick_components();
		PC += offset;
	}

	void s_SharpSM83::push(s_WordRegister data)
	{
		--SP;
		write_to(SP.get(), data.get_msb());
		--SP;
		write_to(SP.get(), data.get_lsb());

		tick_components();
	}

	void s_SharpSM83::push_byte(u8 data)
	{
		--SP;
		write_to(SP.get(), data);
	}

	void s_SharpSM83::pop(s_WordRegister& target)
	{
		u8 lsb = read_at(SP.get());
		++SP;

		u8 msb = read_at(SP.get());
		++SP;

		target.set_lsb(lsb);
		target.set_msb(msb);
	}

	void s_SharpSM83::rlc(u8& target)
	{
		u8 carryBit = check_bit(target, 7);
		target = static_cast<u8>((target << 1) | carryBit);

		F.Z = (target == 0);
		F.N = 0;
		F.H = 0;
		F.C = carryBit;
	}

	void s_SharpSM83::rlc_memory(u16 address)
	{
		u8 value = read_at(address);
		rlc(value);
		write_to(address, value);
	}

	void s_SharpSM83::rrc(u8& target)
	{
		u8 carryBit = check_bit(target, 0);
		u8 result = static_cast<u8>((target >> 1) | (carryBit << 7));

		F.Z = (result == 0);
		F.N = 0;
		F.H = 0;
		F.C = carryBit;

		target = result;
	}

	void s_SharpSM83::rrc_memory(u16 address)
	{
		u8 value = read_at(address);
		rrc(value);
		write_to(address, value);
	}

	void s_SharpSM83::rl(u8& target)
	{
		bool carryBit = check_bit(target, 7);
		target = static_cast<u8>(target << 1) | F.C;

		F.Z = (target == 0);
		F.N = 0;
		F.H = 0;
		F.C = carryBit;
	}

	void s_SharpSM83::rl_memory(u16 address)
	{
		u8 value = read_at(address);
		rl(value);
		write_to(address, value);
	}

	void s_SharpSM83::rr(u8& target)
	{
		bool carryBit = check_bit(target, 0);
		target = static_cast<u8>(target >> 1) | (F.C << 7);

		F.Z = (target == 0);
		F.N = 0;
		F.H = 0;
		F.C = carryBit;
	}

	void s_SharpSM83::rr_memory(u16 address)
	{
		u8 value = read_at(address);
		rr(value);
		write_to(address, value);
	}

	void s_SharpSM83::sla(u8& target)
	{
		u8 carryBit = check_bit(target, 7);
		target = static_cast<u8>(target << 1);

		F.Z = (target == 0);
		F.N = 0;
		F.H = 0;
		F.C = carryBit;
	}

	void s_SharpSM83::sla_memory(u16 address)
	{
		u8 value = read_at(address);
		sla(value);
		write_to(address, value);
	}

	void s_SharpSM83::sra(u8& target)
	{
		u8 carryBit = check_bit(target, 0);
		u8 upperBit = check_bit(target, 7);

		target = static_cast<u8>(target >> 1);
		set_bit_value(target, 7, upperBit);

		F.Z = (target == 0);
		F.N = 0;
		F.H = 0;
		F.C = carryBit;
	}

	void s_SharpSM83::sra_memory(u16 address)
	{
		u8 value = read_at(address);
		sra(value);
		write_to(address, value);
	}

	void s_SharpSM83::swap(u8& target)
	{
		target = (target & 0x0F) << 4 | (target & 0xF0) >> 4;

		F.Z = (target == 0);
		F.N = 0;
		F.H = 0;
		F.C = 0;
	}

	void s_SharpSM83::swap_memory(u16 address)
	{
		u8 value = read_at(address);
		swap(value);
		write_to(address, value);
	}

	void s_SharpSM83::srl(u8& target)
	{
		bool carryBit = check_bit(target, 0);
		target >>= 1;

		F.Z = (target == 0);
		F.N = 0;
		F.H = 0;
		F.C = carryBit;
	}

	void s_SharpSM83::srl_memory(u16 address)
	{
		u8 value = read_at(address);
		srl(value);
		write_to(address, value);
	}

	void s_SharpSM83::bit(const u8 target, const u8 position)
	{
		F.Z = !check_bit(target, position);
		F.N = 0;
		F.H = 1;
	}

	void s_SharpSM83::res(u8& target, u8 position)
	{
		reset_bit(target, position);
	}

	void s_SharpSM83::res_memory(u16 address, u8 position)
	{
		u8 value = read_at(address);
		res(value, position);
		write_to(address, value);
	}

	void s_SharpSM83::set(u8& target, u8 position)
	{
		set_bit(target, position);
	}

	void s_SharpSM83::set_memory(u16 address, u8 position)
	{
		u8 value = read_at(address);
		set(value, position);
		write_to(address, value);
	}

	void s_SharpSM83::ret(const condition& condition)
	{
		if (!condition())
		{
			tick_components();
			return;
		}
		
		ret();
	}

	void s_SharpSM83::ret()
	{
		tick_components();
		pop(PC);
	}

	void s_SharpSM83::reti()
	{
		ret();
		ei();
	}

	void s_SharpSM83::call(const condition& condition, u16 address)
	{
		if (!condition())
			return;
		
		call(address);
	}

	void s_SharpSM83::call(u16 address)
	{
		push(PC);
		PC.set(address);
	}

	void s_SharpSM83::rst(u8 vec)
	{
		push(PC);
		PC.set_lsb(vec);
		PC.set_msb(0x00);
	}
}