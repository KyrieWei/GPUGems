#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 12) out;

out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float grass_width;
uniform float grass_height;

void build_grass(vec4 position)
{    
    mat4 mvp = projection * view * model;

    gl_Position = mvp * (position + vec4( -grass_width, 0.0, 0.0, 0.0));                //v0
    uv = vec2(0.0, 0.0);
    EmitVertex();   
   
    gl_Position = mvp * (position + vec4(  grass_width, 0.0, 0.0, 0.0));                //v1
    uv = vec2(1.0, 0.0);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4( -grass_width, grass_height, 0.0, 0.0));       //v2
    uv = vec2(0.0, 0.2);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4(  grass_width, grass_height, 0.0, 0.0));       //v3
    uv = vec2(1.0, 0.2);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4( -grass_width, grass_height * 2, 0.0, 0.0));   //v4
    uv = vec2(0.0, 0.4);
    EmitVertex();   
    
    gl_Position = mvp * (position + vec4(  grass_width, grass_height * 2, 0.0, 0.0));   //v5
    uv = vec2(1.0, 0.4);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4( -grass_width, grass_height * 3, 0.0, 0.0));   //v6
    uv = vec2(0.0, 0.6);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4(  grass_width, grass_height * 3, 0.0, 0.0));   //v7
    uv = vec2(1.0, 0.6);
    EmitVertex();

    gl_Position = mvp * (position + vec4( -grass_width, grass_height * 4, 0.0, 0.0));   //v8
    uv = vec2(0.0, 0.8);
    EmitVertex();   
    
    gl_Position = mvp * (position + vec4(  grass_width, grass_height * 4, 0.0, 0.0));   //v9
    uv = vec2(1.0, 0.8);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4( -grass_width, grass_height * 5, 0.0, 0.0));   //v10
    uv = vec2(0.0, 1.0);
    EmitVertex();
    
    gl_Position = mvp * (position + vec4(  grass_width, grass_height * 5, 0.0, 0.0));   //v11
    uv = vec2(1.0, 1.0);
    EmitVertex();
   
    EndPrimitive();
}

void main()
{
    build_grass(gl_in[0].gl_Position);
}