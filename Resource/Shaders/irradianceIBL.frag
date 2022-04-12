#version 460
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
in vec3 cubeCoords;
out vec4 colorOutput;


const float PI = 3.14159265359f;

uniform samplerCube envMap;


void main()
{		
    vec3 N = normalize(cubeCoords);
    vec3 irradianceAccumulation = vec3(0.0f);
    
    vec3 upDir = vec3(0.0f, 1.0f, 0.0f);
    vec3 rightDir = cross(upDir, N);
    upDir = cross(N, rightDir);

    float sampleOffset = 0.025f;
    float sampleCount = 0.0f;

    for(float anglePhi = 0.0f; anglePhi < 2.0f * PI; anglePhi += sampleOffset)
    {
        for(float angleTheta = 0.0f; angleTheta < 0.5f * PI; angleTheta += sampleOffset)
        {
            vec3 sampleTangent = vec3(sin(angleTheta) * cos(anglePhi),  sin(angleTheta) * sin(anglePhi), cos(angleTheta));
            vec3 sampleVector = sampleTangent.x * rightDir + sampleTangent.y * upDir + sampleTangent.z * N;

            irradianceAccumulation += texture(envMap, sampleVector).rgb * cos(angleTheta) * sin(angleTheta);
            sampleCount++;
        }
    }

    irradianceAccumulation = irradianceAccumulation * (1.0f / float(sampleCount)) * PI;
    
    colorOutput = vec4(irradianceAccumulation, 1.0f);
}
