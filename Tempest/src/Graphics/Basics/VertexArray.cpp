#include "Graphics/Basics/VertexArray.h"
#include "Logger/Log.h"

namespace Tempest
{
    constexpr static GLenum GLType(VertexType type)
    {
        switch (type)
        {
            case VertexType::Float:
            case VertexType::Float2:
            case VertexType::Float3:
            case VertexType::Float4:
            case VertexType::Mat3:
            case VertexType::Mat4:		return GL_FLOAT;
            case VertexType::Int:
            case VertexType::Int2:
            case VertexType::Int3:
            case VertexType::Int4:		return GL_INT;
            case VertexType::Bool:		return GL_BOOL;
        }
    }
    VertexArray::VertexArray(const tsptr<IndexBuffer>& ibo, const tvector<tsptr<VertexBuffer>>& vbos, const tvector<BufferLayout>& layouts)
        : m_IBO(ibo)
    {
        if (vbos.size() != layouts.size())
            LOG_CRITICAL("Missing Data");

        for (auto& shared : vbos)
        {
            m_VBOs.push_back(shared);
        }

        glCreateVertexArrays(1, &m_ID);
        for (size_t i = 0; i < vbos.size(); ++i)
        {
            AttachVertexBuffer(*m_VBOs[i], layouts[i]);
        }
        AttachIndexBuffer(*m_IBO);
    }
    VertexArray::~VertexArray()
    {
        if (m_ID != 0)
            glDeleteBuffers(1, &m_ID);
    }

    uint32_t VertexArray::GetID() const
    {
        return m_ID;
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_ID);
        m_IBO->Bind();
    }

    void VertexArray::Unbind() const
    {
        m_IBO->Unbind();
        glBindVertexArray(0);
    }

    // Add move constructors for everyth?

    void VertexArray::AttachVertexBuffer(const VertexBuffer& vbo, const BufferLayout& layout)
    {
        uint32_t size_of_one_vertex = 0;
        for (const auto& attrib : layout)
            size_of_one_vertex += BufferLayout::SizeOf(attrib.type);

        const size_t nVertices = vbo.GetSize() / size_of_one_vertex;

        for (const auto& attrib : layout)
        {
            glEnableVertexArrayAttrib(m_ID, m_Index);
            glVertexArrayVertexBuffer(m_ID, m_Index, vbo.GetID(), attrib.offset * nVertices, attrib.stride);
            switch (GLType(attrib.type))
            {
                case GL_FLOAT:
                    glVertexArrayAttribFormat(m_ID, m_Index, BufferLayout::CountOf(attrib.type), GL_FLOAT, attrib.normalized, attrib.relative_offset);
                    break;
                case GL_INT:
                case GL_BOOL:
                    glVertexArrayAttribIFormat(m_ID, m_Index, BufferLayout::CountOf(attrib.type), GL_INT, attrib.relative_offset);
                    break;
            }
            ++m_Index;
        }
    }

    void VertexArray::AttachIndexBuffer(const IndexBuffer& ibo) const
    {
        glVertexArrayElementBuffer(m_ID, ibo.GetID());
    }

    tsptr<VertexArray> VertexArray::GetShared()
    {
        return shared_from_this();
    };
}