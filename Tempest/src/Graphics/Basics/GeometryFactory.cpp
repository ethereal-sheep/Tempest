/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Graphics/Basics/GeometryFactory.h"

namespace Tempest
{
	size_t Vertices::GetVertexCount() const
	{
		return position.size();
	}
	
	BufferLayout Vertices::GetLayout() const
	{
		return  BufferLayout
		{
			Layout_Format::SOA,
			{
				VertexType::Float3,
				VertexType::Float2,
				VertexType::Float3,
				VertexType::Float3,
				VertexType::Float3
			}
		};
	}
	
	size_t Indices::GetIndicesCount() const
	{
		return size();
	}

	std::pair<Vertices, Indices> GeometryFactory::GenerateIndexedPlane()
	{
		Vertices vertices;
		for (int i = 0; i <= 1; ++i)
		{
			float row = i / static_cast<float>(1);
			for (int j = 0; j <= 1; ++j)
			{
				float col = j / static_cast<float>(1);

				vertices.position.emplace_back(col * 2 - 1.f, row * 2 - 1.f, 0.f);
				vertices.texCoord.emplace_back(col, row);
				vertices.normal.emplace_back(0.f, 0.f, 1.f);
				vertices.tangent.emplace_back(1.f, 0.f, 0.f);
				vertices.bitangent.emplace_back(0.f, 1.f, 0.f);
			}
		}

		Indices indices = GenerateIndices(1, 1);
		//AssertVertexCount(vertices);

		return { vertices, indices };
	}

	std::pair<Vertices, Indices> GeometryFactory::GenerateIndexedPlane(int stacks, int slices)
	{
		Vertices vertices;
		for (int i = 0; i <= stacks; ++i)
		{
			float row = i / static_cast<float>(stacks);
			for (int j = 0; j <= slices; ++j)
			{
				float col = j / static_cast<float>(slices);

				vertices.position.emplace_back(col - 0.5f, row - 0.5f, 0.f);
				vertices.texCoord.emplace_back(col, row);
				vertices.normal.emplace_back(0.f, 0.f, 1.f);
				vertices.tangent.emplace_back(1.f, 0.f, 0.f);
				vertices.bitangent.emplace_back(0.f, 1.f, 0.f);
			}
		}

		Indices indices = GenerateIndices(stacks, slices);
		//AssertVertexCount(vertices);

		return { vertices, indices };
	}

	std::pair<Vertices, Indices> GeometryFactory::GenerateIndexedCube(int stacks, int slices)
	{
		auto [planeVertices, planeIndices] = GenerateIndexedPlane(stacks, slices);
		Vertices vertices;
		Indices indices;

		constexpr glm::vec3 translateArray[] =
		{
			glm::vec3(+0.0f, +0.0f, +0.5f), // Z+
			glm::vec3(+0.0f, +0.0f, -0.5f), // Z-
			glm::vec3(+0.5f, +0.0f, +0.0f), // X+
			glm::vec3(-0.5f, +0.0f, +0.0f), // X-
			glm::vec3(+0.0f, +0.5f, +0.0f), // Y+
			glm::vec3(+0.0f, -0.5f, +0.0f), // Y-
		};

		constexpr glm::vec2 rotateArray[] =
		{
			glm::vec2(+0.0f, +0.0f),					// Z+
			glm::vec2(+0.0f, +glm::pi<float>()),		// Z-
			glm::vec2(+0.0f, +glm::half_pi<float>()),	// X+       
			glm::vec2(+0.0f, -glm::half_pi<float>()),	// X-
			glm::vec2(-glm::half_pi<float>(), +0.0f),	// Y+
			glm::vec2(+glm::half_pi<float>(), +0.0f)	// Y-
		};

		for (auto i = 0; i < planeIndices.GetIndicesCount(); ++i)
		{
			glm::mat4 transformMat = glm::translate(translateArray[i])
				* glm::rotate(rotateArray[i].y, glm::vec3{ 0.f, 1.f, 0.f })
				* glm::rotate(rotateArray[i].x, glm::vec3{ 1.f, 0.f, 0.f });

			for (int j = 0; j < planeVertices.GetVertexCount(); ++j)
			{
				vertices.position.emplace_back(glm::vec3(transformMat * glm::vec4(planeVertices.position[j], 1.0)));
				vertices.texCoord.emplace_back(planeVertices.texCoord[j]);
				vertices.normal.emplace_back(glm::vec3(transformMat * glm::vec4(planeVertices.normal[j], 1.0)));
				vertices.tangent.emplace_back(glm::vec3(transformMat * glm::vec4(planeVertices.tangent[j], 1.0)));
				vertices.bitangent.emplace_back(glm::vec3(transformMat * glm::vec4(planeVertices.bitangent[j], 1.0)));
			}

			for (size_t j = 0; j < planeIndices.GetIndicesCount(); ++j)
				indices.push_back(static_cast<uint32_t>(planeIndices[j] + planeVertices.GetVertexCount() * i));
		}
		//AssertVertexCount(vertices);

		return { vertices, indices };
	}

	std::pair<Vertices, Indices> GeometryFactory::GenerateIndexedSphere(float radius, int stacks, int slices)
	{
		Vertices vertices;
		for (int stack = 0; stack <= stacks; ++stack)
		{
			const float row = static_cast<float>(stack) / static_cast<float>(stacks);
			const float beta = glm::pi<float>() * (row - 0.5f);
			const float sinBeta = sin(beta);
			const float cosBeta = cos(beta);

			for (int slice = 0; slice <= slices; ++slice)
			{
				const float col = static_cast<float>(slice) / static_cast<float>(slices);
				const float alpha = col * glm::pi<float>() * 2.f;
				const float sinAlpha = sin(alpha);
				const float cosAlpha = cos(alpha);

				// spherical coordinates, inclination - Beta, azimuth - alpha
				glm::vec3 v = {
					radius * cosBeta * sinAlpha,
					radius * sinBeta,
					radius * cosBeta * cosAlpha
				};
				glm::vec3 n = normalize(v);
				glm::vec3 c1 = cross(n, glm::vec3(0.0, 0.0, 1.0));
				glm::vec3 c2 = cross(n, glm::vec3(0.0, 1.0, 0.0));
				glm::vec3 t = normalize(length(c1) > length(c2) ? c1 : c2);
				glm::vec3 b = cross(t, n);

				vertices.position.emplace_back(v);
				vertices.texCoord.emplace_back(col, row);
				vertices.normal.emplace_back(n);
				vertices.tangent.emplace_back(t);
				vertices.bitangent.emplace_back(b);
			}
		}

		Indices indices = GenerateIndices(stacks, slices);
		//AssertVertexCount(vertices);

		return { vertices, indices };
	}

	std::pair<Vertices, Indices> GeometryFactory::GenerateIndexedIcosahedron()
	{
		constexpr double X = 0.525731112119133606;
		constexpr double Z = 0.850650808352039932;

		double vdata[] =
		{
			-X, 0.0, Z,    X, 0.0, Z,  -X,  0.0, -Z,  X,  0.0, -Z,
			0.0, Z,  X,   0.0, Z, -X,  0.0, -Z,   X, 0.0, -Z,  -X,
			 Z,  X, 0.0,  -Z,  X, 0.0,  Z,  -X,  0.0, -Z, -X,  0.0
		};

		uint32_t tindices[60] =
		{
			0,  4, 1, 0, 9,  4, 9,  5, 4,  4, 5, 8, 4, 8, 1,
			8, 10, 1, 8, 3, 10, 5,  3, 8,  5, 2, 3, 2, 7, 3,
			7, 10, 3, 7, 6, 10, 7, 11, 6, 11, 0, 6, 0, 1, 6,
			6, 1, 10, 9, 0, 11, 9, 11, 2,  9, 2, 5, 7, 2, 11
		};

		Vertices vertices;
		Indices indices;

		for (int i = 0; i < 36; i += 3)
		{
			glm::vec3 v = glm::vec3(static_cast<GLfloat>(vdata[i]), static_cast<GLfloat>(vdata[i + 1]), static_cast<GLfloat>(vdata[i + 2]));
			glm::vec3 n = normalize(v);
			glm::vec3 c1 = cross(n, glm::vec3(0.0, 0.0, 1.0));
			glm::vec3 c2 = cross(n, glm::vec3(0.0, 1.0, 0.0));
			glm::vec3 t = normalize(length(c1) > length(c2) ? c1 : c2);
			glm::vec3 b = cross(t, n);

			vertices.position.emplace_back(v);
			vertices.texCoord.emplace_back(0, 1);
			vertices.normal.emplace_back(n);
			vertices.tangent.emplace_back(t);
			vertices.bitangent.emplace_back(b);
		}

		for (int i = 0; i < 60; ++i)
		{
			indices.emplace_back(tindices[i]);
		}

		return { vertices, indices };
	}

	std::pair<Vertices, Indices> GeometryFactory::GenerateIndexedDodecahedron()
	{
		double vdata[] =
		{
			-0.57735,	-0.57735,  0.57735,
			 0.934172,  0.356822,  0,
			 0.934172, -0.356822,  0,
			-0.934172,  0.356822,  0,
			-0.934172, -0.356822,  0,
			 0,  0.934172,  0.356822,
			 0,  0.934172, -0.356822,
			 0.356822,  0, -0.934172,
			-0.356822,  0, -0.934172,
			 0, -0.934172, -0.356822,
			 0, -0.934172,  0.356822,
			 0.356822,  0,  0.934172,
			-0.356822,  0,  0.934172,
			 0.57735,  0.57735, -0.57735,
			 0.57735,  0.57735,  0.57735,
			-0.57735,  0.57735, -0.57735,
			-0.57735,  0.57735,  0.57735,
			 0.57735, -0.57735, -0.57735,
			 0.57735, -0.57735,  0.57735,
			-0.57735, -0.57735, -0.57735
		};

		Vertices vertices;
		Indices indices;

		return { vertices, indices };
	}

	Indices GeometryFactory::GenerateIndices(int stacks, int slices)
	{
		Indices indices;
		const uint32_t stride = slices + 1;
		for (uint32_t i = 0; i < static_cast<uint32_t>(stacks); ++i)
		{
			const uint32_t cur_row = i * stride;
			for (uint32_t j = 0; j < static_cast<uint32_t>(slices); ++j)
			{
				uint32_t I0 = cur_row + j;
				uint32_t I1 = I0 + 1;
				uint32_t I2 = I1 + stride;

				uint32_t I3 = I2;
				uint32_t I4 = I3 - 1;
				uint32_t I5 = I0;

				indices.push_back(I0);
				indices.push_back(I1);
				indices.push_back(I2);
				indices.push_back(I3);
				indices.push_back(I4);
				indices.push_back(I5);
			}
		}
		return indices;
	}
}