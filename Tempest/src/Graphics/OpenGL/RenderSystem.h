#pragma once
#include "Graphics/OpenGL/Renderer.h"
#include "Graphics/OpenGL/RenderPipeline.h"
#include "Graphics/OpenGL/Shader.h"
#include "Graphics/OpenGL/Camera.h"
#include "Graphics/OpenGL/FrameBuffer.h"
#include "Graphics/OpenGL/FBO.h"
#include "Graphics/OpenGL/LineRenderer.h"
#include "ECS/Components/Components.h"
#include "Graphics/OpenGL/Texture.h"
#include "Util.h"
#include <memory>
#include <Core.h>

namespace Tempest
{
	

	class RenderSystem
	{
	private:
		//tmap<string, Shader>> 
		LineRenderer m_LineRenderer;
		//Mesh m_Mesh1 = Mesh::GenerateIndexedCube(1, 1);
		//Mesh m_Mesh2 = Mesh::GenerateIndexedSphere(1, 16, 16);
		FBO m_Framebuffer;
		Renderer m_Renderer;
		RenderPipeline m_Pipeline;
		
		Shader quad_Shader{ "Shaders/Basic_vertex.glsl", "Shaders/Basic_fragment.glsl" };
		Shader line_Shader{ "Shaders/Line_vertex.glsl", "Shaders/Line_fragment.glsl" };

		tmap<Shape, Mesh> meshes;
		

		using Transform = Components::Transform;

	public:
		RenderSystem(uint32_t width, uint32_t height);
		void Resize(uint32_t width, uint32_t height);

		void StartFrame();
		void EndFrame();

		void System_Begin();
		void System_Draw();
		void System_End();
		void System_Reset();

		void Submit(Shape shape, Transform& transform);

		glm::mat4 ModelMatrix(Transform& transform);

		uint32_t GetColourBuffer() const;

		Camera& GetCamera();
		tuptr<Texture> tex = std::make_unique<Texture>("Assets/test_photo.png", false);

	};
}