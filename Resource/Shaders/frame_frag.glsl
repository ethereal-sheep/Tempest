#version 460
// Input ----------------------------------------------------------------------
in vec2 TexCoord;

// Uniforms / SSBO ------------------------------------------------------------
uniform sampler2D tex;

// Output ---------------------------------------------------------------------
layout(location = 0) out vec4 FragColor;

void main()
{
	FragColor = texture(tex, TexCoord);
}

