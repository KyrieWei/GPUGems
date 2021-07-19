#version 330 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D ground_texture;

void main()
{
    vec3 color = texture(ground_texture, texCoord).rgb;
    fragColor = vec4(color, 1.0);
}