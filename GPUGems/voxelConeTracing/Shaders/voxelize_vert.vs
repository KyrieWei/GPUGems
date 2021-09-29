#version 450 core

layout(location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 view;
uniform mat4 project;

void main()
{
	FragPos = aPos;
	gl_Position = project * view * vec4(aPos, 1.0);
}