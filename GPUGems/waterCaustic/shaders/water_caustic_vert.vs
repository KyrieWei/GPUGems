#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 water_vertex_normal;

uniform float _Time;
uniform float k;
uniform float Q;

#define PI 3.1415926538
#define num_func 1

layout (std140) uniform params
{
	vec4 wave[num_func];
	float speed[num_func];
};

vec3 Gerstner_normal(vec3 pos)
{
	vec3 res = vec3(0.0, 1.0, 0.0);
	
	vec2 center = vec2(2.0, 2.0);
	vec2 curpos = pos.xz - center;

	for(int i = 0; i < num_func; i ++)
	{
		float amplitude = wave[i].z;
		float wavelength = wave[i].w;
		float omega = 2 * PI / wavelength;
		
		float phi = speed[i] * omega;
		float d = sqrt(curpos.x * curpos.x + curpos.y * curpos.y);

		res.x += 2 * amplitude * k * pow(sin(d * omega - _Time * phi) / 2, k-1) * cos(d * omega - _Time * phi) * omega * curpos.x / d;
		res.z += 2 * amplitude * k * pow(sin(d * omega - _Time * phi) / 2, k-1) * cos(d * omega - _Time * phi) * omega * curpos.y / d;
	}

	res = normalize(res);

	return res;
}


void main()
{
	water_vertex_normal = Gerstner_normal(aPos);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}