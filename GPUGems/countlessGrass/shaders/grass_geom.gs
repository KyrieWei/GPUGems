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

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void build_grass(vec4 position)
{    
    mat4 mvp = projection * view * model;

    float offset_x = rand(position.xy) * mesh_unit_length_x;
    float offset_z = rand(position.yz) * mesh_unit_length_z;

    float rotate_angle = rand(position.xz) * 3.1415926;
    vec2 grass_dir = vec2(sin(rotate_angle), cos(rotate_angle)) * grass_width;
    
    float grass_height_offset = min(rand(vec2(offset_x, offset_z)), 0.3);
    float grass_height_final = (grass_height_offset + 1) * grass_height;

    position.x += offset_x;
    position.z += offset_z;

    gl_Position = mvp * (position + vec4( -grass_dir.x, 0.0, -grass_dir.y, 0.0));                //v0
    uv = vec2(0.0, 0.0);
    EmitVertex();   
   
    gl_Position = mvp * (position + vec4(  grass_dir.x, 0.0, grass_dir.y, 0.0));                //v1
    uv = vec2(1.0, 0.0);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4( -grass_dir.x, grass_height_final, -grass_dir.y, 0.0));       //v2
    uv = vec2(0.0, 0.2);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4(  grass_dir.x, grass_height_final, grass_dir.y, 0.0));       //v3
    uv = vec2(1.0, 0.2);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4( -grass_dir.x, grass_height_final * 2, -grass_dir.y, 0.0));   //v4
    uv = vec2(0.0, 0.4);
    EmitVertex();   
    
    gl_Position = mvp * (position + vec4(  grass_dir.x, grass_height_final * 2, grass_dir.y, 0.0));   //v5
    uv = vec2(1.0, 0.4);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4( -grass_dir.x, grass_height_final * 3, -grass_dir.y, 0.0));   //v6
    uv = vec2(0.0, 0.6);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4(  grass_dir.x, grass_height_final * 3, grass_dir.y, 0.0));   //v7
    uv = vec2(1.0, 0.6);
    EmitVertex();

    gl_Position = mvp * (position + vec4( -grass_dir.x, grass_height_final * 4, -grass_dir.y, 0.0));   //v8
    uv = vec2(0.0, 0.8);
    EmitVertex();   
    
    gl_Position = mvp * (position + vec4(  grass_dir.x, grass_height_final * 4, grass_dir.y, 0.0));   //v9
    uv = vec2(1.0, 0.8);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4( -grass_dir.x, grass_height_final * 5, -grass_dir.y, 0.0));   //v10
    uv = vec2(0.0, 1.0);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4(  grass_dir.x, grass_height_final * 5, grass_dir.y, 0.0));   //v11
    uv = vec2(1.0, 1.0);
    EmitVertex();
   
    EndPrimitive();
}

void main()
{
    build_grass(gl_in[0].gl_Position);
}