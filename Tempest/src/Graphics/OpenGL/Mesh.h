#pragma once

#include <vector>
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <mat4x4.hpp>
#include <vec4.hpp>
#include <gtc/epsilon.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/transform.hpp>
#include <gtx/quaternion.hpp>
#include <ext/scalar_constants.hpp>

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

		Mesh() = default;
		Mesh(const std::pair<Vertices, Indices>&data);

		VertexArray& GetVAO();
		VertexBuffer& GetVBO();
		IndexBuffer& GetIBO();
		size_t GetVertexCount();

		static std::pair<Vertices, Indices> GenerateIndexedPlane(int stacks, int slices);
		static std::pair<Vertices, Indices> GenerateIndexedCube(int stacks, int slices); // Width and Height
		static std::pair<Vertices, Indices> GenerateIndexedSphere(float radius = 1.f, int stacks = 16, int slices = 16);
		static Indices GenerateIndices(int stacks = 1, int slices = 1);
	};
}