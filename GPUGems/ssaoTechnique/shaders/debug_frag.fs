#version 330 core

out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D test_texture;

void main()
{
	float occlusion = texture(test_texture, TexCoords).r;
	fragColor = vec4(occlusion, occlusion, occlusion, 1.0);
}