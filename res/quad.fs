#version 330 core

layout (location = 0) out vec4 o_colour;

in vec4 v_colour;
in vec2 v_texcoord;
in float v_texindex;

uniform sampler2D u_textures[32];

void main()
{
    int i = int(v_texindex);
    o_colour = v_colour;
}