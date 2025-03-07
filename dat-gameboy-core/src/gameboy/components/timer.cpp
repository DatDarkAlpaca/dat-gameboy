#include "pch.hpp"
#include "timer.hpp"

namespace dat
{
	void s_Timer::initialize(
		NON_OWNING s_MMU* memory,
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
		this->memory = memory;

		restart();
	}

	void s_Timer::tick()
	{
		++m_InternalDIV;

		bool currentTima = (m_InternalDIV & m_SelectedClock) != 0;
		currentTima &= timerControl->is_bit_set(e_TAC::ENABLE);

		if (m_PreviousTIMA && !currentTima)
		{
			++timerCounter;
			if (currentTima == 0x00)
			{
				m_Overflow = 1;
				m_OverflowTicks = 0;
			}
		}

		m_PreviousTIMA = currentTima;

		if (m_Overflow) 
		{
			++m_OverflowTicks;

			if (m_OverflowTicks == 4)
				memory->IF().set(e_IF::TIMER, true);

			else if (m_OverflowTicks == 5)
				timerCounter->write(timerModulo->get());

			else if (m_OverflowTicks == 6) 
			{
				m_Overflow = 0;
				m_OverflowTicks = 0;
			}
		}
	}

	void s_Timer::restart()
	{
		m_SelectedClock = ClockSelector[0];
		bool m_Overflow = false;
		bool m_PreviousTIMA = false;
		u16 m_InternalDIV = 0;
		u8 m_OverflowTicks = 0;
	}

	void s_Timer::write(u16 address, u8 value)
	{
		if (address == DIV_Address)
			divider->write(0x0);

		else if (address == TIMA_Address)
		{
			if (m_OverflowTicks == 5)
				return;

			timerCounter->write(value);
			m_Overflow = false;
			m_OverflowTicks = 0;
		}

		else if (address == TMA_Address)
		{
			timerModulo->write(value);

			if (m_OverflowTicks == 5)
				timerCounter->write(value);
		}

		else if (address == TAC_Address)
		{
			bool prevEnabledTimer = timerControl->is_bit_set(e_TAC::ENABLE);
			u16 prevSelectedClock = m_SelectedClock;

			timerControl->write(value & 0b111);

			m_SelectedClock = ClockSelector[timerControl->get_clock_select()];

			handle_tima_obscure_behavior(prevEnabledTimer, prevSelectedClock);
		}
	}

	void s_Timer::handle_tima_obscure_behavior(bool prevEnable, u16 prevSelectedClock)
	{
		if (!prevEnable)
			return;

		bool isTimerEnabled = timerControl->is_bit_set(e_TAC::ENABLE);

		if (m_InternalDIV & prevSelectedClock)
		{
			if (!isTimerEnabled || !(m_InternalDIV & m_SelectedClock))
			{
				++timerCounter;
				if (timerCounter->get() == 0x00)
				{
					timerCounter->write(timerModulo->get());
					memory->IF().set(e_IF::TIMER, true);
				}

				m_PreviousTIMA = false;
			}
		}
	}
}