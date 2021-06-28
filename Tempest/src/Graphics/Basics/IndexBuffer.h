#pragma once
#include "Core.h"
#include "VertexArray"

namespace Tempest
{
    class IndexBuffer : std::enable_shared_from_this<IndexBuffer>
    {
        uint32 m_ID;
        uint32 m_Count;

    public:
        IndexBuffer(const uint32* array, uint32 count) : m_Count(count)
        {
            // glcreate(m_id)
            // createindexbuffer
        }
        ~IndexBuffer()
        {
            // delete
        }

        uint32 GetID() const
        {
            return m_ID;
        }

        uint32 GetCount() const
        {
            return m_Count;
        }

        void Bind() const
        {

        }

        void Unbind() const
        {

        }




        tsptr<IndexBuffer> GetShared()
        {
            return shared_from_this();
        };
    };
}