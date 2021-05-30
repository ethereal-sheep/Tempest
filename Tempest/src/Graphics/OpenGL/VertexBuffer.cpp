#include "VertexBuffer.h"

namespace Tempest
{
	constexpr GLenum GLMode(BufferMode bm)
	{
		switch (bm)
		{
			default: assert(0);
			case BufferMode::STREAM_DRAW:		return GL_STREAM_DRAW;
			case BufferMode::STREAM_READ:		return GL_STREAM_READ;
			case BufferMode::STREAM_COPY:		return GL_STREAM_COPY;

			case BufferMode::STATIC_DRAW:		return GL_STATIC_DRAW;
			case BufferMode::STATIC_READ:		return GL_STATIC_READ;
			case BufferMode::STATIC_COPY:		return GL_STATIC_COPY;

			case BufferMode::DYNAMIC_DRAW:		return GL_DYNAMIC_DRAW;
			case BufferMode::DYNAMIC_READ:		return GL_DYNAMIC_READ;
			case BufferMode::DYNAMIC_COPY:		return GL_DYNAMIC_COPY;
		}
	}

	constexpr GLenum GLType(BufferType bt)
	{
		switch (bt)
		{
		default: assert(0);
		case BufferType::ARRAY_BUFFER:					return GL_ARRAY_BUFFER;
		case BufferType::ATOMIC_COUNTER_BUFFER:			return GL_ATOMIC_COUNTER_BUFFER;
		case BufferType::COPY_READ_BUFFER:				return GL_COPY_READ_BUFFER;
		case BufferType::COPY_WRITE_BUFFER:				return GL_COPY_WRITE_BUFFER;
		case BufferType::DISPATCH_INDIRECT_BUFFER:		return GL_DISPATCH_INDIRECT_BUFFER;
		case BufferType::DRAW_INDIRECT_BUFFER:			return GL_DRAW_INDIRECT_BUFFER;
		case BufferType::PIXEL_PACK_BUFFER:				return GL_PIXEL_PACK_BUFFER;
		case BufferType::PIXEL_UNPACK_BUFFER:			return GL_PIXEL_UNPACK_BUFFER;
		case BufferType::QUERY_BUFFER:					return GL_QUERY_BUFFER;
		case BufferType::SHADER_STORAGE_BUFFER:			return GL_SHADER_STORAGE_BUFFER;
		case BufferType::TEXTURE_BUFFER:				return GL_TEXTURE_BUFFER;
		case BufferType::TRANSFORM_FEEDBACK_BUFFER:		return GL_TRANSFORM_FEEDBACK_BUFFER;
		case BufferType::UNIFORM_BUFFER:				return GL_UNIFORM_BUFFER;
		}
	}

	VertexBuffer::VertexBuffer()
	{
		glCreateBuffers(1, &m_id);
	}

	VertexBuffer::VertexBuffer(void* data, int size, BufferMode mode, BufferType type)
	{
		m_size = size;
		m_type = type;
		glCreateBuffers(1, &m_id);
		glNamedBufferData(m_id, size, data, GLMode(mode));
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GLType(m_type), m_id);
	}

	void VertexBuffer::BindBase(unsigned int index) const
	{
		glBindBufferBase(GLType(m_type), index, m_id);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GLType(m_type), 0);
	}

	void VertexBuffer::SetData(void* data, int size, BufferMode mode)
	{
		glNamedBufferData(m_id, size, data, GLMode(mode));
	}

	void VertexBuffer::SetSubData(void* data, int size, int offset)
	{
		if (m_size < size)
		{
			m_size = size;
			glNamedBufferData(m_id, size, data, GLMode(BufferMode::DYNAMIC_DRAW));
		}

		else
			glNamedBufferSubData(m_id, offset, size, data);
	}

	unsigned int VertexBuffer::GetID() const
	{
		return m_id;
	}

	int VertexBuffer::GetSize() const
	{
		return m_size;
	}

	BufferType VertexBuffer::GetBufferType() const
	{
		return m_type;
	}

}