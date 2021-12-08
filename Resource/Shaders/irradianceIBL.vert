#version 460
/**********************************************************************************
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \author		Tiong Jun Ming, Jerome (j.tiong@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
layout (location = 0) in vec3 position;

out vec3 cubeCoords;

uniform mat4 projection;
uniform mat4 view;


void main()
{
    cubeCoords = position;

    gl_Position =  projection * view * vec4(cubeCoords, 1.0f);
}
