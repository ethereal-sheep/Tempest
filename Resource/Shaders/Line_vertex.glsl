
// Input ----------------------------------------------------------------------
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;		// per instance

// Uniforms / SSBO ------------------------------------------------------------
uniform mat4 viewProjMatrix;

// Output ---------------------------------------------------------------------
out vec4 fragmentColor;

// Functions ------------------------------------------------------------------
void main()
{
	gl_Position = viewProjMatrix * vec4(position, 1.f);
	fragmentColor = color;
}

