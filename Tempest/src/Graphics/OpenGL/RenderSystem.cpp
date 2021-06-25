#include "RenderSystem.h"

namespace Tempest
{
	RenderSystem::RenderSystem(uint32_t width, uint32_t height) : m_Framebuffer(width, height)
	{
		m_Renderer.SetViewport(0, 0, width, height);
		m_Pipeline.cameras.push_back(Camera{});
		m_Pipeline.meshes.push_back(Mesh::GenerateIndexedSphere(1, 16, 16));
		m_Pipeline.meshes.push_back(Mesh::GenerateIndexedCube(1, 1));
		
	}

	void RenderSystem::Resize(uint32_t width, uint32_t height)
	{
		m_Framebuffer.Resize(width, height);
		m_Renderer.SetViewport(0, 0, width, height);

		for(auto& i : m_Pipeline.cameras)
		{
			i.SetViewport(0, 0, width, height);
		}
	}

	Camera& RenderSystem::GetCamera()
	{
		return m_Pipeline.cameras.front();
	}

	void RenderSystem::StartFrame()
	{
		//AttachFrameBuffer();
		m_Framebuffer.Attach();
		System_Begin();
		System_Draw();
		//AttachDefaultBuffer();
		m_Framebuffer.Detach();
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		m_Framebuffer.Draw();
		//frame_Shader.Bind();
		
		//glBindVertexArray(quadVAO);
		//glBindTexture(GL_TEXTURE_2D, m_FrameText->GetID());	// use the color attachment texture as the texture of the quad plane
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//DrawFrameBuffer();
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
		m_Renderer.EnableBlend(true);
		m_Renderer.EnableCulling(true, true, true);
		//m_Renderer.SetPolygonMode(PolyMode::FILL);

		m_Renderer.ClearColour(0.5f, 0.5f, 0.5f, 0.0f);
		//m_Renderer.Clear();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderSystem::System_Draw()
	{
		glm::mat4 transform(1.f);
		transform = glm::translate(transform, glm::vec3(0.f, 0.f, 0.f));
		transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
		transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
		transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		transform = glm::scale(transform, glm::vec3(.1f, .1f, .1f));

		glm::mat4 transform2(1.f);
		transform2 = glm::translate(transform2, glm::vec3(-0.5f, 0.f, 0.f));
		transform2 = glm::rotate(transform2, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
		transform2 = glm::rotate(transform2, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
		transform2 = glm::rotate(transform2, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		transform2 = glm::scale(transform2, glm::vec3(.25f, .25f, .25f));

		transforms.push_back(transform);
		transforms.push_back(transform2);

		auto dir = m_Pipeline.cameras.front().GetFront();

		quad_Shader.Bind();

		quad_Shader.SetMat4fv(m_Pipeline.cameras.front().GetViewMatrix(), "ViewMatrix");
		quad_Shader.SetMat4fv(m_Pipeline.cameras.front().GetProjectionMatrix(), "ProjectionMatrix");

		//for(auto& i : m_Pipeline.meshes)
		//	m_Renderer.DrawElements(DrawMode::TRIANGLES, i.GetVAO(), i.GetIBO(), DrawType::UNSIGNED_INT);

		for (int i = 0; i < m_Pipeline.meshes.size(); ++i)
		{
			quad_Shader.SetMat4fv(transforms[i], "ModelMatrix");
			m_Renderer.DrawElements(DrawMode::TRIANGLES, m_Pipeline.meshes[i].GetVAO(), m_Pipeline.meshes[i].GetIBO(), DrawType::UNSIGNED_INT);
		}
		quad_Shader.Unbind();
	}

	void RenderSystem::System_End()
	{
		m_Renderer.Flush();
	}

	void RenderSystem::System_Reset()
	{
		//m_Pipeline.meshes.clear();
		m_Pipeline.sprites.clear();
	}

	uint32_t RenderSystem::GetColourBuffer() const
	{
		return m_Framebuffer.ColourBuffer();
	}

}