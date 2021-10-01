#include "Graphics/Basics/Mesh.h"

namespace Tempest
{
    Mesh::Mesh(const std::pair<Vertices, Indices>& data)
    {
		const auto& [vertices, indices] = data;

		std::shared_ptr<VertexBuffer> m_VBO = std::make_shared<VertexBuffer>();
		std::shared_ptr<IndexBuffer> m_IBO = std::make_shared<IndexBuffer>(indices.data(), static_cast<uint32_t>(indices.size()));

		tvector<tsptr<VertexBuffer>> vbos;
		tvector<BufferLayout> layouts;
		
		auto size_1 = static_cast<uint32_t>(sizeof(glm::vec3) * vertices.position.size());
		auto size_2 = static_cast<uint32_t>(sizeof(glm::vec2) * vertices.texCoord.size());
		auto size_3 = static_cast<uint32_t>(sizeof(glm::vec3) * vertices.normal.size());
		auto size_4 = static_cast<uint32_t>(sizeof(glm::vec3) * vertices.tangent.size());
		auto size_5 = static_cast<uint32_t>(sizeof(glm::vec3) * vertices.bitangent.size());
		auto total_size = size_1 + size_2 + size_3 + size_4 + size_5;

		m_VBO->SetData(nullptr, total_size, BufferType::ARRAY_BUFFER);

		m_VBO->SetSubData((void*)vertices.position.data(), size_1);
		m_VBO->SetSubData((void*)vertices.texCoord.data(), size_2, size_1);
		m_VBO->SetSubData((void*)vertices.normal.data(), size_3, size_1 + size_2);
		m_VBO->SetSubData((void*)vertices.tangent.data(), size_4, size_1 + size_2 + size_3);
		m_VBO->SetSubData((void*)vertices.bitangent.data(), size_5, size_1 + size_2 + size_3 + size_4);
		
		vbos.push_back(m_VBO);
		layouts.push_back(vertices.GetLayout());

		m_VAO = std::make_shared<VertexArray>(m_IBO, vbos, layouts);
		m_Count = static_cast<uint32_t>(indices.size());
    }

    void Mesh::Bind() const
    {
        m_VAO->Bind();
    }
    void Mesh::Unbind() const
    {
        m_VAO->Unbind();
    }

	uint32_t Mesh::GetVertexCount() const
	{
		return m_Count;
	}

	tsptr<VertexArray> Mesh::GetVertexArray()
	{
		return m_VAO;
	}

	MeshCode Mesh::GetMeshCode() const
	{
		return m_Code;
	}
	
}