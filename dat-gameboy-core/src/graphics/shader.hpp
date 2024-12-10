#pragma once
#include <glad/glad.h>
#include "common_graphics.hpp"

namespace dat
{
	enum class ShaderType
	{
		UNKNOWN = 0,
		VERTEX,
		TESSELATION_CONTROL,
		TESSELATION_EVALUATION,
		GEOMETRY,
		FRAGMENT,
		COMPUTE,
	};

	struct ShaderDescriptor
	{
		std::filesystem::path filepath;
		ShaderType type;
	};

	constexpr inline const char* get_shader_type_name(ShaderType type)
	{
		switch (type)
		{
			case ShaderType::VERTEX:
				return "VERTEX";

			case ShaderType::TESSELATION_CONTROL:
				return "TESSELATION_CONTROL";

			case ShaderType::TESSELATION_EVALUATION:
				return "TESSELATION_EVALUATION";

			case ShaderType::GEOMETRY:
				return "GEOMETRY";

			case ShaderType::FRAGMENT:
				return "FRAGMENT";

			case ShaderType::COMPUTE:
				return "COMPUTE";
		}

		return "UNKNOWN";
	}
}

namespace dat
{
    inline void check_compile_errors(gl_handle shader, ShaderType type)
    {
        constexpr uint64_t BufferSize = 256;

        int success;
        char infoLog[BufferSize];

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, BufferSize, NULL, infoLog);
			DAT_LOG_ERROR("Failed to compile {} Shader: {}", get_shader_type_name(type), infoLog);
        }
    }

    constexpr inline gl_handle get_shader_type(ShaderType type)
    {
        switch (type)
        {
			case ShaderType::VERTEX:
				return GL_VERTEX_SHADER;

			case ShaderType::TESSELATION_CONTROL:
				return GL_TESS_CONTROL_SHADER;

			case ShaderType::TESSELATION_EVALUATION:
				return GL_TESS_EVALUATION_SHADER;

			case ShaderType::GEOMETRY:
				return GL_GEOMETRY_SHADER;

			case ShaderType::FRAGMENT:
				return GL_FRAGMENT_SHADER;

			case ShaderType::COMPUTE:
				return GL_COMPUTE_SHADER;

			default:
				DAT_LOG_CRITICAL("OpenGL does not support the following shader type: {}", (uint32_t)type);
        }

		return 0xFFFFFFFF;
    }

	inline void check_program_link_errors(gl_handle program)
	{
		constexpr uint64_t BufferSize = 256;

		int success;
		char infoLog[BufferSize];

		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, BufferSize, NULL, infoLog);
			DAT_LOG_ERROR("Failed to link shader program: {}", infoLog);
		}
	}
}