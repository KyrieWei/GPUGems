#version 330 core

#define DIRECT_LIGHT_INTENSITY 0.96f

#define DIST_FACTOR 1.1f
#define CONSTANT 1
#define LINEAR 0
#define QUADRATIC 1

#define GAMMA_CORRECTION 1



struct PointLight
{
    vec3 position;
    vec3 color;
};

struct Material
{
    vec3 diffuseColor;
    vec3 specularColor;
};

struct Settings
{
    bool indirectSpecularLight;
    bool indirectDiffuseLight;
    bool directLight;
    bool shadows;
};

uniform Material material;
uniform Settings settings;
uniform vec3 cameraPosition;

in vec3 worldPositionFrag;
in vec3 normalFrag;

out vec4 fragColor;

float attenuate(float dist)
{
    dist *= DIST_FACTOR;
    return 1.0f / (CONSTANT + LINEAR * dist + QUADRATIC * dist * dist);
}

vec3 calculateDirectLight(const PointLight light, const vec3 viewDirection)
{
    vec3 lightDirection = light.position - worldPositionFrag;
    const float distanceToLight = length(lightDirection);
    lightDirection = normalize(lightDirection);
    const float lightAngle = dot(normalFrag, lightDirection);
    
    //diffuse lighting
    float diffuseAngle = max(lightAngle, 0.0f);

    const float diffuse = diffuseAngle;

    const vec3 diff = material.diffuseColor * diffuse;

    const vec3 total = diff;

    return attenuate(distanceToLight) * total;
}

vec3 directLight(vec3 viewDirection)
{
    vec3 direct = vec3(0.0f);
    const uint maxLights = min(numberOfLights, MAX_LIGHTS);
    for(uint i = 0; i < maxLights; i ++)
        direct += calculateDirectLight(pointLights[i], viewDirection);

   direct *= DIRECT_LIGHT_INTENSITY;
   return direct;
}

void main()
{
    color = vec4(0, 0, 0, 1);
    const vec3 viewDirection = normalize(worldPosFrag - cameraPosition);
    
    //Direct light
    if(settings.directLight)
        color.rgb += directLight(viewDirection);

    fragColor = vec4(material.diffuseColor, 1.0);
}