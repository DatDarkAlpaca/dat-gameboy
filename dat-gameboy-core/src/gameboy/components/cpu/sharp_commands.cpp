#include "pch.hpp"
#include "sharp_sm83.hpp"

namespace dat
{
	void s_SharpSM83::run_00() { nop(); }												// NOP
	void s_SharpSM83::run_01() { load(BC.get(), fetch_word()); }						// LD BC, n16
	void s_SharpSM83::run_02() { load_memory(BC.get(), A); }							// LD [BC], A
	void s_SharpSM83::run_03() { inc(BC.get()); }			 							// INC BC
	void s_SharpSM83::run_04() { inc(BC.get_msb()); }			 						// INC B
	void s_SharpSM83::run_05() { dec(BC.get_msb()); }			 						// DEC B
	void s_SharpSM83::run_06() { load(BC.get_msb(), fetch_byte()); }			 		// LD B, n8
	void s_SharpSM83::run_07() { rlca(); }					 							// RLCA
	void s_SharpSM83::run_08() { load_a16_SP(); }										// LD [a16], SP
	void s_SharpSM83::run_09() { add(HL.get(), BC.get()); }			 					// ADD HL, BC
	void s_SharpSM83::run_0A() { load(A, value_at(BC.get())); }							// LD A, [BC]
	void s_SharpSM83::run_0B() { dec(BC.get()); }			 							// DEC BC
	void s_SharpSM83::run_0C() { inc(BC.get_lsb()); }			 						// INC C
	void s_SharpSM83::run_0D() { dec(BC.get_lsb()); }			 						// DEC C
	void s_SharpSM83::run_0E() { load(BC.get_lsb(), fetch_byte()); }			 		// LD C, n8
	void s_SharpSM83::run_0F() { rrca(); }												// RRCA

	void s_SharpSM83::run_10() { stop(); }												// STOP n8
	void s_SharpSM83::run_11() { load(DE.get(), fetch_word()); }						// LD DE, n16
	void s_SharpSM83::run_12() { load_memory(DE.get(), A); }							// LD [DE], A
	void s_SharpSM83::run_13() { inc(DE.get()); }										// INC DE
	void s_SharpSM83::run_14() { inc(DE.get_msb()); }									// INC D
	void s_SharpSM83::run_15() { dec(DE.get_msb()); }									// DEC D
	void s_SharpSM83::run_16() { load(DE.get_msb(), fetch_byte()); }					// LD D, n8
	void s_SharpSM83::run_17() { rla(); }												// RLA
	void s_SharpSM83::run_18() { jr(fetch_signed()); }									// JR e8
	void s_SharpSM83::run_19() { add(HL.get(), DE.get()); }								// ADD HL, DE
	void s_SharpSM83::run_1A() { load(A, value_at(DE.get())); }							// LD A, [DE]
	void s_SharpSM83::run_1B() { dec(DE.get()); }										// DEC DE
	void s_SharpSM83::run_1C() { inc(DE.get_lsb()); }									// INC E
	void s_SharpSM83::run_1D() { dec(DE.get_lsb()); }									// DEC E
	void s_SharpSM83::run_1E() { load(DE.get_lsb(), fetch_byte()); }					// LD E, n8
	void s_SharpSM83::run_1F() { rra(); }												// RRA

	void s_SharpSM83::run_20() { jr(condNZ, fetch_signed()); }							// JR NZ, e8
	void s_SharpSM83::run_21() { load(HL.get(), fetch_word()); }						// LD HL, n16
	void s_SharpSM83::run_22() { load_memory(HL.get(), A); HL.get()++; }				// LD [HL+], A
	void s_SharpSM83::run_23() { inc(HL.get()); }										// INC HL
	void s_SharpSM83::run_24() { inc(HL.get_msb()); }									// INC H
	void s_SharpSM83::run_25() { dec(HL.get_msb()); }									// DEC H
	void s_SharpSM83::run_26() { load(HL.get_msb(), fetch_byte()); }					// LD H, n8
	void s_SharpSM83::run_27() { daa(); }												// DAA
	void s_SharpSM83::run_28() { jr(condZ, fetch_signed()); }							// JR Z, e8
	void s_SharpSM83::run_29() { add(HL.get(), HL.get()); }								// ADD HL, HL
	void s_SharpSM83::run_2A() { load(A, value_at(HL.get())); HL.get()++; }				// LD A, [HL+]
	void s_SharpSM83::run_2B() { dec(HL.get()); }										// DEC HL
	void s_SharpSM83::run_2C() { inc(HL.get_lsb()); }									// INC L
	void s_SharpSM83::run_2D() { dec(HL.get_lsb()); }									// DEC L
	void s_SharpSM83::run_2E() { load(HL.get_lsb(), fetch_byte()); }					// LD L, n8
	void s_SharpSM83::run_2F() { cpl(); }												// CPL

	void s_SharpSM83::run_30() { jr(condNC, fetch_signed()); }							// JR NC, e8
	void s_SharpSM83::run_31() { load(SP.get(), fetch_word()); }						// LD SP, n16
	void s_SharpSM83::run_32() { load_memory(HL.get(), A); HL.get()--; }				// LD [HL-], A
	void s_SharpSM83::run_33() { inc(SP.get()); }										// INC SP
	void s_SharpSM83::run_34() { inc_memory(HL.get()); }								// INC [HL]
	void s_SharpSM83::run_35() { dec_memory(HL.get()); }								// DEC [HL]
	void s_SharpSM83::run_36() { load_memory(HL.get(), fetch_byte()); }					// LD [HL], n8
	void s_SharpSM83::run_37() { scf(); }												// SCF
	void s_SharpSM83::run_38() { jr(condC, fetch_signed()); }							// JR C, e8
	void s_SharpSM83::run_39() { add(HL.get(), SP.get()); }								// ADD HL, SP
	void s_SharpSM83::run_3A() { load(A, value_at(HL.get())); HL.get()--; }				// LD A, [HL-]
	void s_SharpSM83::run_3B() { dec(SP.get()); }										// DEC SP
	void s_SharpSM83::run_3C() { inc(A); }												// INC A
	void s_SharpSM83::run_3D() { dec(A); }												// DEC A
	void s_SharpSM83::run_3E() { load(A, fetch_byte()); }								// LD A, n8
	void s_SharpSM83::run_3F() { ccf(); }												// CCF

	void s_SharpSM83::run_40() { load(BC.get_msb(), BC.get_msb()); }					// LD B, B
	void s_SharpSM83::run_41() { load(BC.get_msb(), BC.get_lsb()); }					// LD B, C
	void s_SharpSM83::run_42() { load(BC.get_msb(), DE.get_msb()); }					// LD B, D
	void s_SharpSM83::run_43() { load(BC.get_msb(), DE.get_lsb()); }					// LD B, E
	void s_SharpSM83::run_44() { load(BC.get_msb(), HL.get_msb()); }					// LD B, H
	void s_SharpSM83::run_45() { load(BC.get_msb(), HL.get_lsb()); }					// LD B, L
	void s_SharpSM83::run_46() { load(BC.get_msb(), value_at(HL.get())); }				// LD B, [HL]
	void s_SharpSM83::run_47() { load(BC.get_msb(), A); }								// LD B, A
	void s_SharpSM83::run_48() { load(BC.get_lsb(), BC.get_msb()); }					// LD C, B
	void s_SharpSM83::run_49() { load(BC.get_lsb(), BC.get_lsb()); }					// LD C, C
	void s_SharpSM83::run_4A() { load(BC.get_lsb(), DE.get_msb()); }					// LD C, D
	void s_SharpSM83::run_4B() { load(BC.get_lsb(), DE.get_lsb()); }					// LD C, E
	void s_SharpSM83::run_4C() { load(BC.get_lsb(), HL.get_msb()); }					// LD C, H
	void s_SharpSM83::run_4D() { load(BC.get_lsb(), HL.get_lsb()); }					// LD C, L
	void s_SharpSM83::run_4E() { load(BC.get_lsb(), value_at(HL.get())); }				// LD C, [HL]
	void s_SharpSM83::run_4F() { load(BC.get_lsb(), A); }								// LD C, A

	void s_SharpSM83::run_50() { load(DE.get_msb(), BC.get_msb()); }					// LD D, B
	void s_SharpSM83::run_51() { load(DE.get_msb(), BC.get_lsb()); }					// LD D, C
	void s_SharpSM83::run_52() { load(DE.get_msb(), DE.get_msb()); }					// LD D, D
	void s_SharpSM83::run_53() { load(DE.get_msb(), DE.get_lsb()); }					// LD D, E
	void s_SharpSM83::run_54() { load(DE.get_msb(), HL.get_msb()); }					// LD D, H
	void s_SharpSM83::run_55() { load(DE.get_msb(), HL.get_lsb()); }					// LD D, L
	void s_SharpSM83::run_56() { load(DE.get_msb(), value_at(HL.get())); }				// LD D, [HL]
	void s_SharpSM83::run_57() { load(DE.get_msb(), A); }								// LD D, A
	void s_SharpSM83::run_58() { load(DE.get_lsb(), BC.get_msb()); }					// LD E, B
	void s_SharpSM83::run_59() { load(DE.get_lsb(), BC.get_lsb()); }					// LD E, C
	void s_SharpSM83::run_5A() { load(DE.get_lsb(), DE.get_msb()); }					// LD E, D
	void s_SharpSM83::run_5B() { load(DE.get_lsb(), DE.get_lsb()); }					// LD E, E
	void s_SharpSM83::run_5C() { load(DE.get_lsb(), HL.get_msb()); }					// LD E, H
	void s_SharpSM83::run_5D() { load(DE.get_lsb(), HL.get_lsb()); }					// LD E, L
	void s_SharpSM83::run_5E() { load(DE.get_lsb(), value_at(HL.get())); }				// LD E, [HL]
	void s_SharpSM83::run_5F() { load(DE.get_lsb(), A); }								// LD E, A

	void s_SharpSM83::run_60() { load(HL.get_msb(), BC.get_msb()); }					// LD H, B
	void s_SharpSM83::run_61() { load(HL.get_msb(), BC.get_lsb()); }					// LD H, C
	void s_SharpSM83::run_62() { load(HL.get_msb(), DE.get_msb()); }					// LD H, D
	void s_SharpSM83::run_63() { load(HL.get_msb(), DE.get_lsb()); }					// LD H, E
	void s_SharpSM83::run_64() { load(HL.get_msb(), HL.get_msb()); }					// LD H, H
	void s_SharpSM83::run_65() { load(HL.get_msb(), HL.get_lsb()); }					// LD H, L
	void s_SharpSM83::run_66() { load(HL.get_msb(), value_at(HL.get())); }				// LD H, [HL]
	void s_SharpSM83::run_67() { load(HL.get_msb(), A); }								// LD H, A
	void s_SharpSM83::run_68() { load(HL.get_lsb(), BC.get_msb()); }					// LD L, B
	void s_SharpSM83::run_69() { load(HL.get_lsb(), BC.get_lsb()); }					// LD L, C
	void s_SharpSM83::run_6A() { load(HL.get_lsb(), DE.get_msb()); }					// LD L, D
	void s_SharpSM83::run_6B() { load(HL.get_lsb(), DE.get_lsb()); }					// LD L, E
	void s_SharpSM83::run_6C() { load(HL.get_lsb(), HL.get_msb()); }					// LD L, H
	void s_SharpSM83::run_6D() { load(HL.get_lsb(), HL.get_lsb()); }					// LD L, L
	void s_SharpSM83::run_6E() { load(HL.get_lsb(), value_at(HL.get())); }				// LD L, [HL]
	void s_SharpSM83::run_6F() { load(HL.get_lsb(), A); }								// LD L, A

	void s_SharpSM83::run_70() { load_memory(HL.get(), BC.get_msb()); }					// LD [HL], B
	void s_SharpSM83::run_71() { load_memory(HL.get(), BC.get_lsb()); }					// LD [HL], C
	void s_SharpSM83::run_72() { load_memory(HL.get(), DE.get_msb()); }					// LD [HL], D
	void s_SharpSM83::run_73() { load_memory(HL.get(), DE.get_lsb()); }					// LD [HL], E
	void s_SharpSM83::run_74() { load_memory(HL.get(), HL.get_msb()); }					// LD [HL], H
	void s_SharpSM83::run_75() { load_memory(HL.get(), HL.get_lsb()); }					// LD [HL], L
	void s_SharpSM83::run_76() { halt(); }												// HALT
	void s_SharpSM83::run_77() { load_memory(HL.get(), A); }							// LD [HL], A
	void s_SharpSM83::run_78() { load(A, BC.get_msb()); }								// LD A, B
	void s_SharpSM83::run_79() { load(A, BC.get_lsb()); }								// LD A, C
	void s_SharpSM83::run_7A() { load(A, DE.get_msb()); }								// LD A, D
	void s_SharpSM83::run_7B() { load(A, DE.get_lsb()); }								// LD A, E
	void s_SharpSM83::run_7C() { load(A, HL.get_msb()); }								// LD A, H
	void s_SharpSM83::run_7D() { load(A, HL.get_lsb()); }								// LD A, L
	void s_SharpSM83::run_7E() { load(A, value_at(HL.get())); }							// LD A, [HL]
	void s_SharpSM83::run_7F() { load(A, A); }											// LD A, A

	void s_SharpSM83::run_80() { add(A, BC.get_msb()); }								// ADD A, B
	void s_SharpSM83::run_81() { add(A, BC.get_lsb()); }								// ADD A, C
	void s_SharpSM83::run_82() { add(A, DE.get_msb()); }								// ADD A, D
	void s_SharpSM83::run_83() { add(A, DE.get_lsb()); }								// ADD A, E
	void s_SharpSM83::run_84() { add(A, HL.get_msb()); }								// ADD A, H
	void s_SharpSM83::run_85() { add(A, HL.get_lsb()); }								// ADD A, L
	void s_SharpSM83::run_86() { add(A, value_at(HL.get())); }							// ADD A, [HL]
	void s_SharpSM83::run_87() { add(A, A); }											// ADD A, A
	void s_SharpSM83::run_88() { adc(A, BC.get_msb()); }								// ADC A, B
	void s_SharpSM83::run_89() { adc(A, BC.get_lsb()); }								// ADC A, C
	void s_SharpSM83::run_8A() { adc(A, DE.get_msb()); }								// ADC A, D
	void s_SharpSM83::run_8B() { adc(A, DE.get_lsb()); }								// ADC A, E
	void s_SharpSM83::run_8C() { adc(A, HL.get_msb()); }								// ADC A, H
	void s_SharpSM83::run_8D() { adc(A, HL.get_lsb()); }								// ADC A, L
	void s_SharpSM83::run_8E() { adc(A, value_at(HL.get())); }							// ADC A, [HL]
	void s_SharpSM83::run_8F() { adc(A, A); }											// ADC A, A

	void s_SharpSM83::run_90() { sub(A, BC.get_msb()); }								// SUB A, B
	void s_SharpSM83::run_91() { sub(A, BC.get_lsb()); }								// SUB A, C
	void s_SharpSM83::run_92() { sub(A, DE.get_msb()); }								// SUB A, D
	void s_SharpSM83::run_93() { sub(A, DE.get_lsb()); }								// SUB A, E
	void s_SharpSM83::run_94() { sub(A, HL.get_msb()); }								// SUB A, H
	void s_SharpSM83::run_95() { sub(A, HL.get_lsb()); }								// SUB A, L
	void s_SharpSM83::run_96() { sub(A, value_at(HL.get())); }							// SUB A, [HL]
	void s_SharpSM83::run_97() { sub(A, A); }											// SUB A, A
	void s_SharpSM83::run_98() { sbc(A, BC.get_msb()); }								// SBC A, B
	void s_SharpSM83::run_99() { sbc(A, BC.get_lsb()); }								// SBC A, C
	void s_SharpSM83::run_9A() { sbc(A, DE.get_msb()); }								// SBC A, D
	void s_SharpSM83::run_9B() { sbc(A, DE.get_lsb()); }								// SBC A, E
	void s_SharpSM83::run_9C() { sbc(A, HL.get_msb()); }								// SBC A, H
	void s_SharpSM83::run_9D() { sbc(A, HL.get_lsb()); }								// SBC A, L
	void s_SharpSM83::run_9E() { sbc(A, value_at(HL.get())); }							// SBC A, [HL]
	void s_SharpSM83::run_9F() { sbc(A, A); }											// SBC A, A

	void s_SharpSM83::run_A0() { _and(A, BC.get_msb()); }								// AND A, B
	void s_SharpSM83::run_A1() { _and(A, BC.get_lsb()); }								// AND A, C
	void s_SharpSM83::run_A2() { _and(A, DE.get_msb()); }								// AND A, D
	void s_SharpSM83::run_A3() { _and(A, DE.get_lsb()); }								// AND A, E
	void s_SharpSM83::run_A4() { _and(A, HL.get_msb()); }								// AND A, H
	void s_SharpSM83::run_A5() { _and(A, HL.get_lsb()); }								// AND A, L
	void s_SharpSM83::run_A6() { _and(A, value_at(HL.get())); }							// AND A, [HL]
	void s_SharpSM83::run_A7() { _and(A, A); }											// AND A, A
	void s_SharpSM83::run_A8() { _xor(A, BC.get_msb()); }								// XOR A, B
	void s_SharpSM83::run_A9() { _xor(A, BC.get_lsb()); }								// XOR A, C
	void s_SharpSM83::run_AA() { _xor(A, DE.get_msb()); }								// XOR A, D
	void s_SharpSM83::run_AB() { _xor(A, DE.get_lsb()); }								// XOR A, E
	void s_SharpSM83::run_AC() { _xor(A, HL.get_msb()); }								// XOR A, H
	void s_SharpSM83::run_AD() { _xor(A, HL.get_lsb()); }								// XOR A, L
	void s_SharpSM83::run_AE() { _xor(A, value_at(HL.get())); }							// XOR A, [HL]
	void s_SharpSM83::run_AF() { _xor(A, A); }											// XOR A, A

	void s_SharpSM83::run_B0() { _or(A, BC.get_msb()); }								// OR A, B
	void s_SharpSM83::run_B1() { _or(A, BC.get_lsb()); }								// OR A, C
	void s_SharpSM83::run_B2() { _or(A, DE.get_msb()); }								// OR A, D
	void s_SharpSM83::run_B3() { _or(A, DE.get_lsb()); }								// OR A, E
	void s_SharpSM83::run_B4() { _or(A, HL.get_msb()); }								// OR A, H
	void s_SharpSM83::run_B5() { _or(A, HL.get_lsb()); }								// OR A, L
	void s_SharpSM83::run_B6() { _or(A, value_at(HL.get())); }							// OR A, [HL]
	void s_SharpSM83::run_B7() { _or(A, A); }											// OR A, A
	void s_SharpSM83::run_B8() { cp(BC.get_msb()); }									// CP A, B
	void s_SharpSM83::run_B9() { cp(BC.get_lsb()); }									// CP A, C
	void s_SharpSM83::run_BA() { cp(DE.get_msb()); }									// CP A, D
	void s_SharpSM83::run_BB() { cp(DE.get_lsb()); }									// CP A, E
	void s_SharpSM83::run_BC() { cp(HL.get_msb()); }									// CP A, H
	void s_SharpSM83::run_BD() { cp(HL.get_lsb()); }									// CP A, L
	void s_SharpSM83::run_BE() { cp(value_at(HL.get())); }								// CP A, [HL]
	void s_SharpSM83::run_BF() { cp(A); }												// CP A, A

	void s_SharpSM83::run_C0() { ret(condNZ); }											// RET NZ
	void s_SharpSM83::run_C1() { pop(BC); }												// POP BC
	void s_SharpSM83::run_C2() { jp(condNZ, fetch_word()); }							// JP NZ, a16
	void s_SharpSM83::run_C3() { jp(fetch_word()); }									// JP a16
	void s_SharpSM83::run_C4() { call(condNZ, fetch_word()); }							// CALL NZ, a16
	void s_SharpSM83::run_C5() { push(BC); }											// PUSH BC
	void s_SharpSM83::run_C6() { add(A, fetch_byte()); }								// ADD A, n8
	void s_SharpSM83::run_C7() { rst(0x00); }											// RST $00
	void s_SharpSM83::run_C8() { ret(condZ); }											// RET Z
	void s_SharpSM83::run_C9() { ret(); }												// RET
	void s_SharpSM83::run_CA() { jp(condZ, fetch_word()); }								// JP Z, a16
	void s_SharpSM83::run_CB() { m_isPrefixActive = true; }								// PREFIX
	void s_SharpSM83::run_CC() { call(condZ, fetch_word()); }							// CALL Z, a16
	void s_SharpSM83::run_CD() { call(fetch_word()); }									// CALL a16
	void s_SharpSM83::run_CE() { adc(A, fetch_byte()); }								// ADC A, n8
	void s_SharpSM83::run_CF() { rst(0x08); }											// RST $08

	void s_SharpSM83::run_D0() { ret(condNC); }											// RET NC
	void s_SharpSM83::run_D1() { pop(DE); }												// POP DE
	void s_SharpSM83::run_D2() { jp(condNC, fetch_word()); }							// JP NC, a16
	void s_SharpSM83::run_D3() { illegal("0xD3"); }										// —	
	void s_SharpSM83::run_D4() { call(condNC, fetch_word()); }							// CALL NC, a16
	void s_SharpSM83::run_D5() { push(DE); }											// PUSH DE
	void s_SharpSM83::run_D6() { sub(A, fetch_byte()); }								// SUB A, n8
	void s_SharpSM83::run_D7() { rst(0x10); }											// RST $10
	void s_SharpSM83::run_D8() { ret(condC); }											// RET C
	void s_SharpSM83::run_D9() { reti(); }												// RETI
	void s_SharpSM83::run_DA() { jp(condC, fetch_word()); }								// JP C, a16
	void s_SharpSM83::run_DB() { illegal("0xDB"); }										// —	
	void s_SharpSM83::run_DC() { call(condC, fetch_word()); }							// CALL C, a16
	void s_SharpSM83::run_DD() { illegal("0xDD"); }										// —	
	void s_SharpSM83::run_DE() { sbc(A, fetch_byte()); }								// SBC A, n8
	void s_SharpSM83::run_DF() { rst(0x18); }											// RST $18

	void s_SharpSM83::run_E0() { ldh_addr_a8_A(); }										// LDH [a8], A
	void s_SharpSM83::run_E1() { pop(HL); }												// POP HL
	void s_SharpSM83::run_E2() { ldh_C_A(); }											// LDH [C], A
	void s_SharpSM83::run_E3() { illegal("0xE3"); }										// —	
	void s_SharpSM83::run_E4() { illegal("0xE4"); }										// —	
	void s_SharpSM83::run_E5() { push(HL); }											// PUSH HL
	void s_SharpSM83::run_E6() { _and(A, fetch_byte()); }								// AND A, n8
	void s_SharpSM83::run_E7() { rst(0x20); }											// RST $20
	void s_SharpSM83::run_E8() { add(SP.get(), fetch_signed()); }						// ADD SP, e8
	void s_SharpSM83::run_E9() { jp(HL.get()); }										// JP HL
	void s_SharpSM83::run_EA() { load_memory(fetch_word(), A); }						// LD [a16], A
	void s_SharpSM83::run_EB() { illegal("0xEB"); }										// —	
	void s_SharpSM83::run_EC() { illegal("0xEC"); }										// —	
	void s_SharpSM83::run_ED() { illegal("0xED"); }										// —	
	void s_SharpSM83::run_EE() { _xor(A, fetch_byte()); }								// XOR A, n8
	void s_SharpSM83::run_EF() { rst(0x28); }											// RST $28

	void s_SharpSM83::run_F0() { ldh_A_addr_a8(); }										// LDH A, [a8]
	void s_SharpSM83::run_F1() { pop_af(); }											// POP AF
	void s_SharpSM83::run_F2() { ldh_A_C(); }											// LD A, [C]
	void s_SharpSM83::run_F3() { di(); }												// DI
	void s_SharpSM83::run_F4() { illegal("0xF4"); }										// —
	void s_SharpSM83::run_F5() { push_byte(A); push_byte(F.get_register()); }			// PUSH AF
	void s_SharpSM83::run_F6() { _or(A, fetch_byte()); }								// OR A, n8
	void s_SharpSM83::run_F7() { rst(0x30); }											// RST $30
	void s_SharpSM83::run_F8() { load_HL_SPe8(); }										// LD HL, SP + e8
	void s_SharpSM83::run_F9() { load(SP.get(), HL.get()); }							// LD SP, HL
	void s_SharpSM83::run_FA() { load(A, value_at(fetch_word())); }						// LD A, [a16]
	void s_SharpSM83::run_FB() { ei(); }												// EI
	void s_SharpSM83::run_FC() { illegal("0xFC"); }										// —	
	void s_SharpSM83::run_FD() { illegal("0xFD"); }										// —	
	void s_SharpSM83::run_FE() { cp(fetch_byte()); }									// CP A, n8
	void s_SharpSM83::run_FF() { rst(0x38); }											// RST $38
}

namespace dat
{
	void s_SharpSM83::run_cb_00() { rlc(BC.get_msb()); }								// RLC B
	void s_SharpSM83::run_cb_01() { rlc(BC.get_lsb()); }								// RLC C
	void s_SharpSM83::run_cb_02() { rlc(DE.get_msb()); }								// RLC D
	void s_SharpSM83::run_cb_03() { rlc(DE.get_lsb()); }								// RLC E
	void s_SharpSM83::run_cb_04() { rlc(HL.get_msb()); }								// RLC H
	void s_SharpSM83::run_cb_05() { rlc(HL.get_lsb()); }								// RLC L
	void s_SharpSM83::run_cb_06() { rlc_memory(HL.get()); }								// RLC [HL]
	void s_SharpSM83::run_cb_07() { rlc(A); }											// RLC A
	void s_SharpSM83::run_cb_08() { rrc(BC.get_msb()); }								// RRC B
	void s_SharpSM83::run_cb_09() { rrc(BC.get_lsb()); }								// RRC C
	void s_SharpSM83::run_cb_0A() { rrc(DE.get_msb()); }								// RRC D
	void s_SharpSM83::run_cb_0B() { rrc(DE.get_lsb()); }								// RRC E
	void s_SharpSM83::run_cb_0C() { rrc(HL.get_msb()); }								// RRC H
	void s_SharpSM83::run_cb_0D() { rrc(HL.get_lsb()); }								// RRC L
	void s_SharpSM83::run_cb_0E() { rrc_memory(HL.get()); }								// RRC [HL]
	void s_SharpSM83::run_cb_0F() { rrc(A); }											// RRC A

	void s_SharpSM83::run_cb_10() { rl(BC.get_msb()); }									// RL B
	void s_SharpSM83::run_cb_11() { rl(BC.get_lsb()); }									// RL C
	void s_SharpSM83::run_cb_12() { rl(DE.get_msb()); }									// RL D
	void s_SharpSM83::run_cb_13() { rl(DE.get_lsb()); }									// RL E
	void s_SharpSM83::run_cb_14() { rl(HL.get_msb()); }									// RL H
	void s_SharpSM83::run_cb_15() { rl(HL.get_lsb()); }									// RL L
	void s_SharpSM83::run_cb_16() { rl_memory(HL.get()); }								// RL [HL]
	void s_SharpSM83::run_cb_17() { rl(A); }											// RL A
	void s_SharpSM83::run_cb_18() { rr(BC.get_msb()); }									// RR B
	void s_SharpSM83::run_cb_19() { rr(BC.get_lsb()); }									// RR C
	void s_SharpSM83::run_cb_1A() { rr(DE.get_msb()); }									// RR D
	void s_SharpSM83::run_cb_1B() { rr(DE.get_lsb()); }									// RR E
	void s_SharpSM83::run_cb_1C() { rr(HL.get_msb()); }									// RR H
	void s_SharpSM83::run_cb_1D() { rr(HL.get_lsb()); }									// RR L
	void s_SharpSM83::run_cb_1E() { rr_memory(HL.get()); }								// RR [HL]
	void s_SharpSM83::run_cb_1F() { rr(A); }											// RR A

	void s_SharpSM83::run_cb_20() { sla(BC.get_msb()); }								// SLA B
	void s_SharpSM83::run_cb_21() { sla(BC.get_lsb()); }								// SLA C
	void s_SharpSM83::run_cb_22() { sla(DE.get_msb()); }								// SLA D
	void s_SharpSM83::run_cb_23() { sla(DE.get_lsb()); }								// SLA E
	void s_SharpSM83::run_cb_24() { sla(HL.get_msb()); }								// SLA H
	void s_SharpSM83::run_cb_25() { sla(HL.get_lsb()); }								// SLA L
	void s_SharpSM83::run_cb_26() { sla_memory(HL.get()); }								// SLA [HL]
	void s_SharpSM83::run_cb_27() { sla(A); }											// SLA A
	void s_SharpSM83::run_cb_28() { sra(BC.get_msb()); }								// SRA B
	void s_SharpSM83::run_cb_29() { sra(BC.get_lsb()); }								// SRA C
	void s_SharpSM83::run_cb_2A() { sra(DE.get_msb()); }								// SRA D
	void s_SharpSM83::run_cb_2B() { sra(DE.get_lsb()); }								// SRA E
	void s_SharpSM83::run_cb_2C() { sra(HL.get_msb()); }								// SRA H
	void s_SharpSM83::run_cb_2D() { sra(HL.get_lsb()); }								// SRA L
	void s_SharpSM83::run_cb_2E() { sra_memory((HL.get())); }							// SRA [HL]
	void s_SharpSM83::run_cb_2F() { sra(A); }											// SRA A

	void s_SharpSM83::run_cb_30() { swap(BC.get_msb()); }								// SWAP B
	void s_SharpSM83::run_cb_31() { swap(BC.get_lsb()); }								// SWAP C
	void s_SharpSM83::run_cb_32() { swap(DE.get_msb()); }								// SWAP D
	void s_SharpSM83::run_cb_33() { swap(DE.get_lsb()); }								// SWAP E
	void s_SharpSM83::run_cb_34() { swap(HL.get_msb()); }								// SWAP H
	void s_SharpSM83::run_cb_35() { swap(HL.get_lsb()); }								// SWAP L
	void s_SharpSM83::run_cb_36() { swap_memory(HL.get()); }							// SWAP [HL]
	void s_SharpSM83::run_cb_37() { swap(A); }											// SWAP A
	void s_SharpSM83::run_cb_38() { srl(BC.get_msb()); }								// SRL B
	void s_SharpSM83::run_cb_39() { srl(BC.get_lsb()); }								// SRL C
	void s_SharpSM83::run_cb_3A() { srl(DE.get_msb()); }								// SRL D
	void s_SharpSM83::run_cb_3B() { srl(DE.get_lsb()); }								// SRL E
	void s_SharpSM83::run_cb_3C() { srl(HL.get_msb()); }								// SRL H
	void s_SharpSM83::run_cb_3D() { srl(HL.get_lsb()); }								// SRL L
	void s_SharpSM83::run_cb_3E() { srl_memory(HL.get()); }								// SRL [HL]
	void s_SharpSM83::run_cb_3F() { srl(A); }											// SRL A

	void s_SharpSM83::run_cb_40() { bit(BC.get_msb(), 0); }								// BIT 0, B
	void s_SharpSM83::run_cb_41() { bit(BC.get_lsb(), 0); }								// BIT 0, C
	void s_SharpSM83::run_cb_42() { bit(DE.get_msb(), 0); }								// BIT 0, D
	void s_SharpSM83::run_cb_43() { bit(DE.get_lsb(), 0); }								// BIT 0, E
	void s_SharpSM83::run_cb_44() { bit(HL.get_msb(), 0); }								// BIT 0, H
	void s_SharpSM83::run_cb_45() { bit(HL.get_lsb(), 0); }								// BIT 0, L
	void s_SharpSM83::run_cb_46() { bit(value_at(HL.get()), 0); }						// BIT 0, [HL]
	void s_SharpSM83::run_cb_47() { bit(A, 0); }										// BIT 0, A
	void s_SharpSM83::run_cb_48() { bit(BC.get_msb(), 1); }								// BIT 1, B
	void s_SharpSM83::run_cb_49() { bit(BC.get_lsb(), 1); }								// BIT 1, C
	void s_SharpSM83::run_cb_4A() { bit(DE.get_msb(), 1); }								// BIT 1, D
	void s_SharpSM83::run_cb_4B() { bit(DE.get_lsb(), 1); }								// BIT 1, E
	void s_SharpSM83::run_cb_4C() { bit(HL.get_msb(), 1); }								// BIT 1, H
	void s_SharpSM83::run_cb_4D() { bit(HL.get_lsb(), 1); }								// BIT 1, L
	void s_SharpSM83::run_cb_4E() { bit(value_at(HL.get()), 1); }						// BIT 1, [HL]
	void s_SharpSM83::run_cb_4F() { bit(A, 1); }										// BIT 1, A

	void s_SharpSM83::run_cb_50() { bit(BC.get_msb(), 2); }								// BIT 2, B
	void s_SharpSM83::run_cb_51() { bit(BC.get_lsb(), 2); }								// BIT 2, C
	void s_SharpSM83::run_cb_52() { bit(DE.get_msb(), 2); }								// BIT 2, D
	void s_SharpSM83::run_cb_53() { bit(DE.get_lsb(), 2); }								// BIT 2, E
	void s_SharpSM83::run_cb_54() { bit(HL.get_msb(), 2); }								// BIT 2, H
	void s_SharpSM83::run_cb_55() { bit(HL.get_lsb(), 2); }								// BIT 2, L
	void s_SharpSM83::run_cb_56() { bit(value_at(HL.get()), 2); }						// BIT 2, [HL]
	void s_SharpSM83::run_cb_57() { bit(A, 2); }										// BIT 2, A
	void s_SharpSM83::run_cb_58() { bit(BC.get_msb(), 3); }								// BIT 3, B
	void s_SharpSM83::run_cb_59() { bit(BC.get_lsb(), 3); }								// BIT 3, C
	void s_SharpSM83::run_cb_5A() { bit(DE.get_msb(), 3); }								// BIT 3, D
	void s_SharpSM83::run_cb_5B() { bit(DE.get_lsb(), 3); }								// BIT 3, E
	void s_SharpSM83::run_cb_5C() { bit(HL.get_msb(), 3); }								// BIT 3, H
	void s_SharpSM83::run_cb_5D() { bit(HL.get_lsb(), 3); }								// BIT 3, L
	void s_SharpSM83::run_cb_5E() { bit(value_at(HL.get()), 3); }						// BIT 3, [HL]
	void s_SharpSM83::run_cb_5F() { bit(A, 3); }										// BIT 3, A

	void s_SharpSM83::run_cb_60() { bit(BC.get_msb(), 4); }								// BIT 4, B
	void s_SharpSM83::run_cb_61() { bit(BC.get_lsb(), 4); }								// BIT 4, C
	void s_SharpSM83::run_cb_62() { bit(DE.get_msb(), 4); }								// BIT 4, D
	void s_SharpSM83::run_cb_63() { bit(DE.get_lsb(), 4); }								// BIT 4, E
	void s_SharpSM83::run_cb_64() { bit(HL.get_msb(), 4); }								// BIT 4, H
	void s_SharpSM83::run_cb_65() { bit(HL.get_lsb(), 4); }								// BIT 4, L
	void s_SharpSM83::run_cb_66() { bit(value_at(HL.get()), 4); }						// BIT 4, [HL]
	void s_SharpSM83::run_cb_67() { bit(A, 4); }										// BIT 4, A
	void s_SharpSM83::run_cb_68() { bit(BC.get_msb(), 5); }								// BIT 5, B
	void s_SharpSM83::run_cb_69() { bit(BC.get_lsb(), 5); }								// BIT 5, C
	void s_SharpSM83::run_cb_6A() { bit(DE.get_msb(), 5); }								// BIT 5, D
	void s_SharpSM83::run_cb_6B() { bit(DE.get_lsb(), 5); }								// BIT 5, E
	void s_SharpSM83::run_cb_6C() { bit(HL.get_msb(), 5); }								// BIT 5, H
	void s_SharpSM83::run_cb_6D() { bit(HL.get_lsb(), 5); }								// BIT 5, L
	void s_SharpSM83::run_cb_6E() { bit(value_at(HL.get()), 5); }						// BIT 5, [HL]
	void s_SharpSM83::run_cb_6F() { bit(A, 5); }										// BIT 5, A

	void s_SharpSM83::run_cb_70() { bit(BC.get_msb(), 6); }								// BIT 6, B
	void s_SharpSM83::run_cb_71() { bit(BC.get_lsb(), 6); }								// BIT 6, C
	void s_SharpSM83::run_cb_72() { bit(DE.get_msb(), 6); }								// BIT 6, D
	void s_SharpSM83::run_cb_73() { bit(DE.get_lsb(), 6); }								// BIT 6, E
	void s_SharpSM83::run_cb_74() { bit(HL.get_msb(), 6); }								// BIT 6, H
	void s_SharpSM83::run_cb_75() { bit(HL.get_lsb(), 6); }								// BIT 6, L
	void s_SharpSM83::run_cb_76() { bit(value_at(HL.get()), 6); }						// BIT 6, [HL]
	void s_SharpSM83::run_cb_77() { bit(A, 6); }										// BIT 6, A
	void s_SharpSM83::run_cb_78() { bit(BC.get_msb(), 7); }								// BIT 7, B
	void s_SharpSM83::run_cb_79() { bit(BC.get_lsb(), 7); }								// BIT 7, C
	void s_SharpSM83::run_cb_7A() { bit(DE.get_msb(), 7); }								// BIT 7, D
	void s_SharpSM83::run_cb_7B() { bit(DE.get_lsb(), 7); }								// BIT 7, E
	void s_SharpSM83::run_cb_7C() { bit(HL.get_msb(), 7); }								// BIT 7, H
	void s_SharpSM83::run_cb_7D() { bit(HL.get_lsb(), 7); }								// BIT 7, L
	void s_SharpSM83::run_cb_7E() { bit(value_at(HL.get()), 7); }						// BIT 7, [HL]
	void s_SharpSM83::run_cb_7F() { bit(A, 7); }										// BIT 7, A

	void s_SharpSM83::run_cb_80() { res(BC.get_msb(), 0); }								// RES 0, B
	void s_SharpSM83::run_cb_81() { res(BC.get_lsb(), 0); }								// RES 0, C
	void s_SharpSM83::run_cb_82() { res(DE.get_msb(), 0); }								// RES 0, D
	void s_SharpSM83::run_cb_83() { res(DE.get_lsb(), 0); }								// RES 0, E
	void s_SharpSM83::run_cb_84() { res(HL.get_msb(), 0); }								// RES 0, H
	void s_SharpSM83::run_cb_85() { res(HL.get_lsb(), 0); }								// RES 0, L
	void s_SharpSM83::run_cb_86() { res_memory(HL.get(), 0); }							// RES 0, [HL]
	void s_SharpSM83::run_cb_87() { res(A, 0); }										// RES 0, A
	void s_SharpSM83::run_cb_88() { res(BC.get_msb(), 1); }								// RES 1, B
	void s_SharpSM83::run_cb_89() { res(BC.get_lsb(), 1); }								// RES 1, C
	void s_SharpSM83::run_cb_8A() { res(DE.get_msb(), 1); }								// RES 1, D
	void s_SharpSM83::run_cb_8B() { res(DE.get_lsb(), 1); }								// RES 1, E
	void s_SharpSM83::run_cb_8C() { res(HL.get_msb(), 1); }								// RES 1, H
	void s_SharpSM83::run_cb_8D() { res(HL.get_lsb(), 1); }								// RES 1, L
	void s_SharpSM83::run_cb_8E() { res_memory(HL.get(), 1); }							// RES 1, [HL]
	void s_SharpSM83::run_cb_8F() { res(A, 1); }										// RES 1, A

	void s_SharpSM83::run_cb_90() { res(BC.get_msb(), 2); }								// RES 2, B
	void s_SharpSM83::run_cb_91() { res(BC.get_lsb(), 2); }								// RES 2, C
	void s_SharpSM83::run_cb_92() { res(DE.get_msb(), 2); }								// RES 2, D
	void s_SharpSM83::run_cb_93() { res(DE.get_lsb(), 2); }								// RES 2, E
	void s_SharpSM83::run_cb_94() { res(HL.get_msb(), 2); }								// RES 2, H
	void s_SharpSM83::run_cb_95() { res(HL.get_lsb(), 2); }								// RES 2, L
	void s_SharpSM83::run_cb_96() { res_memory(HL.get(), 2); }							// RES 2, [HL]
	void s_SharpSM83::run_cb_97() { res(A, 2); }										// RES 2, A
	void s_SharpSM83::run_cb_98() { res(BC.get_msb(), 3); }								// RES 3, B
	void s_SharpSM83::run_cb_99() { res(BC.get_lsb(), 3); }								// RES 3, C
	void s_SharpSM83::run_cb_9A() { res(DE.get_msb(), 3); }								// RES 3, D
	void s_SharpSM83::run_cb_9B() { res(DE.get_lsb(), 3); }								// RES 3, E
	void s_SharpSM83::run_cb_9C() { res(HL.get_msb(), 3); }								// RES 3, H
	void s_SharpSM83::run_cb_9D() { res(HL.get_lsb(), 3); }								// RES 3, L
	void s_SharpSM83::run_cb_9E() { res_memory(HL.get(), 3); }							// RES 3, [HL]
	void s_SharpSM83::run_cb_9F() { res(A, 3); }										// RES 3, A

	void s_SharpSM83::run_cb_A0() { res(BC.get_msb(), 4); }								// RES 4, B
	void s_SharpSM83::run_cb_A1() { res(BC.get_lsb(), 4); }								// RES 4, C
	void s_SharpSM83::run_cb_A2() { res(DE.get_msb(), 4); }								// RES 4, D
	void s_SharpSM83::run_cb_A3() { res(DE.get_lsb(), 4); }								// RES 4, E
	void s_SharpSM83::run_cb_A4() { res(HL.get_msb(), 4); }								// RES 4, H
	void s_SharpSM83::run_cb_A5() { res(HL.get_lsb(), 4); }								// RES 4, L
	void s_SharpSM83::run_cb_A6() { res_memory(HL.get(), 4); }							// RES 4, [HL]
	void s_SharpSM83::run_cb_A7() { res(A, 4); }										// RES 4, A
	void s_SharpSM83::run_cb_A8() { res(BC.get_msb(), 5); }								// RES 5, B
	void s_SharpSM83::run_cb_A9() { res(BC.get_lsb(), 5); }								// RES 5, C
	void s_SharpSM83::run_cb_AA() { res(DE.get_msb(), 5); }								// RES 5, D
	void s_SharpSM83::run_cb_AB() { res(DE.get_lsb(), 5); }								// RES 5, E
	void s_SharpSM83::run_cb_AC() { res(HL.get_msb(), 5); }								// RES 5, H
	void s_SharpSM83::run_cb_AD() { res(HL.get_lsb(), 5); }								// RES 5, L
	void s_SharpSM83::run_cb_AE() { res_memory(HL.get(), 5); }							// RES 5, [HL]
	void s_SharpSM83::run_cb_AF() { res(A, 5); }										// RES 5, A

	void s_SharpSM83::run_cb_B0() { res(BC.get_msb(), 6); }								// RES 6, B
	void s_SharpSM83::run_cb_B1() { res(BC.get_lsb(), 6); }								// RES 6, C
	void s_SharpSM83::run_cb_B2() { res(DE.get_msb(), 6); }								// RES 6, D
	void s_SharpSM83::run_cb_B3() { res(DE.get_lsb(), 6); }								// RES 6, E
	void s_SharpSM83::run_cb_B4() { res(HL.get_msb(), 6); }								// RES 6, H
	void s_SharpSM83::run_cb_B5() { res(HL.get_lsb(), 6); }								// RES 6, L
	void s_SharpSM83::run_cb_B6() { res_memory(HL.get(), 6); }							// RES 6, [HL]
	void s_SharpSM83::run_cb_B7() { res(A, 6); }										// RES 6, A
	void s_SharpSM83::run_cb_B8() { res(BC.get_msb(), 7); }								// RES 7, B
	void s_SharpSM83::run_cb_B9() { res(BC.get_lsb(), 7); }								// RES 7, C
	void s_SharpSM83::run_cb_BA() { res(DE.get_msb(), 7); }								// RES 7, D
	void s_SharpSM83::run_cb_BB() { res(DE.get_lsb(), 7); }								// RES 7, E
	void s_SharpSM83::run_cb_BC() { res(HL.get_msb(), 7); }								// RES 7, H
	void s_SharpSM83::run_cb_BD() { res(HL.get_lsb(), 7); }								// RES 7, L
	void s_SharpSM83::run_cb_BE() { res_memory(HL.get(), 7); }							// RES 7, [HL]
	void s_SharpSM83::run_cb_BF() { res(A, 7); }										// RES 7, A

	void s_SharpSM83::run_cb_C0() { set(BC.get_msb(), 0); }								// SET 0, B
	void s_SharpSM83::run_cb_C1() { set(BC.get_lsb(), 0); }								// SET 0, C
	void s_SharpSM83::run_cb_C2() { set(DE.get_msb(), 0); }								// SET 0, D
	void s_SharpSM83::run_cb_C3() { set(DE.get_lsb(), 0); }								// SET 0, E
	void s_SharpSM83::run_cb_C4() { set(HL.get_msb(), 0); }								// SET 0, H
	void s_SharpSM83::run_cb_C5() { set(HL.get_lsb(), 0); }								// SET 0, L
	void s_SharpSM83::run_cb_C6() { set_memory(HL.get(), 0); }							// SET 0, [HL]
	void s_SharpSM83::run_cb_C7() { set(A, 0); }										// SET 0, A
	void s_SharpSM83::run_cb_C8() { set(BC.get_msb(), 1); }								// SET 1, B
	void s_SharpSM83::run_cb_C9() { set(BC.get_lsb(), 1); }								// SET 1, C
	void s_SharpSM83::run_cb_CA() { set(DE.get_msb(), 1); }								// SET 1, D
	void s_SharpSM83::run_cb_CB() { set(DE.get_lsb(), 1); }								// SET 1, E
	void s_SharpSM83::run_cb_CC() { set(HL.get_msb(), 1); }								// SET 1, H
	void s_SharpSM83::run_cb_CD() { set(HL.get_lsb(), 1); }								// SET 1, L
	void s_SharpSM83::run_cb_CE() { set_memory(HL.get(), 1); }							// SET 1, [HL]
	void s_SharpSM83::run_cb_CF() { set(A, 1); }										// SET 1, A

	void s_SharpSM83::run_cb_D0() { set(BC.get_msb(), 2); }								// SET 2, B
	void s_SharpSM83::run_cb_D1() { set(BC.get_lsb(), 2); }								// SET 2, C
	void s_SharpSM83::run_cb_D2() { set(DE.get_msb(), 2); }								// SET 2, D
	void s_SharpSM83::run_cb_D3() { set(DE.get_lsb(), 2); }								// SET 2, E
	void s_SharpSM83::run_cb_D4() { set(HL.get_msb(), 2); }								// SET 2, H
	void s_SharpSM83::run_cb_D5() { set(HL.get_lsb(), 2); }								// SET 2, L
	void s_SharpSM83::run_cb_D6() { set_memory(HL.get(), 2); }							// SET 2, [HL]
	void s_SharpSM83::run_cb_D7() { set(A, 2); }										// SET 2, A
	void s_SharpSM83::run_cb_D8() { set(BC.get_msb(), 3); }								// SET 3, B
	void s_SharpSM83::run_cb_D9() { set(BC.get_lsb(), 3); }								// SET 3, C
	void s_SharpSM83::run_cb_DA() { set(DE.get_msb(), 3); }								// SET 3, D
	void s_SharpSM83::run_cb_DB() { set(DE.get_lsb(), 3); }								// SET 3, E
	void s_SharpSM83::run_cb_DC() { set(HL.get_msb(), 3); }								// SET 3, H
	void s_SharpSM83::run_cb_DD() { set(HL.get_lsb(), 3); }								// SET 3, L
	void s_SharpSM83::run_cb_DE() { set_memory(HL.get(), 3); }							// SET 3, [HL]
	void s_SharpSM83::run_cb_DF() { set(A, 3); }										// SET 3, A

	void s_SharpSM83::run_cb_E0() { set(BC.get_msb(), 4); }								// SET 4, B
	void s_SharpSM83::run_cb_E1() { set(BC.get_lsb(), 4); }								// SET 4, C
	void s_SharpSM83::run_cb_E2() { set(DE.get_msb(), 4); }								// SET 4, D
	void s_SharpSM83::run_cb_E3() { set(DE.get_lsb(), 4); }								// SET 4, E
	void s_SharpSM83::run_cb_E4() { set(HL.get_msb(), 4); }								// SET 4, H
	void s_SharpSM83::run_cb_E5() { set(HL.get_lsb(), 4); }								// SET 4, L
	void s_SharpSM83::run_cb_E6() { set_memory(HL.get(), 4); }							// SET 4, [HL]
	void s_SharpSM83::run_cb_E7() { set(A, 4); }										// SET 4, A
	void s_SharpSM83::run_cb_E8() { set(BC.get_msb(), 5); }								// SET 5, B
	void s_SharpSM83::run_cb_E9() { set(BC.get_lsb(), 5); }								// SET 5, C
	void s_SharpSM83::run_cb_EA() { set(DE.get_msb(), 5); }								// SET 5, D
	void s_SharpSM83::run_cb_EB() { set(DE.get_lsb(), 5); }								// SET 5, E
	void s_SharpSM83::run_cb_EC() { set(HL.get_msb(), 5); }								// SET 5, H
	void s_SharpSM83::run_cb_ED() { set(HL.get_lsb(), 5); }								// SET 5, L
	void s_SharpSM83::run_cb_EE() { set_memory(HL.get(), 5); }							// SET 5, [HL]
	void s_SharpSM83::run_cb_EF() { set(A, 5); }										// SET 5, A

	void s_SharpSM83::run_cb_F0() { set(BC.get_msb(), 6); }								// SET 6, B
	void s_SharpSM83::run_cb_F1() { set(BC.get_lsb(), 6); }								// SET 6, C
	void s_SharpSM83::run_cb_F2() { set(DE.get_msb(), 6); }								// SET 6, D
	void s_SharpSM83::run_cb_F3() { set(DE.get_lsb(), 6); }								// SET 6, E
	void s_SharpSM83::run_cb_F4() { set(HL.get_msb(), 6); }								// SET 6, H
	void s_SharpSM83::run_cb_F5() { set(HL.get_lsb(), 6); }								// SET 6, L
	void s_SharpSM83::run_cb_F6() { set_memory(HL.get(), 6); }							// SET 6, [HL]
	void s_SharpSM83::run_cb_F7() { set(A, 6); }										// SET 6, A
	void s_SharpSM83::run_cb_F8() { set(BC.get_msb(), 7); }								// SET 7, B
	void s_SharpSM83::run_cb_F9() { set(BC.get_lsb(), 7); }								// SET 7, C
	void s_SharpSM83::run_cb_FA() { set(DE.get_msb(), 7); }								// SET 7, D
	void s_SharpSM83::run_cb_FB() { set(DE.get_lsb(), 7); }								// SET 7, E
	void s_SharpSM83::run_cb_FC() { set(HL.get_msb(), 7); }								// SET 7, H
	void s_SharpSM83::run_cb_FD() { set(HL.get_lsb(), 7); }								// SET 7, L
	void s_SharpSM83::run_cb_FE() { set_memory(HL.get(), 7); }							// SET 7, [HL]
	void s_SharpSM83::run_cb_FF() { set(A, 7); }										// SET 7, A
}