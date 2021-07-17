#include "Graphics/Basics/IndexBuffer.h"

namespace Tempest
{
    IndexBuffer::IndexBuffer(const uint32_t* array, uint32_t count) 
        : m_Count(count)
    {
        glCreateBuffers(1, &m_ID);
        glNamedBufferData(m_ID, count * sizeof(unsigned int), array, GL_STATIC_DRAW);
    }
    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    uint32_t IndexBuffer::GetID() const
    {
        return m_ID;
    }

    uint32_t IndexBuffer::GetCount() const
    {
        return m_Count;
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }

    void IndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    //tsptr<IndexBuffer> IndexBuffer::GetShared()
    //{
    //    return shared_from_this();
    //};
}