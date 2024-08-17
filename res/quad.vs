#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_colour;
layout (location = 2) in vec2 a_texcoord;
layout (location = 3) in float a_texindex;

uniform mat4 u_model;
uniform mat4 u_vp_matrix;

out vec4 v_colour;
out vec2 v_texcoord;
out float v_texindex;

void main()
{
    v_colour   = a_colour;
    v_texcoord = a_texcoord;
    v_texindex = a_texindex;
    
	gl_Position = u_vp_matrix * u_model * vec4(a_position, 1.0);
}