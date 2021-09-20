//#pragma once
//#include <glew.h>
//#include <cstdint>
//
//namespace Tempest
//{
//	struct DrawArraysIndirectCommand
//	{
//		uint32_t vertexCount = 0;
//		uint32_t instanceCount = 0;
//		uint32_t baseVertex = 0;
//		uint32_t baseInstance = 0;
//	};
//
//	struct DrawElementsIndirectCommand
//	{
//		uint32_t vertexCount = 0;
//		uint32_t instanceCount = 0;
//		uint32_t firstIndex = 0;
//		uint32_t baseVertex = 0;
//		uint32_t baseInstance = 0;
//	};
//
//	enum struct BufferType
//	{
//		ARRAY_BUFFER,
//		ATOMIC_COUNTER_BUFFER,
//		COPY_READ_BUFFER,
//		COPY_WRITE_BUFFER,
//		DISPATCH_INDIRECT_BUFFER,
//		DRAW_INDIRECT_BUFFER,
//		PIXEL_PACK_BUFFER,
//		PIXEL_UNPACK_BUFFER,
//		QUERY_BUFFER,
//		SHADER_STORAGE_BUFFER,
//		TEXTURE_BUFFER,
//		TRANSFORM_FEEDBACK_BUFFER,
//		UNIFORM_BUFFER
//	};
//
//	enum struct BufferMode
//	{
//		STREAM_DRAW,
//		STREAM_READ,
//		STREAM_COPY,
//		STATIC_DRAW,
//		STATIC_READ,
//		STATIC_COPY,
//		DYNAMIC_DRAW,
//		DYNAMIC_READ,
//		DYNAMIC_COPY
//	};
//
//	class VertexBuffer
//	{
//	private:
//
//		unsigned int m_ID = 0;
//		int m_Size = 0;
//		BufferType m_Type;
//
//	public:
//
//		explicit VertexBuffer();
//		explicit VertexBuffer(void* data, int size, BufferMode mode, BufferType type = BufferType::ARRAY_BUFFER);
//		~VertexBuffer();
//
//		VertexBuffer(const VertexBuffer&) = delete;
//		VertexBuffer& operator=(const VertexBuffer&) = delete;
//
//		VertexBuffer(VertexBuffer&& rhs) noexcept;
//		VertexBuffer& operator=(VertexBuffer&& rhs) noexcept;
//
//		void Bind() const;
//		void BindBase(unsigned int index) const;
//		void Unbind() const;
//		void SetData(void* data, int size, BufferMode mode, BufferType type = BufferType::ARRAY_BUFFER);
//		void SetSubData(void* data, int size, int offset = 0);
//		void SetSubDataResize(void* data, int size);
//
//		unsigned int GetID() const;
//		int GetSize() const;
//		BufferType GetBufferType() const;
//	};
//}