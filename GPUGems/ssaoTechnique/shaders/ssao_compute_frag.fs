#version 330 core

out float FragColor;

in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

const vec2 noiseScale = vec2(1280.0 / 4.0, 720.0 / 4.0);

uniform mat4 projection;

void main()
{
	vec3 fragPos = texture(gPosition, TexCoord).xyz;
	vec3 normal = normalize(texture(gNormal, TexCoord).rgb);
	vec3 randomVec = normalize(texture(texNoise, TexCoord * noiseScale).xyz);

	//create TBN change-of-biasis matrix: from tangent space to view space
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal); 
	
	float occlusion = 0.0;
	for(int i = 0; i < kernelSize; i ++)
	{
		//get sample position
		vec3 samplePos = TBN * samples[i];
		samplePos = fragPos + samplePos * radius;

		//project sample position to sample texture to get position on screen/texture
		vec4 offset = vec4(samplePos, 1.0);
		offset = projection * offset;
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;

		//get sample depth
		float sampleDepth = texture(gPosition, offset.xy).z;

		//range check & accumulate
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
	}

	occlusion = 1.0 - (occlusion / kernelSize);

	FragColor = occlusion;
}