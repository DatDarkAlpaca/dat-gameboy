#pragma once
#include "registers.hpp"
#include "gameboy/constants.hpp"
#include "gameboy/components/memory.hpp"
#include "gameboy/instruction/instruction.hpp"

#define OPCODE_FUNCTION(row)															    \
	void run_##row##0(); void run_##row##1(); void run_##row##2(); void run_##row##3();		\
	void run_##row##4(); void run_##row##5(); void run_##row##6(); void run_##row##7();		\
	void run_##row##8(); void run_##row##9(); void run_##row##A(); void run_##row##B();		\
	void run_##row##C(); void run_##row##D(); void run_##row##E(); void run_##row##F();		\

#define OPCODE_CB_FUNCTION(row)																			\
	void run_cb_##row##0(); void run_cb_##row##1(); void run_cb_##row##2(); void run_cb_##row##3();		\
	void run_cb_##row##4(); void run_cb_##row##5(); void run_cb_##row##6(); void run_cb_##row##7();		\
	void run_cb_##row##8(); void run_cb_##row##9(); void run_cb_##row##A(); void run_cb_##row##B();		\
	void run_cb_##row##C(); void run_cb_##row##D(); void run_cb_##row##E(); void run_cb_##row##F();		\

namespace dat
{
	struct s_SharpSM83
	{
	public:
		s_SharpSM83();

	public:
		void initialize(s_MMU* memory);

		void tick();
		
		void restart();

	private:
		void execute_interrupts();

		bool handle_interrupt(u8 interrupt, u16 source, u8 interruptsEnabled);

		void execute_instruction();

		void execute_cb_instruction();

	private:
		void write_to(u16 reg, u8 value);

		u8 value_at(u16 reg);

		u8 fetch_byte();

		i8 fetch_signed();

		u16 fetch_word();

		s_Instruction fetch_instruction();

	public:
		u8 A;
		s_SM83Flags F;
		s_WordRegister BC;
		s_WordRegister DE;
		s_WordRegister HL;
		s_WordRegister SP;
		s_WordRegister PC;

	public:
		bool get_IME() const { return m_IME; }

	private:
		using condition = std::function<bool()>;

		condition condNZ = [&]() -> bool {
			bool result = !F.Z;

			if (!result)
				m_MCyclesRequired = m_Instruction.tStatesFalse / TCyclesPerMCycle;
			else
				m_MCyclesRequired = m_Instruction.tStates / TCyclesPerMCycle;

			return result;
		};
		condition condZ = [&]() -> bool {
			bool result = F.Z;

			if (!result)
				m_MCyclesRequired = m_Instruction.tStatesFalse / TCyclesPerMCycle;
			else
				m_MCyclesRequired = m_Instruction.tStates / TCyclesPerMCycle;

			return result;
		};
		condition condNC = [&]() -> bool {
			bool result = !F.C;

			if (!result)
				m_MCyclesRequired = m_Instruction.tStatesFalse / TCyclesPerMCycle;
			else
				m_MCyclesRequired = m_Instruction.tStates / TCyclesPerMCycle;

			return result;
		};
		condition condC = [&]() -> bool {
			bool result = F.C;

			if (!result)
				m_MCyclesRequired = m_Instruction.tStatesFalse / TCyclesPerMCycle;
			else
				m_MCyclesRequired = m_Instruction.tStates / TCyclesPerMCycle;

			return result;
		};

	private:
		s_MMU* r_Memory = nullptr;

	private:
		s_Instruction m_Instruction;

		u8 m_MCyclesRequired = 0;
		bool m_isFirstFetch = true;
		bool m_isInstructionActive = false;
		bool m_isPrefixActive = false;

		bool m_isHalted = false;
		bool m_IME = false;

	private:
		inline void load_a16_SP()
		{
			u16 word = fetch_word();

			r_Memory->write(word, SP.get_lsb());
			word++;
			r_Memory->write(word, SP.get_msb());
		}

		inline void ldh_A_C()
		{
			A = value_at(0xFF00 + BC.get_lsb());
		}

		inline void ldh_C_A()
		{
			r_Memory->write(0xFF00 + BC.get_lsb(), A);
		}

		inline void ldh_addr_a8_A()
		{
			r_Memory->write(0xFF00 + fetch_byte(), A);
		}

		inline void ldh_A_addr_a8()
		{
			A = value_at(0xFF00 + fetch_byte());
		}

		inline void pop_af()
		{
			u8 lsb = value_at(SP.get());
			++SP;

			u8 msb = value_at(SP.get());
			++SP;

			A = msb;
			F.set(lsb);
		}

		inline void load_HL_SPe8()
		{
			u16 originalTarget = SP.get();
			int value = fetch_signed();

			int result = static_cast<int>(originalTarget + value);

			F.Z = 0;
			F.N = 0;
			F.H = ((originalTarget ^ value ^ (result & 0xFFFF)) & 0x10) == 0x10;
			F.C = ((originalTarget ^ value ^ (result & 0xFFFF)) & 0x100) == 0x100;

			HL.set(static_cast<u16>(result));
		}

	private:
		/* Misc */
		inline void illegal(const char* name) { DAT_LOG_WARN("Illegal opcode called: {}", name); }

		/* Control */
		void nop();
		void stop();
		void halt();
		void di();
		void ei();

		/* Load */
		void load(u8& target, u8 value);
		void load(u16& target, u16 value);
		void load_memory(u16 address, u8 value);

		/* Arithmetic */
		void add(u16& target, u16 value);
		void add(u16& target, i8 value);
		void add(u8& target, u8 value);
		void adc(u8& target, u8 value);
		void sub(u8& target, u8 value);
		void sbc(u8& target, u8 value);

		void inc(u8& target);
		void inc(u16& target);
		void inc_memory(u16 address);
		void dec(u8& target);
		void dec(u16& target);
		void dec_memory(u16 address);

		void _and(u8& target, u8 value);
		void _xor(u8& target, u8 value);
		void _or(u8& target, u8 value);
		void cp(u8 value);
		
		void cpl();
		void scf();
		void ccf();

		void rrca();
		void rra();

		void rlca();
		void rla();

		void daa();
		
		/* Jump */
		void jp(const condition& condition, u16 address);
		void jp(u16 address);
		
		void jr(const condition& condition, i8 offset);
		void jr(i8 offset);
		
		void ret(const condition& condition);
		void ret();
		void reti();

		void call(const condition& condition, u16 address);
		void call(u16 address);

		void rst(u8 vec);

		/* Stack Pointer */
		void push(s_WordRegister data);
		void push_byte(u8 data);
		void pop(s_WordRegister& target);

		/* Prefix Instructions */
		void rlc(u8& target);
		void rlc_memory(u16 address);
		void rrc(u8& target);
		void rrc_memory(u16 address);
		void rl(u8& target);
		void rl_memory(u16 address);
		void rr(u8& target);
		void rr_memory(u16 address);

		void sla(u8& target);
		void sla_memory(u16 address);
		void sra(u8& target);
		void sra_memory(u16 address);

		void swap(u8& target);
		void swap_memory(u16 address);
		void srl(u8& target);
		void srl_memory(u16 address);

		void bit(const u8 target, const u8 position);
		void res(u8& target, u8 position);
		void res_memory(u16 address, u8 position);
		void set(u8& target, u8 position);
		void set_memory(u16 address, u8 position);

	private:
		OPCODE_FUNCTION(0);
		OPCODE_FUNCTION(1);
		OPCODE_FUNCTION(2);
		OPCODE_FUNCTION(3);
		OPCODE_FUNCTION(4);
		OPCODE_FUNCTION(5);
		OPCODE_FUNCTION(6);
		OPCODE_FUNCTION(7);
		OPCODE_FUNCTION(8);
		OPCODE_FUNCTION(9);
		OPCODE_FUNCTION(A);
		OPCODE_FUNCTION(B);
		OPCODE_FUNCTION(C);
		OPCODE_FUNCTION(D);
		OPCODE_FUNCTION(E);
		OPCODE_FUNCTION(F);

		OPCODE_CB_FUNCTION(0);
		OPCODE_CB_FUNCTION(1);
		OPCODE_CB_FUNCTION(2);
		OPCODE_CB_FUNCTION(3);
		OPCODE_CB_FUNCTION(4);
		OPCODE_CB_FUNCTION(5);
		OPCODE_CB_FUNCTION(6);
		OPCODE_CB_FUNCTION(7);
		OPCODE_CB_FUNCTION(8);
		OPCODE_CB_FUNCTION(9);
		OPCODE_CB_FUNCTION(A);
		OPCODE_CB_FUNCTION(B);
		OPCODE_CB_FUNCTION(C);
		OPCODE_CB_FUNCTION(D);
		OPCODE_CB_FUNCTION(E);
		OPCODE_CB_FUNCTION(F);
	};
}

#undef OPCODE_FUNCTION