#version 450 core

#define MAX_LIGHTS 1
#define POINT_LIGHT_INTENSITY 1

#define DIST_FACTOR 1.1f
#define CONSTANT 1
#define LINEAR 0
#define QUADRATIC 1

struct PointLight
{
    vec3 position;
    vec3 color;
};

out vec4 fragColor;

uniform PointLight pointLights[MAX_LIGHTS];
uniform int numberOfLights;

layout(RGBA8) uniform image3D texture3D;

in vec3 worldPositionFrag;
in vec3 normalFrag;

float attenuate(float dist)
{
    dist *= DIST_FACTOR;
    return 1.0f / (CONSTANT + LINEAR * dist + QUADRATIC * dist * dist);
}

vec3 calculatePointLight(PointLight light)
{
    vec3 direction = normalize(light.position - worldPositionFrag);
    float distanceToLight = distance(light.position, worldPositionFrag);
    float attenuation = attenuate(distanceToLight);
    float d = max(dot(normalize(normalFrag), direction), 0.0f);
    return d * POINT_LIGHT_INTENSITY * attenuation * light.color;
}


bool isInsideCube(vec3 p, float e)
{
    return abs(p.x) < 1 + e && abs(p.y) < 1 + e && abs(p.z) < 1 + e;
}

vec3 scaleAndBias(vec3 p) 
{
    return 0.5f * p + vec3(0.5f);
}

void main()
{
    vec3 color = vec3(0.0f);
    if(!isInsideCube(worldPositionFrag, 0))
        return;

    int maxLights = min(numberOfLights, MAX_LIGHTS);
    for(int i = 0; i < maxLights; i ++)
       color += calculatePointLight(pointLights[i]);

    vec3 voxel = scaleAndBias(worldPositionFrag);
    ivec3 dim = imageSize(texture3D);
    vec4 res = vec4(0.2, 0.3, 0.5, 1.0);
    imageStore(texture3D, ivec3(dim * voxel), res);
}