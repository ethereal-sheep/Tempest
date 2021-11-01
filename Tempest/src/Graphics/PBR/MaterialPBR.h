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
