#pragma once
#include <numeric>
#include "GraphicsAPI.hpp"

namespace dat::graphics
{
	struct GraphicsSpecification
	{
		GraphicsAPI api = GraphicsAPI::UNKNOWN;
		uint8_t major = 0, minor = 0, patch = 0, variant = 0;
	};
}