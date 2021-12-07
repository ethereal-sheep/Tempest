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
layout(location = 0) in vec3 vertex_position;

layout(location = 5)  in mat4 model;
uniform mat4 lightSpaceMatrix;
//uniform mat4 model;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(vertex_position, 1.0);
   // gl_Position =  ProjectionMatrix * ViewMatrix * model * vec4(vertex_position, 1.f);
}

