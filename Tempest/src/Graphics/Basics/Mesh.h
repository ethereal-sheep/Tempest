#pragma once
#include "Core.h"
#include "Graphics/Basics/VertexArray.h"
#include "Graphics/Basics/GeometryFactory.h"

namespace Tempest
{
    enum struct MeshCode
    {
        PLANE,
        SPHERE,
        CUBE,
        ICOSAHEDRON
    };

    class Mesh
    {
        tsptr<VertexArray> m_VAO;
        MeshCode m_Code;
        uint32_t m_Count = 0;

    public:

        Mesh(const std::pair<Vertices, Indices>& data);
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
        Mesh(Mesh&& mesh) noexcept;
        Mesh& operator=(Mesh&& mesh)noexcept;
        //Mesh(const Mesh& m);

        void Bind() const;
        void Unbind() const;

        uint32_t GetVertexCount() const;
        tsptr<VertexArray> GetVertexArray();
        MeshCode GetMeshCode() const;
    };

}