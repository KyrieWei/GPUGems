#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D wood;

void main()
{
	vec3 color = texture(wood, TexCoord).rgb;
	FragColor = vec4(color, 1.0);
}