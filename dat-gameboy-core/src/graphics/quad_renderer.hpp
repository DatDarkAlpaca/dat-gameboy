#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics_device.hpp"
#include "core/assets/texture_library.hpp"

namespace dat
{
	// Draws a textured quad.

	class s_QuadRenderer
	{
	public:
		s_QuadRenderer(const std::vector<ShaderDescriptor>& descriptors)
		{
			pipelineProgramID = glCreateProgram();

			std::vector<u32> shaders;
			for (const auto& descriptor : descriptors)
			{
				u32 shader = create_shader(descriptor);
				glAttachShader(pipelineProgramID, shader);
			}

			glLinkProgram(pipelineProgramID);
			
			check_program_link_errors(pipelineProgramID);

			for (const auto& shader : shaders)
				glDeleteShader(shader);

			initialize_objects();
		}

		~s_QuadRenderer()
		{
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
		}

	public:
		void render()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glViewport(0, 0, viewportWidth, viewportHeight);

			glUseProgram(pipelineProgramID);
			set_mvp();

			if (texture != invalid_handle)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture);
			}

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

	public:
		void set_texture(gl_handle handle)
		{
			texture = handle;
		}

		void set_framebuffer(gl_handle framebuffer)
		{
			FBO = framebuffer;
		}

		void set_viewport_size(u32 width, u32 height)
		{
			viewportWidth = width;
			viewportHeight = height;
		}

		void update_buffer(const void* data, u32 size)
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
		}

		void restore_buffer() 
		{
			update_buffer(QuadVertices, 16 * sizeof(float));
		}

	private:
		void initialize_objects()
		{
			constexpr unsigned int QuadIndices[] = { 0, 1, 2, 2, 3, 0  };

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), QuadVertices, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QuadIndices), QuadIndices, GL_STATIC_DRAW);

			{
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
				glEnableVertexAttribArray(1);
			}

			// Pipeline:
			glUseProgram(pipelineProgramID);
			glUniform1i(glGetUniformLocation(pipelineProgramID, "u_texture"), 0);
		}

		void set_mvp()
		{
			// TODO: complete.

			glm::mat4 model(1.0f);
			glm::mat4 projection(1.0f);

			glUniformMatrix4fv(glGetUniformLocation(pipelineProgramID, "u_model"), 1, GL_FALSE, &model[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(pipelineProgramID, "u_projection"), 1, GL_FALSE, &projection[0][0]);
		}

	public:
		u32 viewportWidth = 0;
		u32 viewportHeight = 0;

	private:
		gl_handle pipelineProgramID = invalid_handle;
		gl_handle texture = invalid_handle;
		gl_handle VAO = invalid_handle;
		gl_handle VBO = invalid_handle;
		gl_handle EBO = invalid_handle;
		gl_handle FBO = invalid_handle;

	private:
		constexpr static inline float QuadVertices[] = {
			-0.5f, -0.5f,	0.0f, 0.0f,
			 0.5f, -0.5f,	1.0f, 0.0f,
			 0.5f,  0.5f,	1.0f, 1.0f,
			-0.5f,  0.5f,	0.0f, 1.0f,
		};
	};
}