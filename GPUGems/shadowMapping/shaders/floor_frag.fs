#version 330 core

out vec4 fragColor;

uniform sampler2D floor_texture;

in vec2 texCoord;

void main()
{
	fragColor = texture(floor_texture, texCoord);
}