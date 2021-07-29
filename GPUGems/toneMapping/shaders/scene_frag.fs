#version 330 core
#define light_num 4

struct SpotLight
{
	vec3 Position;
	vec3 Color;
};

out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} fs_in;

uniform sampler2D wood;

uniform SpotLight lights[light_num];
uniform vec3 viewPos;
uniform int inverse_normals;

void main()
{
	vec3 normal = normalize(fs_in.Normal);

	vec3 wood_color = texture(wood, fs_in.TexCoord).rgb;

	vec3 ambient = 0.0 * wood_color;

	vec3 light = vec3(0.0);

	for(int i = 0; i < light_num; i ++)
	{
		vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);
		float diff = max(dot(lightDir, normal), 0.0);
		vec3 diffuse = lights[i].Color * diff * wood_color;

		vec3 result = diffuse;

		float distance = length(fs_in.FragPos - lights[i].Position);
		result *= 1.0 / (distance * distance);

		light += result;
	}

	FragColor = vec4(ambient + light, 1.0);
}