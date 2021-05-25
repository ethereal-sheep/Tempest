#include "IndexBuffer.h"

namespace Tempest
{
	IndexBuffer::IndexBuffer()
	{
		glCreateBuffers(1, &id);
	}

	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int size)
	{
		count = size;
		glCreateBuffers(1, &id);
		glNamedBufferData(id, size * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &id);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::SetData(const unsigned int* data, unsigned int size)
	{
		count = size;
		glNamedBufferData(id, size * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}

	unsigned int IndexBuffer::GetCount() const
	{
		return count;
	}

	unsigned int IndexBuffer::GetID() const
	{
		return id;
	}
}