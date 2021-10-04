#version 460

in vec3 vs_color;
in vec2 vs_tex;

out vec4 fs_color;

uniform sampler2D texture0;
uniform vec3 DiffuseColour;

void main()
{
	fs_color = vec4(DiffuseColour * vec3(texture(texture0, vs_tex)), 1.0);
}