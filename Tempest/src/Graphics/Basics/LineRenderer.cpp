#include "Graphics/Basics/LineRenderer.h"
#include "Logger/Log.h"

namespace Tempest
{
	constexpr static GLenum GLType(VertexType type)
	{
		switch (type)
		{
			default: assert(0);
			case VertexType::Float:
			case VertexType::Float2:
			case VertexType::Float3:
			case VertexType::Float4:
			case VertexType::Mat3:
			case VertexType::Mat4:		return GL_FLOAT;
			case VertexType::Int:
			case VertexType::Int2:
			case VertexType::Int3:
			case VertexType::Int4:		return GL_INT;
			case VertexType::Bool:		return GL_BOOL;
		}
	}

	LineRenderer::LineRenderer()
	{
		glCreateBuffers(1, &m_VBO);				// Normal Operations [Array Buffer]
		glNamedBufferData(m_VBO, 0, nullptr, GL_DYNAMIC_DRAW);

		glCreateBuffers(1, &m_InstancedVBO);	// Instancing [Array Buffer]
		glNamedBufferData(m_InstancedVBO, 0, nullptr, GL_DYNAMIC_DRAW);

		glCreateBuffers(1, &m_IndirectVBO);		// Indirect Draw Call [Indirect Buffer]
		glNamedBufferData(m_IndirectVBO, 0, nullptr, GL_DYNAMIC_DRAW);
		
		glCreateVertexArrays(1, &m_VAO);

		const BufferLayout vbol{ Layout_Format::SOA, { VertexType::Float3 } };
		AttachVertexBuffer(vbol);
		
		const BufferLayout ivbol{ Layout_Format::AOS, { VertexType::Float4 }};
		AttachInstancedBuffer(ivbol);
	}

	LineRenderer::~LineRenderer()
	{
		glDeleteBuffers(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_InstancedVBO);
		glDeleteBuffers(1, &m_IndirectVBO);
	}	

	void LineRenderer::Submit(const Line& line, const glm::vec4& color)
	{
		m_VertexBuffer.emplace_back(line.p0);
		m_VertexBuffer.emplace_back(line.p1);
		m_InstanceBuffer.emplace_back(color);
		
		auto& cmd = m_Indirect.emplace_back();
		cmd.vertexCount = 2;
		cmd.instanceCount = 1;
		cmd.baseVertex = m_CurVertex;
		cmd.baseInstance = static_cast<uint32_t>(m_Indirect.size() - 1);
		
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
		
		auto& cmd = m_Indirect.emplace_back();
		cmd.vertexCount = 8;
		cmd.instanceCount = 1;
		cmd.baseVertex = m_CurVertex;
		cmd.baseInstance = static_cast<uint32_t>(m_Indirect.size() - 1);
		
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
		m_Indirect.clear();
		m_CurVertex = 0;
	}

	void LineRenderer::SubmitBuffer()
	{
		glNamedBufferData(m_VBO, 0, nullptr, GL_DYNAMIC_DRAW);
		glNamedBufferData(m_InstancedVBO, 0, nullptr, GL_DYNAMIC_DRAW);
		glNamedBufferData(m_IndirectVBO, 0, nullptr, GL_DYNAMIC_DRAW);

		ResizeVBO(m_VertexBuffer.data(), static_cast<uint32_t>(m_VertexBuffer.size() * sizeof(glm::vec3)));
		ResizeInstanced(m_InstanceBuffer.data(), static_cast<uint32_t>(m_InstanceBuffer.size() * sizeof(glm::vec4)));
		ResizeIndirect(m_Indirect.data(), static_cast<uint32_t>(m_Indirect.size() * sizeof(Line_Indirect)));
	}

	void LineRenderer::Render(const glm::mat4& vproj)
	{
		m_Shader.Bind();
		m_Shader.SetMat4fv(vproj, "viewProjMatrix");
		glLineWidth(2.f);
		if (IndirectVBO_Size != 0)
		{
			glBindVertexArray(m_VAO);
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_IndirectVBO);
			glMultiDrawArraysIndirect(GL_LINE, nullptr, IndirectVBO_Size / sizeof(Line_Indirect), 0);
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		//glLineWidth(1.f);
		m_Shader.Unbind();
	}

	void LineRenderer::AttachVertexBuffer(const BufferLayout& layout)
	{
		uint32_t size_of_one_vertex = 0;

		for (const auto& attrib : layout)
			size_of_one_vertex += BufferLayout::SizeOf(attrib.type);
		
		const size_t nVertices = VBO_Size / size_of_one_vertex;
		
		for (const auto& attrib : layout)
		{
			glEnableVertexArrayAttrib(m_VAO, Attrib_Index);
			glVertexArrayVertexBuffer(m_VAO, Attrib_Index, m_VBO, attrib.offset * nVertices, attrib.stride);
			switch (GLType(attrib.type))
			{
				case GL_FLOAT:
					glVertexArrayAttribFormat(m_VAO, Attrib_Index, BufferLayout::CountOf(attrib.type), GL_FLOAT,
						attrib.normalized, attrib.relative_offset);
					break;
				case GL_INT:
				case GL_BOOL:
					glVertexArrayAttribIFormat(m_VAO, Attrib_Index, BufferLayout::CountOf(attrib.type), GL_INT,
						attrib.relative_offset);
					break;
				default: assert(0);
			}
			++Attrib_Index;
		}
	}

	void LineRenderer::AttachInstancedBuffer(const BufferLayout& layout)
	{
		const uint32_t startingIndex = Attrib_Index;
		uint32_t size_of_one_vertex = 0;

		for (const auto& attrib : layout)
			size_of_one_vertex += BufferLayout::SizeOf(attrib.type);

		const size_t nVertices = InstancedVBO_Size / size_of_one_vertex;

		for (const auto& attrib : layout)
		{
			glEnableVertexArrayAttrib(m_VAO, Attrib_Index);
			glVertexArrayVertexBuffer(m_VAO, Attrib_Index, m_InstancedVBO, attrib.offset * nVertices, attrib.stride);
			switch (GLType(attrib.type))
			{
				case GL_FLOAT:
					glVertexArrayAttribFormat(m_VAO, Attrib_Index, BufferLayout::CountOf(attrib.type), GL_FLOAT,
						attrib.normalized, attrib.relative_offset);
					break;
				case GL_INT:
				case GL_BOOL:
					glVertexArrayAttribIFormat(m_VAO, Attrib_Index, BufferLayout::CountOf(attrib.type), GL_INT,
						attrib.relative_offset);
					break;
				default: assert(0);
			}
			++Attrib_Index;
		}
		
		for (auto i = startingIndex; i < Attrib_Index; ++i)
			glVertexArrayBindingDivisor(m_VAO, i, 1);
	}

	void LineRenderer::ResizeVBO(void* data, uint32_t size)
	{
		if (VBO_Size < size)
			glNamedBufferData(m_VBO, size, data, GL_DYNAMIC_DRAW);
		else
			glNamedBufferSubData(m_VBO, 0, size, data);
		VBO_Size = size;
	}

	void LineRenderer::ResizeInstanced(void* data, uint32_t size)
	{
		if (InstancedVBO_Size < size)
			glNamedBufferData(m_InstancedVBO, size, data, GL_DYNAMIC_DRAW);
		else
			glNamedBufferSubData(m_InstancedVBO, 0, size, data);
		InstancedVBO_Size = size;
	}

	void LineRenderer::ResizeIndirect(void* data, uint32_t size)
	{
		if (IndirectVBO_Size < size)
			glNamedBufferData(m_IndirectVBO, size, data, GL_DYNAMIC_DRAW);
		else
			glNamedBufferSubData(m_IndirectVBO, 0, size, data);
		IndirectVBO_Size = size;
	}
}