#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Core.h"

#include "TexturePBR.h"
#include <Graphics/OpenGL/Shader.h>
//#include "shape.h"

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