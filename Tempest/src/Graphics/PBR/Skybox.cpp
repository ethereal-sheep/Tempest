/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "Skybox.h"


namespace Tempest
{

    Skybox::Skybox()
    {

    }


    Skybox::~Skybox()
    {

    }


    void Skybox::setSkyboxTexture(const char* texPath)
    {
        this->texSkybox.setTextureHDR(texPath, "cubemapHDR", true);
    }


    void Skybox::renderToShader(const tuptr<Shader>& shaderSkybox, glm::mat4& projection, glm::mat4& view)
    {
        shaderSkybox->Bind();
        glActiveTexture(GL_TEXTURE0);
        this->texSkybox.useTexture();
        shaderSkybox->Set1i(0, "envMap");
        shaderSkybox->SetMat4fv(glm::transpose(view), "inverseView");
        shaderSkybox->SetMat4fv(glm::inverse(projection), "inverseProj");
        shaderSkybox->Set1f(cameraAperture, "cameraAperture");
        shaderSkybox->Set1f(cameraShutterSpeed, "cameraShutterSpeed");
        shaderSkybox->Set1f(cameraISO, "cameraISO");


    }


    void Skybox::setExposure(GLfloat aperture, GLfloat shutterSpeed, GLfloat iso)
    {
        this->cameraAperture = aperture;
        this->cameraShutterSpeed = shutterSpeed;
        this->cameraISO = iso;
    }

}