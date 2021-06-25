#pragma once
#include "Graphics/OpenGL/Renderer.h"
#include "Graphics/OpenGL/RenderPipeline.h"
#include "Graphics/OpenGL/Shader.h"
#include "Graphics/OpenGL/Camera.h"
#include "Graphics/OpenGL/FrameBuffer.h"
#include "Graphics/OpenGL/FBO.h"
#include <memory>


namespace Tempest
{
	class RenderSystem
	{
	private:

		FBO m_Framebuffer;
		Renderer m_Renderer;
		RenderPipeline m_Pipeline;
		
		Shader quad_Shader{ "Shaders/Quad_Vert.glsl", "Shaders/Quad_Frag.glsl" };
		std::vector<glm::mat4> transforms;

	public:
		RenderSystem(uint32_t width, uint32_t height);
		void Resize(uint32_t width, uint32_t height);

		void StartFrame();
		void EndFrame();

		void System_Begin();
		void System_Draw();
		void System_End();
		void System_Reset();

		uint32_t GetColourBuffer() const;

		Camera& GetCamera();

	};
}