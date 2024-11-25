#pragma once
#include "common.hpp"

namespace dat
{
	using asset_handle = u32;
	constexpr inline asset_handle invalid_asset = std::numeric_limits<asset_handle>::max();
}