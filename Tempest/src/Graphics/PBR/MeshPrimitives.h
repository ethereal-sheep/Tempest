/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "Graphics/Basics/GeometryFactory.h"
#include "Util.h"

namespace Tempest
{
    struct mVertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    class MeshPrim 
    {
    public:

        MeshPrim() = default;
        MeshPrim(const std::pair<Vertices, Indices>& data);
        ~MeshPrim();

        void Draw();

    private:
        GLuint VAO, VBO, EBO;
        tvector<mVertex> vertices;
        tvector<uint32_t> indices;

        void setupMesh();
    };
}