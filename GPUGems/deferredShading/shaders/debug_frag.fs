#version 330 core

out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D test_texture;

void main()
{
	vec3 col = texture(test_texture, TexCoords).rgb;
	//col = normalize(col);
	fragColor = vec4(col, 1.0);
}