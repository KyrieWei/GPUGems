#version 450 core


//light settings
#define DIRECT_LIGHT_INTENSITY 0.96f
#define MAX_LIGHTS 1

//lighting attenuation factors
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
    float emissivity;
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
uniform PointLight pointLights[MAX_LIGHTS];
uniform vec3 cameraPosition;
uniform int numberOfLights;

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
    float distanceToLight = length(lightDirection);
    lightDirection = normalize(lightDirection);
    float lightAngle = dot(normalFrag, lightDirection);
    
    //diffuse lighting
    float diffuseAngle = max(lightAngle, 0.0f);

    float diffuse = diffuseAngle;

    vec3 diff = material.diffuseColor * diffuse;

    vec3 total = diff;

    vec3 dirLight = total * attenuate(distanceToLight);

    return dirLight;
}

vec3 directLight(vec3 viewDirection)
{
    vec3 direct = vec3(0.0f);
    int maxLights = min(numberOfLights, MAX_LIGHTS);
    for(int i = 0; i < maxLights; i ++)
        direct += calculateDirectLight(pointLights[i], viewDirection);

   direct *= DIRECT_LIGHT_INTENSITY;
   return direct;
}

void main()
{
    vec4 color = vec4(0, 0, 0, 1);
    const vec3 viewDirection = normalize(worldPositionFrag - cameraPosition);

    //Emissivity
    color.rgb += material.emissivity * material.diffuseColor;
    
    //Direct light
    if(settings.directLight)
        color.rgb += directLight(viewDirection);

    fragColor = color;
}