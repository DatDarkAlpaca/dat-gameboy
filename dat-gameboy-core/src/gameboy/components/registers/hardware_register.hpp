#pragma once
#include "common.hpp"
#include "utils/byte_utils.hpp"

namespace dat
{
	using DataRegister = u32;

	template<typename T>
	class s_HardwareRegister
	{
	public:
		virtual ~s_HardwareRegister() = default;

	public:
		void initialize(NON_OWNING u8* data)
		{
			this->data = data;
		}

	public:
		u8 get() const
		{
			return *data;
		}

		void write(u8 value)
		{
			*data = value;
		}

	public:
		bool is_bit_set(T position) const
		{
			return check_bit(*data, static_cast<u8>(position));
		}

		void set(T position, bool value)
		{
			set_bit_value(*data, static_cast<u8>(position), value);
		}

		void set(u8 position, bool value)
		{
			set_bit_value(*data, position, value);
		}

	public:
		s_HardwareRegister& operator++() 
		{
			++(*data);
			return *this;
		}

		s_HardwareRegister operator++(int) 
		{
			s_HardwareRegister temp = *this;
			++(*data);
			return temp;
		}

		s_HardwareRegister& operator--() 
		{
			--(*data);
			return *this;
		}

		s_HardwareRegister operator--(int) 
		{
			s_HardwareRegister temp = *this;
			--(*data);
			return temp;
		}

		bool operator==(u8 value) const
		{
			return *data == value;
		}

		bool operator!=(u8 value) const
		{
			return !(*this == value);
		}

	public:
		friend bool operator==(const s_HardwareRegister& lhs, const s_HardwareRegister& rhs)
		{
			return *lhs.data == *rhs.data;
		}

		friend bool operator!=(const s_HardwareRegister& lhs, const s_HardwareRegister& rhs)
		{
			return !(lhs == rhs);
		}
		
		friend u8 operator+(const s_HardwareRegister& lhs, u8 value)
		{
			return *(lhs.data) + value;
		}

		friend u8 operator+(u8 value, const s_HardwareRegister& rhs)
		{
			return *(rhs.data) + value;
		}

		friend u8 operator-(const s_HardwareRegister& lhs, u8 value)
		{
			return *(lhs.data) - value;
		}

		friend u8 operator-(u8 value, const s_HardwareRegister& rhs)
		{
			return value - *(rhs.data);
		}

	protected:
		NON_OWNING u8* data = nullptr;
	};
}