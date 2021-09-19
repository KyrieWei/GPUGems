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

in vec3 worldPositionFrag;
in vec3 normalFrag;

out vec4 fragColor;

uniform PointLight pointLights[MAX_LIGHTS];
uniform int numberOfLights;

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

void main()
{
    vec3 color = vec3(0.0f);
    if(!isInsideCube(worldPositionFrag, 0))
        return;

     int maxLights = min(numberOfLights, MAX_LIGHTS);
     for(int i = 0; i < maxLights; i ++)
        color += calculatePointLight(pointLights[i]);

    fragColor = vec4(color, 1.0);
}