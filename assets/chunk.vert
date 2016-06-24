#version 330 core

layout (location = 0) in int position;

uniform mat4 MVP;

out vec2 TexCoord;

void main()
{
    vec3 pos = vec3(position & 0x1F, (position >> 5) & 0x1F, (position >> 10) & 0x1F);
    TexCoord = vec2((position >> 15) & 0x1F, (position >> 20) & 0x1F);

    gl_Position = MVP * vec4(pos.xyz, 1.0f);
}
