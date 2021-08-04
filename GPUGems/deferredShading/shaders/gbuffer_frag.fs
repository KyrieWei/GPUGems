#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;


uniform sampler2D diffuseTexture;

void main()
{
	gPosition = FragPos;
	gNormal = Normal;
	gColor = vec4(texture(diffuseTexture, TexCoord).rgb, 1.0);
}