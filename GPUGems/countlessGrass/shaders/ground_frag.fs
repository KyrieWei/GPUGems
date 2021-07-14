#version 330 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D grass_mask;

void main()
{
    float alpha = texture(grass_mask, texCoord).r;

    fragColor = vec4(1.0, 1.0, 1.0, alpha);
}