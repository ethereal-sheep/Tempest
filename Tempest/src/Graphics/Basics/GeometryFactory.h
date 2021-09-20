#pragma once
#include "Core.h"
#include "TMath.h"
#include "Graphics/OpenGL/BufferLayout.h"

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

	struct GeometryFactory
	{
		static std::pair<Vertices, Indices> GenerateIndexedPlane(int stacks, int slices);
		static std::pair<Vertices, Indices> GenerateIndexedPlane();							// For Ground Plane
		static std::pair<Vertices, Indices> GenerateIndexedCube(int stacks, int slices); // Width and Height
		static std::pair<Vertices, Indices> GenerateIndexedSphere(float radius = 1.f, int stacks = 16, int slices = 16);
		static std::pair<Vertices, Indices> GenerateIndexedIcosahedron();
		static std::pair<Vertices, Indices> GenerateIndexedDodecahedron();
		static Indices GenerateIndices(int stacks = 1, int slices = 1);
	};
	
}