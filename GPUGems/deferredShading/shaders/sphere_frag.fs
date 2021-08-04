#version 330 core
#define light_num 500

struct PointLight
{
	vec3 position;
	vec3 color;
};

out vec4 FragColor;

in vec2 TexCoord;

uniform PointLight pointLights[light_num];
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;

uniform vec3 viewPos;
uniform float exposure;

void main()
{
	vec3 FragPos = texture(gPosition, TexCoord).rgb;
	vec3 Normal = texture(gNormal, TexCoord).rgb;
	vec3 color = texture(gColor, TexCoord).rgb;

	//ambient
	vec3 ambient = 0.0 * color;	

	//lighting
	vec3 lighting = vec3(0.0);
	vec3 viewDir = normalize(viewPos - FragPos);

	for(int i = 0; i < light_num; i ++)
	{
		//diffuse
		vec3 lightDir = normalize(pointLights[i].position - FragPos);
		float diff = max(dot(lightDir, Normal), 0.0);
		vec3 result = pointLights[i].color * diff * color;

		//attenuation
		float distance = length(FragPos - pointLights[i].position);
		result *= 1.0 / (distance * distance);
		lighting += result;
	}

	vec3 result = ambient + lighting;

	//tone mapping
	vec3 col = vec3(1.0) - exp(- result * exposure);
	col = pow(col, vec3(0.45));

	FragColor = vec4(col, 1.0);
}