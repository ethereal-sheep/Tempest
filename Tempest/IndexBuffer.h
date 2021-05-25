#pragma once
#include <glew.h>

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

		void Bind() const;
		void Unbind() const;
		void SetData(const unsigned int* data, unsigned int size);

		unsigned int GetCount() const;
		unsigned int GetID() const;
	};
}


