#pragma once
#include <glew.h>
#include <utility>

namespace Tempest
{
	class IndexBuffer
	{
	private:

		unsigned int id;
		unsigned int count;

	public:

		IndexBuffer();
		IndexBuffer(const unsigned int* data, unsigned int size);
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		IndexBuffer(IndexBuffer&& rhs) noexcept;
		IndexBuffer& operator=(IndexBuffer&& rhs) noexcept;

		void Bind() const;
		void Unbind() const;
		void SetData(const unsigned int* data, unsigned int size);

		unsigned int GetCount() const;
		unsigned int GetID() const;
	};
}


