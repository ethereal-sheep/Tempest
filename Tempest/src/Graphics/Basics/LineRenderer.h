#pragma once
#include "TMath.h"
#include "Util.h"
#include "Graphics/OpenGL/Shader.h"
#include "Graphics/OpenGL/BufferLayout.h"

namespace Tempest
{
	struct Line_Indirect
	{
		id_t vertexCount = 0;
		id_t instanceCount = 0;
		id_t baseVertex = 0;
		id_t baseInstance = 0;
	};

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
		id_t m_VAO = 0;
		id_t m_VBO = 0;
		id_t m_InstancedVBO = 0;
		id_t m_IndirectVBO = 0;

		id_t VBO_Size = 0;
		id_t InstancedVBO_Size = 0;
		id_t IndirectVBO_Size = 0;
		id_t Attrib_Index = 0;
		
		std::vector<glm::vec3> m_VertexBuffer;
		std::vector<glm::vec4> m_InstanceBuffer;
		std::vector<Line_Indirect> m_Indirect;
		
		id_t m_CurVertex = 0;

		Shader m_Shader{ "Shaders/Line_vertex.glsl", "Shaders/Line_fragment.glsl" };

	public:

		LineRenderer();
		~LineRenderer();

		LineRenderer(const LineRenderer&) = delete;
		LineRenderer(LineRenderer&&) = delete;
		LineRenderer& operator=(const LineRenderer&) = delete;		
		LineRenderer& operator=(LineRenderer&&) = delete;

		void Submit(const Line& line, const glm::vec4& color);
		void Submit(const AABB& box, const glm::vec4& color);

		void ClearBuffer();
		void SubmitBuffer();
		void Render(const glm::mat4& vproj);

	private:

		void AttachVertexBuffer(const BufferLayout& layout);
		void AttachInstancedBuffer(const BufferLayout& layout);
		void ResizeVBO(void* data, uint32_t size);
		void ResizeInstanced(void* data, uint32_t size);
		void ResizeIndirect(void* data, uint32_t size);
	};
}