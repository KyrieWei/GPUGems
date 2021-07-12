#version 330 core

out vec4 fragColor;

in vec3 water_vertex_normal;

uniform sampler2D sunTexture;
uniform vec3 lightColor;

vec3 refracted_light_dir(vec3 normal)
{
    vec3 indir = vec3(0.0, 1.0, 0.0);

    float IOR = 1.3333;

    float c = dot(indir, normal);
    float sq = 1 + (IOR * IOR) * (c * c - 1);
    
    if(sq > 0)
        sq = sqrt(sq);
    else
        sq = 0;

    vec3 trans = normalize(indir * IOR + normal * (IOR * c - sq));
    
    return trans;
}

void main()
{

    vec3 refracted_dir = refracted_light_dir(water_vertex_normal);

    vec2 texCoord = (refracted_dir.xz + 1.0) / 2;

    float alpha = texture(sunTexture, texCoord).r;
    fragColor = vec4(lightColor, alpha);
}