#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

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

vec3 Gerstner(vec3 pos)
{
	vec3 res;
	res.x = pos.x;
	res.z = pos.z;
	res.y = 0;

	vec2 center = vec2(2.0, 2.0);
	vec2 curpos = center - pos.xz;

	for(int i = 0; i < num_func; i ++)
	{
		float amplitude = wave[i].z;
		float wavelength = wave[i].w;
		float omega = 2 * PI / wavelength;
		vec2 direction = normalize(wave[i].xy);
		
		float Qi = Q / (omega * amplitude);
		float fhi = speed[i] * omega;
		
		//compute x z

		float d = sqrt(curpos.x * curpos.x + curpos.y * curpos.y);
		//float prod = Qi * amplitude * cos(d * omega + _Time * fhi);
		//res.x += direction.x * prod;
		//res.z += direction.y * prod;

		float sine = sin(d * omega - _Time * fhi);
		res.y += 2 * amplitude * pow((sine + 1) / 2, k);
	}

	return res;
}


void main()
{
	vec3 p = Gerstner(aPos);
	gl_Position = projection * view * model * vec4(p, 1.0);
	texCoord = aTexCoord;
}