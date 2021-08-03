#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;

void main()
{
	vec3 normal = normalize(Normal);
	FragColor = vec4(normal, 1.0);
}