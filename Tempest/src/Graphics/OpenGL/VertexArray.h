#pragma once
#include "Graphics/OpenGL/IndexBuffer.h"
#include "Graphics/OpenGL/VertexBuffer.h"
#include "Graphics/OpenGL/BufferLayout.h"

namespace Tempest
{
	class VertexArray
	{
	private:

		unsigned int m_ID = 0;
		unsigned int m_AttributeIndex = 0;

	public:
		VertexArray();
		~VertexArray();

		void AttachVertexBuffer(const VertexBuffer& vbo, const BufferLayout& layout);
		void AttachVertexBufferInstanced(const VertexBuffer& vbo, const BufferLayout& layout);
		void AttachIndexBuffer(const IndexBuffer& ibo) const;

		void SetBindingDivisor(unsigned int index, unsigned int divisor) const;

		unsigned int GetID() const;
		void Bind() const;
		void Unbind() const;

		int GetAttributeCount() const;
	};
}
