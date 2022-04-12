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
#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "TMath.h"
#include "Core.h"

#include "TexturePBR.h"
#include <Graphics/OpenGL/Shader.h>

namespace Tempest
{
    class Skybox
    {
    public:
        GLfloat cameraAperture, cameraShutterSpeed, cameraISO;
        TexturePBR texSkybox;

        Skybox();
        ~Skybox();
        void setSkyboxTexture(const char* texPath);
        void renderToShader(const tuptr<Shader>& shader, glm::mat4& projection, glm::mat4& view);
        void setExposure(GLfloat aperture, GLfloat shutterSpeed, GLfloat iso);
    };

}