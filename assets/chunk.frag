#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D text;

void main()
{
    vec4 tex_color = texture(text, TexCoord/16.0f);
    color = tex_color;//vec4(tex_color.xyz * (3.0f/5 + 2*(ambient/5)), tex_color.w);
}
