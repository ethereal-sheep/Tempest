#version 460
/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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

