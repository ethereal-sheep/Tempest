#pragma once
#include "Core.h"
#include "VertexArray.h"

namespace Tempest
{
    class Mesh
    {
        tsptr<VertexArray> m_VAO;
        MeshCode m_Code;
    public:
        void Bind() const
        {
            m_VAO.Bind();
        }
        void Unind() const
        {
            m_VAO.Unbind();
        }


    };

}