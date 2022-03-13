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
#include "MeshPBR.h"
namespace Tempest
{
    MeshPBR::MeshPBR(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
    {
        this->vertices = vertices;
        this->indices = indices;

        this->setupMesh();
       // calculateNorms();
    }

    MeshPBR::~MeshPBR()
    {

    }

    void MeshPBR::calculateNorms()
    {
        
        //auto normsfunc = [](glm::vec3 A, glm::vec3 B, glm::vec3 C) 
        //{
        //    auto v1 = C - B;
        //    auto v2 = A - B;
        //    return glm::normalize(glm::cross(v1, v2));
        //};
        //if (vertices.size() % 3)
        //{
        //    std::cout << "asd" << std::endl;
        //}
        //if (vertices.size() > 2000000)
        //    return;
        if (vertices.size() % 3)
            return;
        for (unsigned int i = 0; i < vertices.size(); i += 3) 
        {
            glm::vec3 A = vertices[i].Position;
            glm::vec3 B = vertices[i + 1LL].Position;
            glm::vec3 C = vertices[i + 2LL].Position;
            //glm::vec3 norm = normsfunc(A, B, C);
            auto v1 = C - B;
            auto v2 = A - B;
            auto norm = glm::normalize(glm::cross(v1, v2));
            vertices[i].Normal += norm;
            vertices[i + 1LL].Normal += norm;
            vertices[i + 2LL].Normal += norm;


        }
        //for (unsigned int i = 0; i < vertices.size(); i++)
        //    vertices[i].Normal = glm::normalize(vertices[i].Normal);
    }

    void MeshPBR::Draw()
    {
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, GLsizei(this->indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void MeshPBR::setupMesh()
    {
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }

}