#version 330 core
layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_uvs;

out vec2 v_uvs;

uniform mat4 u_model      = mat4(1.0);
uniform mat4 u_projection = mat4(1.0);

void main()
{
	gl_Position = u_projection * u_model * vec4(a_position, 0.0, 1.0);
	v_uvs = a_uvs;
}