/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Graphics/Basics/VertexBuffer.h"

namespace Tempest
{
    constexpr GLenum Type(BufferType type)
    {
        switch (type)
        {
            case BufferType::ARRAY_BUFFER:          return GL_ARRAY_BUFFER;
            case BufferType::INDIRECT_BUFFER:       return GL_DRAW_INDIRECT_BUFFER;
            default: return GL_ARRAY_BUFFER;
        }
    }

    VertexBuffer::VertexBuffer()
    {
        glCreateBuffers(1, &m_ID);
    }

    VertexBuffer::VertexBuffer(void* data, uint32_t size, BufferType type) 
        : m_Size{ size }, m_Type{ type }
    {
        glCreateBuffers(1, &m_ID);
        glNamedBufferData(m_ID, size, data, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    uint32_t VertexBuffer::GetID() const
    {
        return m_ID;
    }

    uint32_t VertexBuffer::GetSize() const
    {
        return m_Size;
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(Type(m_Type), m_ID);
    }

    void VertexBuffer::Unbind() const
    {
        glBindBuffer(Type(m_Type), 0);
    }

    void VertexBuffer::SetData(void* data, int size, BufferType type)
    {
        glNamedBufferData(m_ID, size, data, GL_DYNAMIC_DRAW);
        m_Type = type;
        m_Size = size;
    }

    void VertexBuffer::SetSubData(void* data, int size, int offset)
    {
        glNamedBufferSubData(m_ID, offset, size, data);
    }

    void VertexBuffer::SetSubDataResize(void* data, int size)
    {
        if ((int)GetSize() < size)
            SetData(data, size, m_Type);
        else
            SetSubData(data, size, 0);
    }

    tsptr<VertexBuffer> VertexBuffer::GetShared()
    {
        return shared_from_this();
    };
}