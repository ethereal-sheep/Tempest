#include "VertexArray.h"
#include <vector>

namespace Tempest
{
	constexpr static GLenum GLType(VertexType type)
	{
		switch (type)
		{
		default: assert(0);
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

	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &m_ID);
	}

	VertexArray::~VertexArray()
	{
		if (m_ID != 0)
			glDeleteBuffers(1, &m_ID);
	}

	void VertexArray::AttachVertexBuffer(const VertexBuffer& vbo, const BufferLayout& layout)
	{
		uint32_t size_of_one_vertex = 0;
		for (const auto& attrib : layout)
			size_of_one_vertex += BufferLayout::SizeOf(attrib.type);

		const size_t nVertices = vbo.GetSize() / size_of_one_vertex;

		for (const auto& attrib : layout)
		{
			glEnableVertexArrayAttrib(m_ID, m_AttributeIndex);
			glVertexArrayVertexBuffer(m_ID, m_AttributeIndex, vbo.GetID(), attrib.offset * nVertices, attrib.stride);
			switch (GLType(attrib.type))
			{
			case GL_FLOAT:
				glVertexArrayAttribFormat(m_ID, m_AttributeIndex, BufferLayout::CountOf(attrib.type), GL_FLOAT,
					attrib.normalized, attrib.relative_offset);
				break;
			case GL_INT:
			case GL_BOOL:
				glVertexArrayAttribIFormat(m_ID, m_AttributeIndex, BufferLayout::CountOf(attrib.type), GL_INT,
					attrib.relative_offset);
				break;
			default: assert(0);
			}
			++m_AttributeIndex;
		}
	}

	void VertexArray::AttachVertexBufferInstanced(const VertexBuffer& vbo, const BufferLayout& layout)
	{
		const uint32_t startingIndex = m_AttributeIndex;

		AttachVertexBuffer(vbo, layout);

		for (auto i = startingIndex; i < m_AttributeIndex; ++i)
			SetBindingDivisor(i, 1);
	}

	void VertexArray::AttachIndexBuffer(const IndexBuffer& ibo) const
	{
		glVertexArrayElementBuffer(m_ID, ibo.GetID());
	}

	void VertexArray::SetBindingDivisor(uint32_t index, uint32_t divisor) const
	{
		glVertexArrayBindingDivisor(m_ID, index, divisor);
	}

	unsigned int VertexArray::GetID() const
	{
		return m_ID;
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_ID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	int VertexArray::GetAttributeCount() const
	{
		return m_AttributeIndex;
	}
}