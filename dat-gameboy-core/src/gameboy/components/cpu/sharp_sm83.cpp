#include "pch.hpp"
#include "sharp_sm83.hpp"
#include "interrupt_sources.hpp"

#include "utils/byte_utils.hpp"
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

	void s_SharpSM83::initialize(s_MMU* memory)
	{
		r_Memory = memory;
	}

	void s_SharpSM83::tick()
	{
#ifdef DAT_LOG_CPU
		DAT_LOG_DEBUG("TICK");
#endif

		execute_interrupts();

		if (m_isHalted)
			return;

		if (m_isFirstFetch)
		{
			m_Instruction = fetch_instruction();
			m_MCyclesRequired = m_Instruction.tStates / TCyclesPerMCycle;
			m_isFirstFetch = false;
			return;
		}

		if (!m_isInstructionActive)
		{
			if (!m_isPrefixActive)
				execute_instruction();
			else
				execute_cb_instruction();

			m_isInstructionActive = true;
		}

#ifdef DAT_LOG_CPU
		else
			DAT_LOG_DEBUG("    C: {}x | WAIT", m_MCyclesRequired);
#endif

		--m_MCyclesRequired;

		if (m_MCyclesRequired == 0)
		{
			m_Instruction = fetch_instruction();
			m_MCyclesRequired = m_Instruction.tStates / 4;
			m_isInstructionActive = false;
		}
	}

	void s_SharpSM83::restart()
	{
		m_MCyclesRequired = 0;
		m_isFirstFetch = true;
		m_isInstructionActive = false;
		m_isPrefixActive = false;

		m_isHalted = false;
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

	void s_SharpSM83::execute_interrupts()
	{
		if (!m_IME)
			return;

		u8 interrupts = r_Memory->IF().get() & r_Memory->IE().get();
		if (!interrupts)
			return;

		if (m_isHalted && interrupts != 0x0)
			m_isHalted = false;
	
		m_IME = false;
		push(PC);

		if (handle_interrupt(0, interrupt::VBlank, interrupts))
			return;

		if (handle_interrupt(1, interrupt::LCDCStatus, interrupts))
			return;

		if (handle_interrupt(2, interrupt::Timer, interrupts))
			return;

		if (handle_interrupt(3, interrupt::Serial, interrupts))
			return;

		if (handle_interrupt(4, interrupt::Joypad, interrupts))
			return;
	}

	bool s_SharpSM83::handle_interrupt(u8 interrupt, u16 source, u8 interruptsEnabled)
	{
		if (!check_bit(interruptsEnabled, interrupt))
			return false;

		r_Memory->IF().set(interrupt, false);
		PC.set(source);

		return true;
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
		r_Memory->write(reg, value);
	}

	u8 s_SharpSM83::value_at(u16 reg)
	{
		return r_Memory->read(reg);
	}

	u8 s_SharpSM83::fetch_byte()
	{
		u8 byte = value_at(PC.get());
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
#ifdef DAT_LOG_CPU
		DAT_LOG_DEBUG("    C: {}x | FETCH", m_MCyclesRequired);
#endif
		
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
		m_isHalted = true;
	}

	void s_SharpSM83::halt()
	{
		m_isHalted = true;
	}

	void s_SharpSM83::di()
	{
		m_IME = false;
	}

	void s_SharpSM83::ei()
	{
		m_IME = true;
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
		r_Memory->write(address, value);
	}

	void s_SharpSM83::add(u16& target, u16 value)
	{
		u16 originalTarget = target;
		target += value;

		F.N = 0;
		F.H = check_half_overflow_16(originalTarget, value);
		F.C = check_overflow_16(originalTarget, value);
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
	}

	void s_SharpSM83::add(u8& target, u8 value)
	{
		u8 originalTarget = target;
		target += value;

		F.Z = (target == 0);
		F.N = 0;
		F.H = check_half_overflow(originalTarget, value);
		F.C = check_overflow(originalTarget + value);
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
	}

	void s_SharpSM83::inc(u16& target)
	{
		++target;
	}

	void s_SharpSM83::inc_memory(u16 address)
	{
		u8 value = r_Memory->read(address);
		inc(value);
		r_Memory->write(address, value);
	}

	void s_SharpSM83::dec(u8& target)
	{
		u8 originalTarget = target;
		--target;

		F.Z = (target == 0);
		F.N = 1;
		F.H = (target & 0x0F) == 0x0F;
	}

	void s_SharpSM83::dec(u16& target)
	{
		--target;
	}

	void s_SharpSM83::dec_memory(u16 address)
	{
		u8 value = r_Memory->read(address);
		dec(value);
		r_Memory->write(address, value);
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
		if (condition())
			jp(address);
	}

	void s_SharpSM83::jp(u16 address)
	{
		PC.set(address);
	}

	void s_SharpSM83::jr(const condition& condition, i8 offset)
	{
		if (condition())
			jr(offset);
	}

	void s_SharpSM83::jr(i8 offset)
	{
		PC += offset;
	}

	void s_SharpSM83::push(s_WordRegister data)
	{
		--SP;
		write_to(SP.get(), data.get_msb());
		--SP;
		write_to(SP.get(), data.get_lsb());
	}

	void s_SharpSM83::push_byte(u8 data)
	{
		--SP;
		write_to(SP.get(), data);
	}

	void s_SharpSM83::pop(s_WordRegister& target)
	{
		u8 lsb = value_at(SP.get());
		++SP;

		u8 msb = value_at(SP.get());
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
		u8 value = r_Memory->read(address);
		rlc(value);
		r_Memory->write(address, value);
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
		u8 value = r_Memory->read(address);
		rrc(value);
		r_Memory->write(address, value);
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
		u8 value = r_Memory->read(address);
		rl(value);
		r_Memory->write(address, value);
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
		u8 value = r_Memory->read(address);
		rr(value);
		r_Memory->write(address, value);
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
		u8 value = r_Memory->read(address);
		sla(value);
		r_Memory->write(address, value);
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
		u8 value = r_Memory->read(address);
		sra(value);
		r_Memory->write(address, value);
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
		u8 value = r_Memory->read(address);
		swap(value);
		r_Memory->write(address, value);
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
		u8 value = r_Memory->read(address);
		srl(value);
		r_Memory->write(address, value);
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
		u8 value = r_Memory->read(address);
		res(value, position);
		r_Memory->write(address, value);
	}

	void s_SharpSM83::set(u8& target, u8 position)
	{
		set_bit(target, position);
	}

	void s_SharpSM83::set_memory(u16 address, u8 position)
	{
		u8 value = r_Memory->read(address);
		set(value, position);
		r_Memory->write(address, value);
	}

	void s_SharpSM83::ret(const condition& condition)
	{
		if (condition())
			ret();
	}

	void s_SharpSM83::ret()
	{
		pop(PC);
	}

	void s_SharpSM83::reti()
	{
		ret();
		ei();
	}

	void s_SharpSM83::call(const condition& condition, u16 address)
	{
		if (condition())
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