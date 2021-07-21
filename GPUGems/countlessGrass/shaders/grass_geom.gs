#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 12) out;

out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float mesh_unit_length_x;
uniform float mesh_unit_length_z;
uniform float grass_width;
uniform float grass_height;

uniform float _Time;
uniform vec2 wind_dir;

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void build_grass(vec4 position)
{    
    mat4 mvp = projection * view * model;

    //grass position offset
    float offset_x = rand(position.xy) * mesh_unit_length_x;
    float offset_z = rand(position.yz) * mesh_unit_length_z;

    float rotate_angle = rand(position.xz) * 3.1415926;
    vec2 grass_dir = vec2(sin(rotate_angle), cos(rotate_angle)) * grass_width;
   
    float grass_height_offset = min(rand(vec2(offset_x, offset_z)), 0.5);
    float grass_height_final = (grass_height_offset + 1) * grass_height;

    position.x += offset_x;
    position.z += offset_z;
    
    //wind direction offset 
    float wind_rotate_angle = rotate_angle * 0.25;

    vec2 random_wind_dir;
    random_wind_dir.x = cos(wind_rotate_angle) * wind_dir.x - sin(wind_rotate_angle) * wind_dir.y;
    random_wind_dir.y = sin(wind_rotate_angle) * wind_dir.x + cos(wind_rotate_angle) * wind_dir.y;

    //bending angle
    float bending_angle = sin(_Time) * 3.1415926 * 0.02;

    //the grass bend along the wind_dir, cos_theta and sin_theta are used to calculate the x,z values in XZ plane after bending

    float cos_theta = random_wind_dir.x / sqrt(random_wind_dir.x*random_wind_dir.x + random_wind_dir.y*random_wind_dir.y);
    float sin_theta = random_wind_dir.y / sqrt(random_wind_dir.x*random_wind_dir.x + random_wind_dir.y*random_wind_dir.y);

    gl_Position = mvp * (position + vec4( -grass_dir.x, 0.0, -grass_dir.y, 0.0));                       //v0
    uv = vec2(0.0, 0.0);
    EmitVertex();   
   
    gl_Position = mvp * (position + vec4(  grass_dir.x, 0.0, grass_dir.y, 0.0));                        //v1
    uv = vec2(1.0, 0.0);
    EmitVertex();
    
    //the new position of point(0, grass_height_final, 0)
    //then plus the point(grass_dir.x, 0, grass_dir.y) or (-grass_dir.x, 0, grass_dir.y), which are the root points of the grass

    float grass_offset_y = grass_height_final * cos(bending_angle);
    float grass_offset_x = grass_height_final * sin(bending_angle) * sin_theta;
    float grass_offset_z = grass_height_final * sin(bending_angle) * cos_theta;

    gl_Position = mvp * (position + vec4( -grass_dir.x + grass_offset_x, grass_offset_y, -grass_dir.y + grass_offset_z, 0.0));   //v2
    uv = vec2(0.0, 0.2);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4(  grass_dir.x + grass_offset_x, grass_offset_y,  grass_dir.y + grass_offset_z, 0.0));   //v3
    uv = vec2(1.0, 0.2);
    EmitVertex();

    //bend angle grows along with the grass height
    grass_offset_y = grass_height_final * 2 * cos(bending_angle * 2);
    grass_offset_x = grass_height_final * 2 * sin(bending_angle * 2) * sin_theta;
    grass_offset_z = grass_height_final * 2 * sin(bending_angle * 2) * cos_theta;
    
    gl_Position = mvp * (position + vec4( -grass_dir.x + grass_offset_x, grass_offset_y, -grass_dir.y + grass_offset_z, 0.0));   //v4
    uv = vec2(0.0, 0.4);
    EmitVertex();   
    
    gl_Position = mvp * (position + vec4(  grass_dir.x + grass_offset_x, grass_offset_y,  grass_dir.y + grass_offset_z, 0.0));   //v5
    uv = vec2(1.0, 0.4);
    EmitVertex();

    grass_offset_y = grass_height_final * 3 * cos(bending_angle * 3);
    grass_offset_x = grass_height_final * 3 * sin(bending_angle * 3) * sin_theta;
    grass_offset_z = grass_height_final * 3 * sin(bending_angle * 3) * cos_theta;
    
    gl_Position = mvp * (position + vec4( -grass_dir.x + grass_offset_x, grass_offset_y, -grass_dir.y + grass_offset_z, 0.0));   //v6
    uv = vec2(0.0, 0.6);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4(  grass_dir.x + grass_offset_x, grass_offset_y,  grass_dir.y + grass_offset_z, 0.0));   //v7
    uv = vec2(1.0, 0.6);
    EmitVertex();

    grass_offset_y = grass_height_final * 4 * cos(bending_angle * 4);
    grass_offset_x = grass_height_final * 4 * sin(bending_angle * 4) * sin_theta;
    grass_offset_z = grass_height_final * 4 * sin(bending_angle * 4) * cos_theta;

    gl_Position = mvp * (position + vec4( -grass_dir.x + grass_offset_x, grass_offset_y, -grass_dir.y + grass_offset_z, 0.0));   //v8
    uv = vec2(0.0, 0.8);
    EmitVertex();   
    
    gl_Position = mvp * (position + vec4(  grass_dir.x + grass_offset_x, grass_offset_y,  grass_dir.y + grass_offset_z, 0.0));   //v9
    uv = vec2(1.0, 0.8);
    EmitVertex();

    grass_offset_y = grass_height_final * 5 * cos(bending_angle * 5);
    grass_offset_x = grass_height_final * 5 * sin(bending_angle * 5) * sin_theta;
    grass_offset_z = grass_height_final * 5 * sin(bending_angle * 5) * cos_theta;
    
    gl_Position = mvp * (position + vec4( -grass_dir.x + grass_offset_x, grass_offset_y, -grass_dir.y + grass_offset_z, 0.0));   //v10
    uv = vec2(0.0, 1.0);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4(  grass_dir.x + grass_offset_x, grass_offset_y,  grass_dir.y + grass_offset_z, 0.0));   //v11
    uv = vec2(1.0, 1.0);
    EmitVertex();
   
    EndPrimitive();
}

void main()
{
    build_grass(gl_in[0].gl_Position);
}