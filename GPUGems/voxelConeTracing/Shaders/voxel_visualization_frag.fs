#version 450 core

#define STEP_LENGTH 0.005f
#define INV_STEP_LENGTH (1.0f / STEP_LENGTH)

in vec2 textureCoord;

out vec4 fragColor;

uniform sampler2D textureFront;
uniform sampler2D textureBack;
uniform sampler3D texture3D;
uniform vec3 cameraPosition;
uniform int state = 0;  //decide mipmap sample level

vec3 scaleAndBias(vec3 p)
{
    return 0.5f * p + vec3(0.5f);
}


bool isInsideCube(vec3 p, float e)
{
    return abs(p.x) < 1 + e && abs(p.y) < 1 + e && abs(p.z) < 1 + e;
}

void main()
{
     float mipmapLevel = state;

     //initialize ray
     vec3 origin = isInsideCube(cameraPosition, 0.2f) ? cameraPosition : texture(textureFront, textureCoord).xyz;
     vec3 direction = texture(textureBack, textureCoord).xyz - origin;
     uint numberOfSteps = uint(INV_STEP_LENGTH * length(direction));
     direction = normalize(direction);

     //trace

     fragColor = vec4(0.0f);
     for(uint step = 0; step < numberOfSteps && fragColor.a < 0.99f; step ++)
     {
        vec3 currentPoint = origin + STEP_LENGTH * step * direction;
        vec4 currentSample = textureLod(texture3D, scaleAndBias(currentPoint), mipmapLevel);
        fragColor += (1.0f - fragColor.a) * currentSample;
     }

     fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2));
}