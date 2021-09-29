#version 450 core

layout (location = 0) in vec3 aPos;

out vec2 textureCoord;

vec2 scaleAndBias(vec2 p)
{
	return 0.5f * p + vec2(0.5f);
}

void main()
{
	textureCoord = scaleAndBias(aPos.xy);
	gl_Position = vec4(aPos, 1);
}