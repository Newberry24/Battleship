#version 400 core

in vec4 color;
in vec2 tex_coord;

uniform sampler2D tex1;
uniform bool useTexture;

out vec4 fColor;

void main()
{
    if (useTexture)
        fColor = texture(tex1, tex_coord);
    else
        fColor = color;
}

