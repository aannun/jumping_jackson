#version 410 core

in vec2 uvs;

out vec4 final_color;
uniform sampler2D tex_base_color;

void main()
{
    vec4 color = texture(tex_base_color, uvs).xyzw;
    final_color = color;
}