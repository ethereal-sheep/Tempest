#pragma once
#include "Core.h"
#include "BufferLayout"

namespace Tempest
{
    class VertexBuffer : public std::enable_shared_from_this<VertexBuffer>
    {
        uint32_t m_ID;
        uint32_t m_Size;

    public:
        VertexBuffer(void* data, uint32_t size) : m_Size(size)
        {
            // glcreate(m_id)
        }
        ~VertexBuffer()
        {
            //gldelete(m_id)
        }

        uint32 GetID() const
        {

        }

        uint32 GetSize() const
        {

        }


        void Bind() const
        {

        }
        
        void Unbind() const
        {
            
        }

        tsptr<VertexBuffer> GetShared() const
        {
            return shared_from_this();
        };
    };
}