#version 460
/**********************************************************************************
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec4 boneIds;
layout (location = 4) in vec4 weights;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 ModelMatrix;
uniform mat4 finalBonesMatrices[MAX_BONES];
uniform int HasAnimation;



void main()
{
    ivec4 boneids;
    boneids[0] = int(boneIds[0]);
    boneids[1] = int(boneIds[1]);
    boneids[2] = int(boneIds[2]);
    boneids[3] = int(boneIds[3]);

    if(HasAnimation == 1)
    {
        vec4 totalPosition = vec4(0.0f);
        for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
        {
            if(boneids[i] == -1) 
                continue;
            if(boneids[i] >=MAX_BONES) 
            {
                totalPosition = vec4(aPos,1.0f);
                break;
            }
            vec4 localPosition = finalBonesMatrices[boneids[i]] * vec4(aPos,1.0f);
            totalPosition += localPosition * weights[i];
        }    
	    
        gl_Position = ModelMatrix * totalPosition;
    }

    else
    {
        gl_Position = ModelMatrix * vec4(aPos, 1.0);
    }
}