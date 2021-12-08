/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Core.h"
#include "Graphics/Basics/VertexArray.h"
#include "Graphics/OpenGL/Shader.h"
#include "Graphics/Basics/ShadowMap.h"
#include "Graphics/Basics/Mesh.h"
#include "Util.h"
#include "Lights.h"
namespace Tempest
{
	class ShadowBuffer
	{
	public:

		uint32_t m_ID = 0;

		uint32_t m_vao = 0;
		uint32_t m_ibo = 0;
		uint32_t m_vbo = 0;
		uint32_t m_Count = 0;


		ShadowBuffer(uint32_t width = 2048, uint32_t height = 2048);
		
		void AttachDepthAttachment(const ShadowMap& sm) const;
		void ClearAttachments();
		void Validate() const;
		void Draw(const std::vector<Point_Light>& point_lights, const tvector<MeshCode>& sprites, const tvector<glm::mat4>& transforms, const tmap<MeshCode, tuptr<Mesh>>& meshes);

		void Bind() const;
		void Unbind() const;
		void ResizeViewport(const ShadowMap& sm);
		Shader m_Shader{ "Shaders/ShadowDepth_vertex.glsl", "Shaders/ShadowDepth_fragment.glsl" };
		uint32_t GetID() const;
		uint32_t& GetIndexBuffer();
		uint32_t& GetVertexArray();
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_depthMapFBO = 0;
		uint32_t depthMap = 0;
		uint32_t depthCubemap = 0;


	};
}