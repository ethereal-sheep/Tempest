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
        void Draw();

    private:
        GLuint VAO, VBO, EBO;

        void setupMesh();
    };

}
