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


#include "Core.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "glew.h"

namespace Tempest
{

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };


    class MeshPBR {
    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        MeshPBR(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
        ~MeshPBR();
        void calculateNorms();
        void Draw();

    private:
        GLuint VAO, VBO, EBO;

        void setupMesh();
    };

}
