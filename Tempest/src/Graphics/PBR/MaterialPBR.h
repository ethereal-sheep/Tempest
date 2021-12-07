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
#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <Core.h>

#include <glew.h>
#include <Graphics/PBR/TexturePBR.h>
#include <Graphics/OpenGL/Shader.h>

namespace Tempest
{
    class MaterialPBR
    {
    public:
        GLuint matID;
        Shader* matShader;
        std::vector<std::tuple<std::string, TexturePBR>> texList;

        MaterialPBR();
        ~MaterialPBR();
        void addTexture(std::string uniformName, TexturePBR texObj);
        void setShader(const tuptr<Shader>& shader);
        void renderToShader();
    };
}
