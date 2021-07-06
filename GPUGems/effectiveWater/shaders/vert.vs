#version 330 core

#define PI 3.1415926538
#define num_func 3


layout (location = 0) in vec3 aPos;

layout (std140) uniform params
{
	vec4 wave[num_func];
	float speed[num_func];
};



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float _Time;
uniform float k;
uniform float Q;

vec3 Gerstner(vec3 pos)
{
	vec3 res;
	res.x = pos.x;
	res.z = pos.z;
	res.y = 0;

	for(int i = 0; i < num_func; i ++)
	{
		float amplitude = wave[i].z;
		float wavelength = wave[i].w;
		float omega = 2 * PI / wavelength;
		vec2 direction = normalize(wave[i].xy);
		
		float Qi = Q / (omega * amplitude);
		float fhi = speed[i] * omega;
		
		//compute x z
		float prod = Qi * amplitude * cos(dot(direction, pos.xz) * omega + _Time * fhi);
		res.x += direction.x * prod;
		res.z += direction.y * prod;

		float sine = sin(dot(direction, pos.xz) * omega + _Time * fhi);
		res.y += amplitude * sine;
		//res.y += 2 * amplitude * pow((sine + 1) / 2, k);
	}

	return res;
}

void main()
{
	vec3 p = Gerstner(aPos);
	
	gl_Position = projection * view * model * vec4(p, 1.0);
}