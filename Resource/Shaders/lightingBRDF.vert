#version 460 core
/**********************************************************************************
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \author		Tiong Jun Ming, Jerome (j.tiong@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

layout (location = 4) in vec4 gViewPos;
out vec2 TexCoords;
out vec3 envMapCoords;
out vec4 FragPos;
uniform mat4 inverseView;
uniform mat4 inverseProj;
//uniform mat4 lightSpaceMatrix;

//out vec4 FragPosLightSpace;
out vec3 vs_pos;
void main()
{
    TexCoords = texCoords;
    vec4 unprojCoords = (inverseProj * vec4(position, vec2(1.0f)));
    envMapCoords = (inverseView * unprojCoords).xyz;

	
	//FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
	
    gl_Position = vec4(position, 1.0f);
	vs_pos = position;
	FragPos = gViewPos;
}
