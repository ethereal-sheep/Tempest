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
layout(location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 weights;


uniform mat4 lightSpaceMatrix;

uniform int meshDrawing;
uniform mat4 ModelMatrix;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 finalBonesMatrices[MAX_BONES];
uniform int HasAnimation;
void main()
{
    if(HasAnimation == 1)
    {
        vec4 totalPosition = vec4(0.0f);
        for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
        {
            if(boneIds[i] == -1) 
                continue;
            if(boneIds[i] >=MAX_BONES) 
            {
                totalPosition = vec4(vertex_position,1.0f);
                break;
            }
            vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(vertex_position,1.0f);
            totalPosition += localPosition * weights[i];
        }    
	    
        gl_Position =  lightSpaceMatrix * ModelMatrix * totalPosition;
    }

    else
    {
        gl_Position = lightSpaceMatrix * ModelMatrix * vec4(vertex_position, 1.0f);
    }
        
} 


