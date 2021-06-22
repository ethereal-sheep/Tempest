#pragma once
#include "Graphics/OpenGL/Renderer.h"
#include "Graphics/OpenGL/RenderPipeline.h"
#include "Graphics/OpenGL/Shader.h"
#include "Graphics/OpenGL/Camera.h"
#include <memory>


namespace Tempest
{
	class RenderSystem
	{
	private:

		Renderer m_Renderer;
		RenderPipeline m_Pipeline;
		//std::vector<std::shared_ptr<Shader>> shaders;
		Shader quad_Shader{ "Shaders/Quad_Vert.glsl", "Shaders/Quad_Frag.glsl" };
		std::vector<glm::mat4> transforms;
		//Shader sphere_Shader{ "Graphics/Shaders/Sphere_Vert.glsl", "Graphics/Shaders/Sphere_Frag.glsl" };

	public:

		RenderSystem();
		void TestRender(uint32_t width, uint32_t height);
		Camera& GetCamera();

		void StartFrame();
		void EndFrame();

		void System_Begin();
		void System_Draw();
		void System_End();
		void System_Reset();

	};
}