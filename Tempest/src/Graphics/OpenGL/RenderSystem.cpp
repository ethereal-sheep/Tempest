#include "RenderSystem.h"

namespace Tempest
{
	RenderSystem::RenderSystem()
	{
		
	}

	void RenderSystem::TestRender(uint32_t width, uint32_t height)
	{
		m_Renderer.SetViewport(0, 0, width, height);
		m_Renderer.EnableDepthMask(true);
		m_Renderer.EnableDepthTest(true);
		m_Renderer.EnableCulling(false, false, false);
		m_Renderer.EnableBlend(true);
		m_Renderer.EnableCulling(true, true, true);

		m_Renderer.ClearColour(0.5f, 0.5f, 0.5f, 0.0f);
		m_Renderer.Clear();
		Camera camera;

		glm::mat4 transform(1.f);
		transform = glm::translate(transform, glm::vec3(1.f, 1.f, 0.f));
		transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
		transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
		transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		transform = glm::scale(transform, glm::vec3(1.f, 1.f, 0.f));

		quad_Shader.SetMat4fv(transform, "ModelMatrix");
		quad_Shader.SetMat4fv(camera.GetViewMatrix(), "ViewMatrix");
		quad_Shader.SetMat4fv(camera.GetProjectionMatrix(), "ProjectionMatrix");

		Mesh quad = Tempest::Mesh::GenerateIndexedPlane(1, 1);
		m_Renderer.DrawElements(DrawMode::TRIANGLES, quad.GetVAO(), quad.GetIBO(), DrawType::UNSIGNED_INT);
		m_Renderer.Flush();
	}
}