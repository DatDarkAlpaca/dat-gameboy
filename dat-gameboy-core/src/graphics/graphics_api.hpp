#pragma once
#include <string>

namespace dat
{
	enum class GraphicsAPI
	{
		UNKNOWN = 0,
		OPENGL,
	};

	inline std::string_view GetGraphicsAPIString(GraphicsAPI api)
	{
		switch (api)
		{
			case GraphicsAPI::OPENGL:
				return "OPENGL";

			default:
				return "UNKNOWN";
		}
	}

	inline GraphicsAPI GetGraphicsAPIFromString(const std::string& graphicsAPI)
	{
		if (graphicsAPI == "OPENGL")
			return GraphicsAPI::OPENGL;

		return GraphicsAPI::UNKNOWN;
	}
}