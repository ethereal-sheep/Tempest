#pragma once
#include "Graphics/OpenGL/Renderer.h"
#include "Graphics/OpenGL/RenderPipeline.h"
#include "Graphics/OpenGL/Shader.h"
#include "Graphics/OpenGL/Camera.h"


namespace Tempest
{
	class RenderSystem
	{
	private:

		Renderer m_Renderer;
		RenderPipeline m_Pipeline;

		Shader quad_Shader{ "Graphics/Shaders/Quad_Vert.glsl", "Graphics/Shaders/Quad_Frag.glsl" };

	public:

		RenderSystem();
		void TestRender(uint32_t width, uint32_t height);
	};
}