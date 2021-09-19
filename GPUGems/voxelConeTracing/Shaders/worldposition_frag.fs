#version 450 core

in vec3 worldPosition;

out vec4 fragColor;

void main()
{
    fragColor.rgb = worldPosition;
}