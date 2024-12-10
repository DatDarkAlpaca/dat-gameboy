#pragma once
#include <numeric>
#include <limits>
#include <filesystem>

#include "core/defines.hpp"

#include "core/memory/memory.hpp"
#include "core/logger/logger.hpp"
#include "core/assert/assert.hpp"

#define BIND(function, argument) std::bind(function, argument, std::placeholders::_1)

#define EXPOSE_PROPERTY(ClassName, SetterName, PropertyName)						\
	const ClassName& SetterName() const { return PropertyName; }					\
	ClassName& SetterName() { return PropertyName; }

#define KB(x) 1024 * x
#define M(x) x * 1'000'000

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using handle_type = u32;
constexpr inline handle_type invalid_handle = std::numeric_limits<handle_type>::max();