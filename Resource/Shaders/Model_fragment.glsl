#version 460

in vec3 vs_color;
in vec2 vs_tex;

out vec4 fs_color;

void main()
{
	fs_color = vec4(vs_color, 1.0);
}