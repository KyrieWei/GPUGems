#version 330 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D waterSurfaceTexture;

void main()
{
    fragColor = texture(waterSurfaceTexture, texCoord);
}