#version 440 core

in vec3 position;
in vec3 normal;


out vec3 Normal;
out vec3 FragPos;


uniform mat4 u_MPV;


void main()
{
	gl_Position = u_MPV * vec4(position, 1.0f);

	Normal = mat3(transpose(inverse(u_MPV))) * normal;

	FragPos = (u_MPV * vec4(position, 1.0f)).xyz;
};