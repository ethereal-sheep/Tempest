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