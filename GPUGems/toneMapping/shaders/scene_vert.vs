#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool inverse_normal;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} vs_out;

void main()
{
	vec3 n = inverse_normal ? -aNormal : aNormal;
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vs_out.Normal  = normalize(normalMatrix * n);

	vs_out.TexCoord = aTexCoord;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}