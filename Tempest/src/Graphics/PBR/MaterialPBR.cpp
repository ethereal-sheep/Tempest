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
#include "MaterialPBR.h"

namespace Tempest
{
    MaterialPBR::MaterialPBR()
    {

    }


    MaterialPBR::~MaterialPBR()
    {

    }


    void MaterialPBR::addTexture(std::string uniformName, TexturePBR texObj)
    {
        this->texList.push_back(std::tuple<std::string, TexturePBR>(uniformName, texObj));
    }


    void MaterialPBR::setShader(const tuptr<Shader>& shader)
    {
        this->matShader = shader.get();
    }


    void MaterialPBR::renderToShader()
    {
        this->matShader->Bind();

        std::cout << "texList Size : " << this->texList.size() << std::endl;

        for (GLuint i = 0; i < this->texList.size(); i++)
        {
            std::string currentUniformName = std::get<0>(this->texList[i]);
            TexturePBR currentTex = std::get<1>(this->texList[i]);

            std::cout << "i : " << i << std::endl;
            std::cout << "texWidth : " << currentTex.getTexWidth() << std::endl;
            std::cout << "texHeight : " << currentTex.getTexHeight() << std::endl;
            std::cout << "texUniformName : " << currentTex.getTexName() << std::endl;
            std::cout << "ActiveTexture sent : " << GL_TEXTURE0 + i << std::endl;

            glActiveTexture(GL_TEXTURE0 + i);
            currentTex.useTexture();
            matShader->Set1i(i, currentUniformName.c_str());
            std::cout << "------" << std::endl;
        }

        std::cout << "============" << std::endl;
    }

}