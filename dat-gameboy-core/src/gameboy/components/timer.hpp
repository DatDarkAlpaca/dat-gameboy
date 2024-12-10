#pragma once
#include "registers/registers.hpp"

namespace dat
{
	class s_Timer
	{
	public:
		void initialize(
			NON_OWNING s_DIV* divider,
			NON_OWNING s_TAC* timerControl,
			NON_OWNING s_TIMA* timerCounter,
			NON_OWNING s_TMA* timerModulo,
			NON_OWNING s_IF* interruptFlag)
		{
			this->divider = divider;
			this->timerControl = timerControl;
			this->timerCounter = timerCounter;
			this->timerModulo = timerModulo;
			this->interruptFlag = interruptFlag;
		}

	public:
		void restart() 
		{
			m_Cycles = 0;
		}

		void tick(u8 cycles)
		{
			divider += cycles;
			m_Cycles += cycles;

			bool isTimerActive = timerControl->is_bit_set(e_TAC::ENABLE);
			if (!isTimerActive)
				return;

			u32 clockSelect = timerControl->get_clock_select();
			u32 clocksNeeded = 0;
			switch (clockSelect)
			{
				case 0:
					clocksNeeded = ClockRate / 4096;
					break;

				case 1:
					clocksNeeded = ClockRate / 262144;
					break;

				case 2:
					clocksNeeded = ClockRate / 65536;
					break;

				case 3:
					clocksNeeded = ClockRate / 16384;
					break;
			}

			if (m_Cycles >= clocksNeeded)
			{
				m_Cycles %= clocksNeeded;

				u8 tempCounter = timerCounter->get();
				++(*timerCounter);

				if (timerCounter->get() < tempCounter)
				{
					interruptFlag->set(e_IF::TIMER, true);
					timerCounter->write(timerModulo->get());
				}
			}
		}

	private:
		NON_OWNING s_DIV* divider = nullptr;
		NON_OWNING s_TAC* timerControl = nullptr;
		NON_OWNING s_TIMA* timerCounter = nullptr;
		NON_OWNING s_TMA* timerModulo = nullptr;
		NON_OWNING s_IF* interruptFlag = nullptr;
		u32 m_Cycles = 0;
	};
}