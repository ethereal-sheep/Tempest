#include "RenderSystem.h"

namespace Tempest
{
	RenderSystem::RenderSystem(uint32_t width, uint32_t height) : m_Framebuffer(width, height)
	{
		m_Renderer.SetViewport(0, 0, width, height);
		m_Pipeline.cameras.push_back(Camera{});
		meshes.emplace(std::pair(Shape::SHAPE_CUBE, Mesh::GenerateIndexedCube(1, 1)));
		meshes.emplace(std::pair(Shape::SHAPE_SPHERE, Mesh::GenerateIndexedSphere(1, 16, 16)));
		meshes.emplace(std::pair(Shape::SHAPE_PLANE, Mesh::GenerateIndexedPlane(1, 1)));

		Transform transform;
		transform.position = vec3(-0.5f, 0.f, 0.f);
		transform.rotation = quat(0.f, 0.f, 0.f, 0.f);
		transform.scale = vec3(0.1f, 0.1f, 0.1f);
		Submit(Shape::SHAPE_SPHERE, transform);

		Transform transform2;
		transform2.position = vec3(0.5f, 0.f, 0.f);
		transform2.rotation = quat(0.f, 0.f, 0.f, 0.f);
		transform2.scale = vec3(0.1f, 0.1f, 0.1f);
		//SubmitSprite(Shape::SHAPE_CUBE, transform2);

		//m_Pipeline.meshes.push_back(Mesh::GenerateIndexedCube(1, 1));
		//m_Pipeline.meshes.emplace_back(Mesh::GenerateIndexedCube(1, 1));
		//m_Pipeline.meshes.emplace_back(Mesh::GenerateIndexedSphere(1, 16, 16));
		//
		//glm::mat4 transform(1.f);
		//transform = glm::translate(transform, glm::vec3(0.f, 0.f, 0.f));
		//transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
		//transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
		//transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		//transform = glm::scale(transform, glm::vec3(.1f, .1f, .1f));
		//
		//glm::mat4 transform2(1.f);
		//transform2 = glm::translate(transform2, glm::vec3(-0.5f, 0.f, 0.f));
		//transform2 = glm::rotate(transform2, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
		//transform2 = glm::rotate(transform2, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
		//transform2 = glm::rotate(transform2, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		//transform2 = glm::scale(transform2, glm::vec3(.25f, .25f, .25f));
		//
		//glm::mat4 transform3(1.f);
		//transform3 = glm::translate(transform3, glm::vec3(0.5f, 0.f, 0.f));
		//transform3 = glm::rotate(transform3, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
		//transform3 = glm::rotate(transform3, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
		//transform3 = glm::rotate(transform3, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		//transform3 = glm::scale(transform3, glm::vec3(.25f, .25f, .25f));
		//
		//transforms.push_back(transform);
		//transforms.push_back(transform2);
		//transforms.push_back(transform3);
	}

	void RenderSystem::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
			return;

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
		//m_LineRenderer.SubmitBuffer();
		//m_LineRenderer.ClearBuffer();

		m_Framebuffer.Attach();


		System_Begin();
		System_Draw();
		m_Framebuffer.Detach();
		m_Framebuffer.Draw();

		//Line test_line{ glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f) };
		//m_LineRenderer.Submit(test_line, glm::vec4(1.f, 0.f, 0.f, 0.f));
		//line_Shader.Bind();
		//line_Shader.SetMat4fv(m_Pipeline.cameras[0].GetViewProjectionMatrix(), "viewProjMatrix");
		//m_Renderer.SetLineWidth(2.f);
		//m_Renderer.MultiDrawArraysIndirect(DrawMode::LINE, *m_Pipeline.m_Debug.vao, *m_Pipeline.m_Debug.indirect);
		//m_Renderer.SetLineWidth(1.f);
		//line_Shader.Unbind();
	}

	void RenderSystem::EndFrame()
	{
		glFlush();
		//System_End();
		//System_Reset();
	}

	void RenderSystem::System_Begin()
	{
		m_Renderer.EnableDepthMask(true);
		m_Renderer.EnableDepthTest(true);
		m_Renderer.EnableCulling(false, false, false);
		m_Renderer.EnableBlend(true);
		//m_Renderer.EnableCulling(true, true, true);
		//m_Renderer.SetPolygonMode(PolyMode::FILL);

		m_Renderer.ClearColour(0.5f, 0.5f, 0.5f, 0.0f);
		m_Renderer.ClearColorDepth();
	}

	void RenderSystem::System_Draw()
	{
		//quad_Shader.Bind();
		//quad_Shader.SetMat4fv(m_Pipeline.cameras.front().GetViewMatrix(), "ViewMatrix");
		//quad_Shader.SetMat4fv(m_Pipeline.cameras.front().GetProjectionMatrix(), "ProjectionMatrix");
		//quad_Shader.SetMat4fv(transforms[0], "ModelMatrix");
		//m_Renderer.DrawElements(DrawMode::TRIANGLES, m_Pipeline.meshes[0].GetVAO(), m_Pipeline.meshes[0].GetIBO(), DrawType::UNSIGNED_INT);
		//quad_Shader.Unbind();
		//
		//quad_Shader.Bind();
		//quad_Shader.SetMat4fv(m_Pipeline.cameras.front().GetViewMatrix(), "ViewMatrix");
		//quad_Shader.SetMat4fv(m_Pipeline.cameras.front().GetProjectionMatrix(), "ProjectionMatrix");
		//quad_Shader.SetMat4fv(transforms[1], "ModelMatrix");
		//m_Renderer.DrawElements(DrawMode::TRIANGLES, m_Pipeline.meshes[0].GetVAO(), m_Pipeline.meshes[0].GetIBO(), DrawType::UNSIGNED_INT);
		//quad_Shader.Unbind();
		//
		//quad_Shader.Bind();
		//quad_Shader.SetMat4fv(m_Pipeline.cameras.front().GetViewMatrix(), "ViewMatrix");
		//quad_Shader.SetMat4fv(m_Pipeline.cameras.front().GetProjectionMatrix(), "ProjectionMatrix");
		//quad_Shader.SetMat4fv(transforms[2], "ModelMatrix");
		//m_Renderer.DrawElements(DrawMode::TRIANGLES, m_Pipeline.meshes[1].GetVAO(), m_Pipeline.meshes[1].GetIBO(), DrawType::UNSIGNED_INT);
		//quad_Shader.Unbind();

		if (m_Pipeline.shapes.empty())
			return;

		quad_Shader.Bind();

		for (int i = 0; i < m_Pipeline.shapes.size(); ++i)
		{
			quad_Shader.SetMat4fv(m_Pipeline.cameras.front().GetViewMatrix(), "ViewMatrix");
			quad_Shader.SetMat4fv(m_Pipeline.cameras.front().GetProjectionMatrix(), "ProjectionMatrix");
			quad_Shader.SetMat4fv(m_Pipeline.transforms[i], "ModelMatrix");
			// ithink meshes don have to be in pipeline but u decide
			m_Renderer.DrawElements(
				DrawMode::TRIANGLES, 
				meshes.at(m_Pipeline.shapes[i]).GetVAO(), 
				meshes.at(m_Pipeline.shapes[i]).GetIBO(), 
				DrawType::UNSIGNED_INT);
		}
		quad_Shader.Unbind();
	}

	void RenderSystem::System_End()
	{
		m_Renderer.Flush();
	}

	void RenderSystem::System_Reset()
	{
		//static int count = 0;
		//if (count++ % 2 == 0)
		//{
		//	m_Pipeline.meshes.clear();
		//	m_Pipeline.transforms.clear();
		//}
	}

	uint32_t RenderSystem::GetColourBuffer() const
	{
		return m_Framebuffer.ColourBuffer();
	}

	//void RenderSystem::SubmitSprite(Shape shape, Transform& transform)
	//{
	//	switch (shape)
	//	{
	//		case Shape::SHAPE_CUBE:
	//			//m_Pipeline.meshes.emplace_back(Mesh::GenerateIndexedCube(1, 1));
	//			m_Pipeline.transforms.push_back(ModelMatrix(transform));
	//			break;
	//
	//		case Shape::SHAPE_SPHERE:
	//			m_Pipeline.meshes.emplace_back(Mesh::GenerateIndexedSphere(1, 16, 16));
	//			m_Pipeline.transforms.push_back(ModelMatrix(transform));
	//			break;
	//
	//		case Shape::SHAPE_PLANE:
	//			//m_Pipeline.meshes.emplace_back(Mesh::GenerateIndexedPlane(1, 1));
	//			m_Pipeline.transforms.push_back(ModelMatrix(transform));
	//			break;
	//	}
	//}

	void RenderSystem::Submit(Shape shape, Transform& transform)
	{
		//switch (shape)
		//{
		//	case Shape::SHAPE_CUBE:
		//		//some new vector
		//		m_Pipeline.shapes.push_back(shape);
		//		// den when u draw
		//		m_Pipeline.transforms.push_back(ModelMatrix(transform));
		//		break;
		//
		//	case Shape::SHAPE_SPHERE:
		//		m_Pipeline.meshes.emplace_back(Mesh::GenerateIndexedSphere(1, 16, 16));
		//		m_Pipeline.transforms.push_back(ModelMatrix(transform));
		//		break;
		//
		//	case Shape::SHAPE_PLANE:
		//		//m_Pipeline.meshes.emplace_back(Mesh::GenerateIndexedPlane(1, 1));
		//		m_Pipeline.transforms.push_back(ModelMatrix(transform));
		//		break;
		//}
		// next time the component will hold the shared_ptr to its own mesh
		// so submit will just be

		// m_Pipeline.shapes.push_back(shape);
		// m_Pipeline.transforms.push_back(ModelMatrix(transform)); // liddat only
		// also submit sprite is a wrong name, there is no sprite to submit
		// just say submit
		// den do overloading for multiple cases

		
		m_Pipeline.shapes.push_back(shape);
		m_Pipeline.transforms.push_back(ModelMatrix(transform));
	}

	glm::mat4 RenderSystem::ModelMatrix(Transform& transform)
	{
		glm::mat4 model(1.f);
		
		glm::mat4 translate = glm::translate(glm::vec3(transform.position.x, transform.position.y, transform.position.z));
		glm::mat4 rotate(transform.rotation);
		glm::mat4 scale = glm::scale(glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));

		model = translate * rotate * scale;
		return model;
	}

}