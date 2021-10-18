/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "LineRenderer.h"

namespace Tempest
{
	LineRenderer::LineRenderer()
	{
		m_VBO.SetData(nullptr, 0, BufferType::ARRAY_BUFFER);
		m_InstancedVBO.SetData(nullptr, 0, BufferType::ARRAY_BUFFER);
		m_Indirect.SetData(nullptr, 0, BufferType::INDIRECT_BUFFER);

		const BufferLayout vbol{ Layout_Format::SOA, { VertexType::Float3 }};
		m_VAO.AttachVertexBuffer(m_VBO, vbol);

		const BufferLayout ivbol{ Layout_Format::AOS, { VertexType::Float4 }};
		m_VAO.AttachVertexBufferInstanced(m_InstancedVBO, ivbol);
	}

	void LineRenderer::Submit(const Line& line, const glm::vec4& color)
	{
		m_VertexBuffer.emplace_back(line.p0);
		m_VertexBuffer.emplace_back(line.p1);
		m_InstanceBuffer.emplace_back(color);

		auto& cmd = m_IndirectCommand.emplace_back();
		cmd.vertexCount = 2;
		cmd.instanceCount = 1;
		cmd.baseVertex = m_CurVertex;
		cmd.baseInstance = static_cast<uint32_t>(m_IndirectCommand.size() - 1);

		m_CurVertex += cmd.vertexCount;
	}

	void LineRenderer::Submit(const AABB& box, const glm::vec4& color)
	{
		m_VertexBuffer.emplace_back(box.min.x, box.min.y, box.min.z);
		m_VertexBuffer.emplace_back(box.max.x, box.min.y, box.min.z);
		m_VertexBuffer.emplace_back(box.min.x, box.max.y, box.min.z);
		m_VertexBuffer.emplace_back(box.max.x, box.max.y, box.min.z);

		m_VertexBuffer.emplace_back(box.min.x, box.min.y, box.min.z);
		m_VertexBuffer.emplace_back(box.min.x, box.max.y, box.min.z);
		m_VertexBuffer.emplace_back(box.max.x, box.min.y, box.min.z);
		m_VertexBuffer.emplace_back(box.max.x, box.max.y, box.min.z);

		m_InstanceBuffer.emplace_back(color);

		auto& cmd = m_IndirectCommand.emplace_back();
		cmd.vertexCount = 8;
		cmd.instanceCount = 1;
		cmd.baseVertex = m_CurVertex;
		cmd.baseInstance = static_cast<uint32_t>(m_IndirectCommand.size() - 1);

		if (glm::epsilonNotEqual(box.min.z, box.max.z, glm::epsilon<float>()))
		{
			m_VertexBuffer.emplace_back(box.min.x, box.min.y, box.max.z);
			m_VertexBuffer.emplace_back(box.max.x, box.min.y, box.max.z);
			m_VertexBuffer.emplace_back(box.min.x, box.max.y, box.max.z);
			m_VertexBuffer.emplace_back(box.max.x, box.max.y, box.max.z);

			m_VertexBuffer.emplace_back(box.min.x, box.min.y, box.max.z);
			m_VertexBuffer.emplace_back(box.min.x, box.max.y, box.max.z);
			m_VertexBuffer.emplace_back(box.max.x, box.min.y, box.max.z);
			m_VertexBuffer.emplace_back(box.max.x, box.max.y, box.max.z);

			m_VertexBuffer.emplace_back(box.min.x, box.min.y, box.min.z);
			m_VertexBuffer.emplace_back(box.min.x, box.min.y, box.max.z);
			m_VertexBuffer.emplace_back(box.max.x, box.min.y, box.min.z);
			m_VertexBuffer.emplace_back(box.max.x, box.min.y, box.max.z);

			m_VertexBuffer.emplace_back(box.min.x, box.max.y, box.min.z);
			m_VertexBuffer.emplace_back(box.min.x, box.max.y, box.max.z);
			m_VertexBuffer.emplace_back(box.max.x, box.max.y, box.min.z);
			m_VertexBuffer.emplace_back(box.max.x, box.max.y, box.max.z);

			cmd.vertexCount += 16;
		}

		m_CurVertex += cmd.vertexCount;
	}

	void LineRenderer::ClearBuffer()
	{
		m_VertexBuffer.clear();
		m_InstanceBuffer.clear();
		m_IndirectCommand.clear();
		m_CurVertex = 0;
	}

	void LineRenderer::SubmitBuffer()
	{
		m_VBO.SetData(nullptr, 0, BufferType::ARRAY_BUFFER);
		m_InstancedVBO.SetData(nullptr, 0, BufferType::ARRAY_BUFFER);
		m_Indirect.SetData(nullptr, 0, BufferType::INDIRECT_BUFFER);

		m_VBO.SetSubDataResize(m_VertexBuffer.data(), static_cast<uint32_t>(m_VertexBuffer.size() * sizeof(glm::vec3)));
		m_InstancedVBO.SetSubDataResize(m_InstanceBuffer.data(), static_cast<uint32_t>(m_InstanceBuffer.size() * sizeof(glm::vec4)));
		m_Indirect.SetSubDataResize(m_IndirectCommand.data(), static_cast<uint32_t>(m_IndirectCommand.size() * sizeof(DrawArrayIndirect)));
	}

	size_t LineRenderer::GetVertexCount() const
	{
		return m_VertexBuffer.size();
	}

	VertexArray& LineRenderer::GetVAO()
	{
		return m_VAO;
	}

	VertexBuffer& LineRenderer::GetIndirect()
	{
		return m_Indirect;
	}

	void LineRenderer::Render(const glm::mat4& vpm, const tuptr<Shader>& shader)
	{
		shader->Bind();
		shader->SetMat4fv(vpm, "viewProjMatrix");
		glLineWidth(2.f);
		if (m_Indirect.GetSize() != 0)
		{
			m_VAO.Bind();
			m_Indirect.Bind();
			glMultiDrawArraysIndirect(GL_LINES, nullptr, m_Indirect.GetSize() / sizeof(DrawArrayIndirect), 0);
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		//glLineWidth(1.f);
		shader->Unbind();
	}
}