#version 460

in vec2 vs_tex;

out vec4 fs_color;

uniform vec3 DiffuseColour;

void main()
{
	fs_color = vec4(DiffuseColour, 1.0);
}