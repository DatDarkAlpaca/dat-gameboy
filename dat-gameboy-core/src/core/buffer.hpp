	#pragma once
#include "common.hpp"

namespace dat
{
	namespace
	{
		inline u8* default_allocator(u64 size)
		{
			return (u8*)malloc(size);
		}

		inline void default_deleter(u8* data)
		{
			free(data);
		}
	}

	struct s_Buffer
	{
	public:
		s_Buffer(u8* data, size_t size, 
			     std::function<u8*(u64)> allocator = default_allocator, 
			     std::function<void(u8*)> deleter = default_deleter)
			: data(data) 
			, size(size)
			, allocator(std::move(allocator))
			, deleter(std::move(deleter)) 
		{

		}

		~s_Buffer()
		{
			if (data)
				deleter(data);
		}

		s_Buffer() = default;

	public:
		s_Buffer& operator=(s_Buffer&& other) noexcept 
		{
			if (this == &other)
				return *this;

			if (data)
				deleter(data);

			data = other.data;
			size = other.size;
			deleter = std::move(other.deleter);
			allocator = std::move(other.allocator);

			other.data = nullptr;
			other.size = 0;

			return *this;
		}

		s_Buffer(s_Buffer&& other) noexcept
			: data(other.data)
			, size(other.size)
			, allocator(std::move(other.allocator))
			, deleter(std::move(other.deleter))
		{
			other.data = nullptr;
			other.size = 0;
		}

	public:
		s_Buffer(const s_Buffer& other)
			: size(other.size)
			, deleter(other.deleter)
		{
			if (!other.data)
				return;

			data = allocator(size);
			if (!data)
				DAT_LOG_CRITICAL("Failed to allocate buffer");

			std::memcpy(data, other.data, size);
		}

		s_Buffer& operator=(const s_Buffer& other)
		{
			if (this == &other)
				return *this;

			if (data)
				deleter(data);

			size = other.size;
			deleter = other.deleter;
			allocator = other.allocator;

			if (!other.data)
				return *this;

			data = allocator(size);
			std::memcpy(data, other.data, size);

			return *this;
		}

	public:
		inline u8 operator[](u16 address) const { return data[address]; }

		inline u8& operator[](u16 address) { return data[address]; }

	public:
		std::function<void(u8*)> deleter = default_deleter;
		std::function<u8*(u64)> allocator = default_allocator;
		u8* data = nullptr;
		u64 size = 0;
	};
}