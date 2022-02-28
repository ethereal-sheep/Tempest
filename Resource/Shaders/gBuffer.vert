#version 460 core
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
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 weights;

out vec3 viewPos;
out vec2 TexCoords;
out vec3 normal;
out vec4 fragPosition;
out vec4 fragPrevPosition;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 projViewModel;
uniform mat4 prevProjViewModel;

// Animation Stuff
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 finalBonesMatrices[MAX_BONES];
uniform int HasAnimation;


void main()
{
    if(HasAnimation == 1)
    {
        mat3 normalMatrix = transpose(inverse(mat3(projViewModel)));
        normal = normalMatrix * Normal;

        vec4 totalPosition = vec4(0.0f);
        for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
        {
            if(boneIds[i] == -1) 
                continue;
            if(boneIds[i] >= MAX_BONES) 
            {
                totalPosition = vec4(position,1.0f);
                break;
            }
            vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(position,1.0f);
            totalPosition += localPosition * weights[i];
            vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * normal;
        }
	    
        mat4 viewModel = view * projViewModel;
        viewPos = (viewModel * vec4(position, 1.0f)).xyz;
        TexCoords = texCoords;
        
        fragPosition = projViewModel * vec4(position, 1.0f);
        fragPrevPosition = prevProjViewModel * vec4(position, 1.0f);
        
        gl_Position =  projection * viewModel * totalPosition;
    }

    else
    {
        // View Space
        vec4 viewFragPos = view * projViewModel * vec4(position, 1.0f);
        viewPos = viewFragPos.xyz;

        TexCoords = texCoords;

        mat3 normalMatrix = transpose(inverse(mat3(projViewModel)));
        normal = normalMatrix * Normal;

        fragPosition = projViewModel * vec4(position, 1.0f);
        fragPrevPosition = prevProjViewModel * vec4(position, 1.0f);

        gl_Position = projection * viewFragPos;
    }
    


    // World Space
//    worldPos = vec3(model * vec4(position, 1.0f));
//    normal = mat3(transpose(inverse(model))) * Normal;
//    TexCoords = texCoords;

//    gl_Position = projection * view * model * vec4(position, 1.0f);
}
