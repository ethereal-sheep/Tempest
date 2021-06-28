#pragma once
#include "Core.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Tempest
{
    class VertexArray
    {
        uint32_t m_ID;
        tsptr<IndexBuffer> m_IBO;
        tvector<tsptr<VertexBuffer>> m_VBOs;

    public:
        VertexArray(
            const tsptr<IndexBuffer>& ibo, 
            const tvector<tsptr<VertexBuffer>>& vbos)
            : m_IBO(ibo->GetShared())
        {
            for(auto& shared : vbos)
            {
                m_VBOs.push_back(shared.GetShared());
            }
            //glCreate(m_ID)
            // bind_buffers
        }
        ~VertexArray()
        {
            // gldelete(m_ID)
        }

        uint32 GetID() const
        {

        }

        void Bind() const
        {
            // glbind(m_ID)
            // m_IBO->Bind();
        }

        void Unbind() const
        {
            // m_IBO->Unbind();
            // glunbind(m_ID)
        }


    };
}