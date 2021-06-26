#include "Application.h"

#include "Util.h"
#include "Events/Test/event_test.h"

#include "Graphics/OpenGL/RenderSystem.h"

#include "Scripting/Test/test_scripting.h"
#include "Scripting/GMS.h"

#include "ECS/ECS.h"
#include "ECS/Test/test_entity.h"
#include "Physics/Test/test_physics.h"
#include "Util/view.h"
#include "Util/thread_pool.h"
#include "Util/Service.h"

namespace Tempest
{

	Application::Application(uint32_t width, uint32_t height, std::wstring name)
		: m_width{ width }
		, m_height{ height }
		, m_title{ std::move(name) }
	{
		
	}

	struct Test
	{
		Camera cam;
		Renderer renderer;
		Shader quad_shader{ "Shaders/Quad_Vert.glsl", "Shaders/Quad_Frag.glsl" };
		FBO fbo{ 1600,900 };
		Mesh mesh = Mesh::GenerateIndexedCube(1, 1);
		glm::mat4 transform;
	};

	void Application::OnEngineInit()
	{
		// init Engine stuff first
		Logger::Init();
		Service<RenderSystem>::Register(m_width, m_height);
		Service<Test>::Register();
		//Service<RenderSystem>::Get().SetViewport(m_width, m_height);
		LOG("Initializing Tempest Engine");
		LOG(glGetString(GL_VERSION));
		Service<thread_pool>::Register(thread::hardware_concurrency());
		Service<EventManager>::Register();

		OnInit();
		testing_physics();
		//testing_physics_7_1();
	}




	void Application::OnEngineUpdate()
	{
		// Update Engine stuff first
		Service<RenderSystem>::Get().GetCamera().Update();
		Service<Test>::Get().cam.Update();
		OnUpdate();
		
		//testing_physics_7_2();
	}

	void Application::OnEngineRender()
	{
		// render engine stuff first
		//RenderSystem ren;
		//Service<RenderSystem>::Get().TestRender(m_width, m_height);
		
		// this is bad because Mesh is bad (see System_Draw)
		Service<RenderSystem>::Get().StartFrame();
		Service<RenderSystem>::Get().EndFrame();
		{
			//{
			//	auto& test = Service<Test>::Get();
			//
			//	test.cam.SetViewport(0, 0, 1600, 900);
			//	test.renderer.SetViewport(0, 0, 1600, 900);
			//
			//	test.fbo.Attach();
			//
			//	test.renderer.EnableDepthMask(true);
			//	test.renderer.EnableDepthTest(true);
			//	test.renderer.EnableBlend(true);
			//	test.renderer.EnableCulling(true, true, true);
			//	test.renderer.ClearColour(0.5f, 0.5f, 0.5f, 0.0f);
			//
			//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//
			//
			//	{
			//
			//		test.transform = glm::mat4{ 1.f };
			//		test.transform = glm::translate(test.transform, glm::vec3(0.25f, 0.f, 0.f));
			//		test.transform = glm::scale(test.transform, glm::vec3(.1f, .1f, .1f));
			//
			//		test.quad_shader.Bind();
			//
			//		test.quad_shader.SetMat4fv(test.cam.GetViewMatrix(), "ViewMatrix");
			//		test.quad_shader.SetMat4fv(test.cam.GetProjectionMatrix(), "ProjectionMatrix");
			//		test.quad_shader.SetMat4fv(test.transform, "ModelMatrix");
			//
			//		test.mesh.GetVAO().Bind();
			//		test.mesh.GetIBO().Bind();
			//		glDrawElements(GL_TRIANGLES, test.mesh.GetIBO().GetCount(), GL_UNSIGNED_INT, nullptr);
			//		test.mesh.GetVAO().Unbind();
			//		test.mesh.GetIBO().Unbind();
			//
			//		test.quad_shader.Unbind();
			//	}
			//	{
			//
			//		test.transform = glm::mat4{ 1.f };
			//		test.transform = glm::translate(test.transform, glm::vec3(-0.25f, 0.f, 0.f));
			//		test.transform = glm::scale(test.transform, glm::vec3(.1f, .1f, .1f));
			//
			//		test.quad_shader.Bind();
			//
			//		test.quad_shader.SetMat4fv(test.cam.GetViewMatrix(), "ViewMatrix");
			//		test.quad_shader.SetMat4fv(test.cam.GetProjectionMatrix(), "ProjectionMatrix");
			//		test.quad_shader.SetMat4fv(test.transform, "ModelMatrix");
			//
			//		/* THIS BREAKS (passed by value not ref so it creates a new vertex array then destroys it after you set it
			//			working previously is undefined behaviour) */
			//		//test.renderer.DrawElements(DrawMode::TRIANGLES, test.mesh.GetVAO(), test.mesh.GetIBO(), DrawType::UNSIGNED_INT);
			//		test.mesh.GetVAO().Bind();
			//		test.mesh.GetIBO().Bind();
			//		glDrawElements(GL_TRIANGLES, test.mesh.GetIBO().GetCount(), GL_UNSIGNED_INT, nullptr);
			//		test.mesh.GetVAO().Unbind();
			//		test.mesh.GetIBO().Unbind();
			//
			//		test.quad_shader.Unbind();
			//	}
			//
			//
			//
			//
			//
			//	//
			//
			//
			//	test.fbo.Detach();
			//	//glDisable(GL_DEPTH_TEST);
			//	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			//	//glClear(GL_COLOR_BUFFER_BIT);
			//
			//	test.fbo.Draw();
			//
			//	glFlush();
			//
			//}
		}
		OnRender();
	}

	void Application::OnEngineExit()
	{
		// exit application stuff first
		OnExit();

		// then exit engine stuff
	}

	void Application::OnKeyPress(uint8_t key, uint8_t repeat)
	{
		Service<RenderSystem>::Get().GetCamera().OnKeyPress(key);
		(void)repeat;
	}

	void Application::OnKeyRelease(uint8_t key)
	{
		(void)key;
	}

	void Application::Resize(uint32_t width, uint32_t height)
	{
		if(Service<RenderSystem>::GetIf())
			Service<RenderSystem>::Get().Resize(width, height);

		m_width = width;
		m_height = height;
	}
}
