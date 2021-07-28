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

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos)
{
	vec3 lightDir = normalize(light.Position - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);

	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	//specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	
	//attenuation
	float distance = length(light.Position - fragPos);
	float attenuation = 1.0 / (1.0  + 0.09 * distance + 0.032 * distance * distance);

	vec3 ambient = 0.3 * light.Color;
	vec3 diffuse = diff * light.Color;
	vec3 specular = spec * light.Color;

	return ambient + diffuse + specular;
}

void main()
{
	vec3 normal = normalize(fs_in.Normal) * inverse_normals;

	vec3 wood_color = texture(wood, fs_in.TexCoord).rgb;

	vec3 light = vec3(0);

	for(int i = 0; i < light_num; i ++)
		light += CalcSpotLight(lights[i], normal, fs_in.FragPos);

	FragColor = vec4(light * wood_color, 1.0);
}