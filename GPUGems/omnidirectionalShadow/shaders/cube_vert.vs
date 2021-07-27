#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 Normal;

void main()
{
	Normal = aNormal;
	TexCoord = aTexcoord;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}