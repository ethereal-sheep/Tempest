#version 460
/**********************************************************************************
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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

