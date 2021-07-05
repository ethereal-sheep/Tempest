#pragma once

#include <vector>

#include "TMath.h"

#include "Graphics/OpenGL/BufferLayout.h"
#include "Graphics/OpenGL/IndexBuffer.h"
#include "Graphics/OpenGL/VertexArray.h"
#include "Graphics/OpenGL/VertexBuffer.h"

namespace Tempest
{
	struct Vertices
	{
		std::vector<glm::vec3> position;
		std::vector<glm::vec2> texCoord;
		std::vector<glm::vec3> normal;
		std::vector<glm::vec3> tangent;
		std::vector<glm::vec3> bitangent;

		size_t GetVertexCount() const;
		BufferLayout GetLayout() const;
	};

	struct Indices : std::vector<uint32_t>
	{
		size_t GetIndicesCount() const;
	};

	class Mesh
	{
	private:

		VertexArray m_VAO;
		VertexBuffer m_VBO;
		IndexBuffer m_IBO;

		unsigned int m_VertexCount = 0;

	public:

		Mesh(const std::pair<Vertices, Indices>&data);
		Mesh() = delete;
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh(Mesh&&) = default;
		Mesh& operator=(Mesh&& mesh) noexcept = default;

		VertexArray& GetVAO();
		VertexBuffer& GetVBO();
		IndexBuffer& GetIBO();
		size_t GetVertexCount();

		static std::pair<Vertices, Indices> GenerateIndexedPlane(int stacks, int slices);
		static std::pair<Vertices, Indices> GenerateIndexedCube(int stacks, int slices); // Width and Height
		static std::pair<Vertices, Indices> GenerateIndexedSphere(float radius = 1.f, int stacks = 16, int slices = 16);
		static std::pair<Vertices, Indices> GenerateIndexedIcosahedron();
		static std::pair<Vertices, Indices> GenerateIndexedDodecahedron();
		static Indices GenerateIndices(int stacks = 1, int slices = 1);
	};
}