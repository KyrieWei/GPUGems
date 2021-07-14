#version 330 core

out vec4 fragColor;

in vec2 uv;

uniform sampler2D grass_texture;
uniform sampler2D grass_mask;

void main()
{
    vec3 color = texture(grass_texture, uv).rgb;
    float alpha = texture(grass_mask, uv).r;
    fragColor = vec4(color, alpha);
}