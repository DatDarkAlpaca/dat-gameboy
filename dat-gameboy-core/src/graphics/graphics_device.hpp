#pragma once
#include <glad/glad.h>

#include "common_graphics.hpp"
#include "shader.hpp"
#include "utils/file.hpp"

namespace dat
{
	// Shader:
	inline u32 create_shader(const ShaderDescriptor& descriptor)
	{
		std::string contents = read_file(descriptor.filepath);
		const char* cString = contents.c_str();

		uint32_t shaderID = glCreateShader(get_shader_type(descriptor.type));
		glShaderSource(shaderID, 1, &cString, NULL);
		glCompileShader(shaderID);
		check_compile_errors(shaderID, descriptor.type);

		return shaderID;
	}

	// Texture:
	struct TextureDescriptor
	{
		u32 width = 0;
		u32 height = 0;

		u32 internalFormat = GL_RGBA;
		
		u32 dataFormat = GL_RGBA;
		u32 dataType = GL_UNSIGNED_BYTE;

		bool generateMipmaps = false;

		u32 wrapS = GL_REPEAT;
		u32 wrapT = GL_REPEAT;

		u32 minFilter = GL_NEAREST;
		u32 magFilter = GL_NEAREST;
	};

	inline gl_handle create_texture(const TextureDescriptor& descriptor, const void* data = nullptr)
	{
		gl_handle texture;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, descriptor.wrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, descriptor.wrapT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, descriptor.minFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, descriptor.magFilter);
		}

		glTexImage2D(
			GL_TEXTURE_2D, 0, 
			descriptor.internalFormat, 
			descriptor.width, descriptor.height, 0, 
			descriptor.internalFormat, descriptor.dataType, 
			data
		);
		
		if(descriptor.generateMipmaps)
			glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		return texture;
	}

	inline void update_texture(gl_handle texture, u32 width, u32 height, u32 dataFormat, u32 dataType, const void* data)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, dataFormat, dataType, data);
	}

	inline void delete_texture(handle_type texture)
	{
		if (texture != invalid_handle)
			glDeleteTextures(1, &texture);

		DAT_LOG_ERROR("Attempted to delete invalid texture");
	}

	// Framebuffer:
	inline handle_type create_default_framebuffer(handle_type colorTexture, u32 width, u32 height)
	{
		handle_type FBO;

		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

		handle_type RBO;

		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			DAT_LOG_ERROR("Incomplete framebuffer");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		return FBO;
	}
}