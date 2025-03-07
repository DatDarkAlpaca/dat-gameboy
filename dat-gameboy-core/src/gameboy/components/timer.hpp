#pragma once
#include "registers/registers.hpp"
#include "gameboy/components/memory.hpp"

namespace dat
{
	class s_Timer
	{
	public:
		void initialize(
			NON_OWNING s_MMU* memory,
			NON_OWNING s_DIV* divider,
			NON_OWNING s_TAC* timerControl,
			NON_OWNING s_TIMA* timerCounter,
			NON_OWNING s_TMA* timerModulo,
			NON_OWNING s_IF* interruptFlag);

	public:
		void tick();

		void restart();

	public:
		void write(u16 address, u8 value);

	private:
		void handle_tima_obscure_behavior(bool prevEnable, u16 prevSelectedClock);

	private:
		NON_OWNING s_DIV* divider = nullptr;
		NON_OWNING s_TAC* timerControl = nullptr;
		NON_OWNING s_TIMA* timerCounter = nullptr;
		NON_OWNING s_TMA* timerModulo = nullptr;
		NON_OWNING s_IF* interruptFlag = nullptr;

		NON_OWNING s_MMU* memory = nullptr;
		
	private:
		u16 m_SelectedClock = 0;
		u16 m_InternalDIV = 0;

		bool m_PreviousTIMA = false;
		bool m_Overflow = false;
		u8 m_OverflowTicks = 0;

	private:
		static inline u32 ClockSelector[4] = { 512, 8, 32, 128 };
	};
}