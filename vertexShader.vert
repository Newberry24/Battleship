#version 400 core

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 vcolor;
layout(location = 2) in vec3 vnormal;
layout(location = 3) in vec2 in_tex_coord;

uniform mat4 Projection;

out vec4 color;
out vec2 tex_coord;

void main()
{
    color = vcolor;
    gl_Position = Projection * vposition;
    tex_coord = in_tex_coord;
}
