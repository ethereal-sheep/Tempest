#include "RenderSystem.h"

namespace Tempest
{
	RenderSystem::RenderSystem()
	{
		m_Pipeline.cameras.push_back(Camera{});
		m_Pipeline.meshes.push_back(Mesh::GenerateIndexedCube(1, 1));
	}

	void RenderSystem::TestRender(uint32_t width, uint32_t height)
	{
		//m_Renderer.SetViewport(0, 0, width, height);
		//m_Renderer.EnableDepthMask(true);
		//m_Renderer.EnableDepthTest(true);
		//m_Renderer.EnableCulling(false, false, false);
		//m_Renderer.EnableBlend(true);
		//m_Renderer.EnableCulling(true, true, true);
		//
		//m_Renderer.ClearColour(0.5f, 0.5f, 0.5f, 0.0f);
		//m_Renderer.Clear();
		////Camera camera;
		//
		//glm::mat4 transform(1.f);
		//transform = glm::translate(transform, glm::vec3(1.f, 1.f, 1.f));
		//transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
		//transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
		//transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		//transform = glm::scale(transform, glm::vec3(1.f, 1.f, 0.f));
		//
		//quad_Shader.Bind();
		//quad_Shader.SetMat4fv(transform, "ModelMatrix");
		//quad_Shader.SetMat4fv(m_Pipeline.cameras.front().GetViewMatrix(), "ViewMatrix");
		//quad_Shader.SetMat4fv(m_Pipeline.cameras.front().GetProjectionMatrix(), "ProjectionMatrix");
		//
		//Mesh quad = Tempest::Mesh::GenerateIndexedPlane(1, 1);
		//Mesh quad2 = Tempest::Mesh::GenerateIndexedPlane(1, 1);
		//m_Renderer.DrawElements(DrawMode::TRIANGLES, quad.GetVAO(), quad.GetIBO(), DrawType::UNSIGNED_INT);
		//m_Renderer.DrawElements(DrawMode::TRIANGLES, quad2.GetVAO(), quad2.GetIBO(), DrawType::UNSIGNED_INT);
		//m_Renderer.Flush();
		//quad_Shader.Unbind();
	}

	Camera& RenderSystem::GetCamera()
	{
		return m_Pipeline.cameras.front();
	}

	void RenderSystem::StartFrame()
	{
		System_Begin();
		System_Draw();
	}

	void RenderSystem::EndFrame()
	{
		System_End();
		System_Reset();
	}

	void RenderSystem::System_Begin()
	{
		//m_Renderer.SetViewport(0, 0, 1600, 900);
		m_Renderer.EnableDepthMask(true);
		m_Renderer.EnableDepthTest(true);
		m_Renderer.EnableCulling(false, false, false);
		//m_Renderer.EnableBlend(true);
		//m_Renderer.EnableCulling(true, true, true);

		m_Renderer.ClearColour(0.5f, 0.5f, 0.5f, 0.0f);
		m_Renderer.Clear();
	}

	void RenderSystem::System_Draw()
	{
		//glm::mat4 transform(1.f);
		//transform = glm::translate(transform, glm::vec3(1.f, 1.f, 1.f));
		//transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
		//transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
		//transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		//transform = glm::scale(transform, glm::vec3(1.f, 1.f, 0.f));

		//quad_Shader.Bind();
		//quad_Shader.SetMat4fv(transform, "ModelMatrix");
		//quad_Shader.SetMat4fv(m_Pipeline.cameras.front().GetViewMatrix(), "ViewMatrix");
		//quad_Shader.SetMat4fv(m_Pipeline.cameras.front().GetProjectionMatrix(), "ProjectionMatrix");

		for(auto& i : m_Pipeline.meshes)
			m_Renderer.DrawElements(DrawMode::TRIANGLES, i.GetVAO(), i.GetIBO(), DrawType::UNSIGNED_INT);
		//quad_Shader.Unbind();
	}

	void RenderSystem::System_End()
	{
		//m_Renderer.Flush();
	}

	void RenderSystem::System_Reset()
	{
		//m_Pipeline.meshes.clear();
		m_Pipeline.sprites.clear();
	}
}