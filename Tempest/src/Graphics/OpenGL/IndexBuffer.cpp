#include "IndexBuffer.h"

namespace Tempest
{
	IndexBuffer::IndexBuffer()
	{
		glCreateBuffers(1, &m_ID);
	}

	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int size)
	{
		m_Count = size;
		glCreateBuffers(1, &m_ID);
		glNamedBufferData(m_ID, size * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& rhs) noexcept
		: m_ID{std::move(rhs.m_ID)},
		m_Count{std::move(rhs.m_Count)}
	{
		rhs.m_ID = 0;
		rhs.m_Count = 0;
	}
	
	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& rhs) noexcept
	{
		std::swap(m_ID, rhs.m_ID);
		std::swap(m_Count, rhs.m_Count);
		return *this;
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::SetData(const unsigned int* data, unsigned int size)
	{
		m_Count = size;
		glNamedBufferData(m_ID, size * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}

	unsigned int IndexBuffer::GetCount() const
	{
		return m_Count;
	}

	unsigned int IndexBuffer::GetID() const
	{
		return m_ID;
	}
}