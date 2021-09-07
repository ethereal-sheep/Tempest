#pragma once
#include "Core.h"
#include "Graphics/Basics/VertexArray.h"
#include "Graphics/OpenGL/Shader.h"
#include "Graphics/Basics/ShadowMap.h"
#include "Graphics/Basics/Mesh.h"
#include "Util.h"

namespace Tempest
{
	class ShadowBuffer
	{
		uint32_t m_ID = 0;

		uint32_t m_vao = 0;
		uint32_t m_ibo = 0;
		uint32_t m_vbo = 0;
		uint32_t m_Count = 0;

		Shader m_Shader{ "Shaders/ShadowDepth_vertex.glsl", "Shaders/ShadowDepth_fragment.glsl" };

	public:

		ShadowBuffer();

		void AttachDepthAttachment(const ShadowMap& sm) const;
		void ClearAttachments();
		void Validate() const;
		void Draw(const glm::vec3& lightPos, const tvector<MeshCode>& sprites, const tvector<glm::mat4>& transforms, const tmap<MeshCode, tuptr<Mesh>>& meshes);

		void Bind() const;
		void Unbind() const;
		void ResizeViewport(const ShadowMap& sm);

		uint32_t GetID() const;
		uint32_t& GetIndexBuffer();
		uint32_t& GetVertexArray();

	};
}