#pragma once
#include <glew.h>
#include <glm.hpp>

namespace Tempest
{
	enum struct BufferType
	{
		ARRAY_BUFFER,
		ATOMIC_COUNTER_BUFFER,
		COPY_READ_BUFFER,
		COPY_WRITE_BUFFER,
		DISPATCH_INDIRECT_BUFFER,
		DRAW_INDIRECT_BUFFER,
		PIXEL_PACK_BUFFER,
		PIXEL_UNPACK_BUFFER,
		QUERY_BUFFER,
		SHADER_STORAGE_BUFFER,
		TEXTURE_BUFFER,
		TRANSFORM_FEEDBACK_BUFFER,
		UNIFORM_BUFFER
	};

	enum struct BufferMode
	{
		STREAM_DRAW,
		STREAM_READ,
		STREAM_COPY,
		STATIC_DRAW,
		STATIC_READ,
		STATIC_COPY,
		DYNAMIC_DRAW,
		DYNAMIC_READ,
		DYNAMIC_COPY
	};

	class VertexBuffer
	{
	private:

		unsigned int m_id;
		int m_size;
		BufferType m_type;

	public:

		VertexBuffer();
		VertexBuffer(void* data, int size, BufferMode mode, BufferType type = BufferType::ARRAY_BUFFER);
		~VertexBuffer();

		void Bind() const;
		void BindBase(unsigned int index) const;
		void Unbind() const;
		void SetData(void* data, int size, BufferMode mode);
		void SetSubData(void* data, int size, int offset = 0);

		unsigned int GetID() const;
		int GetSize() const;
		BufferType GetBufferType() const;
	};
}