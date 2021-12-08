#version 460
/**********************************************************************************
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;


void main()
{
    gl_Position = vec4(position, 1.0f);
    TexCoords = texCoords;
}
