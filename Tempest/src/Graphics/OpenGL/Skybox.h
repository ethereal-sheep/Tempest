#pragma once

#include "Graphics/OpenGL/VertexArray.h"
#include "Graphics/OpenGL/VertexBuffer.h"
#include "Graphics/OpenGL/BufferLayout.h"

namespace Tempest
{
	class Skybox
	{
	private:

		VertexArray m_VAO;
		VertexBuffer m_VBO;
		size_t m_VertexCount;

	public:

		Skybox();

		VertexArray& GetVAO();
		VertexBuffer& GetVBO();
		size_t GetVertexCount();
	};
}