#pragma once
#include "TMath.h"
#include "Graphics/Basics/VertexBuffer.h"
#include "Graphics/Basics/VertexArray.h"
#include "Core.h"
#include "Graphics/OpenGL/Shader.h"


namespace Tempest
{
	struct Line
	{
		glm::vec3 p0;
		glm::vec3 p1;
	};

	struct AABB
	{
		glm::vec3 min;
		glm::vec3 max;
	};

	class LineRenderer
	{

	private:
		VertexArray m_VAO;
		VertexBuffer m_VBO;
		VertexBuffer m_InstancedVBO;
		VertexBuffer m_Indirect;

		std::vector<glm::vec3> m_VertexBuffer;
		std::vector<glm::vec4> m_InstanceBuffer;
		std::vector<DrawArrayIndirect> m_IndirectCommand;

		uint32_t m_CurVertex = 0;

	public:

		LineRenderer();

		void Submit(const Line& line, const glm::vec4& color);
		void Submit(const AABB& box, const glm::vec4& color);

		void ClearBuffer();
		void SubmitBuffer();
		void Render(const glm::mat4& vpm, const tuptr<Shader>& shader);

		size_t GetVertexCount() const;
		VertexArray& GetVAO();
		VertexBuffer& GetIndirect();
	};
}