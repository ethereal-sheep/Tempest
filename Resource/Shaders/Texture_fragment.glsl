#version 460

layout(location = 0) in vec3 vs_color;
layout(location = 1) in vec2 vs_tex;

layout(location = 0) out vec4 fs_color;

uniform sampler2D texture0;

void main()
{
	fs_color = texture(texture0, vs_tex);
}