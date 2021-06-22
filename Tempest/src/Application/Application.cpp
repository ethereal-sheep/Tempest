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

	void Application::OnEngineInit()
	{
		// init Engine stuff first
		Logger::Init();
		Service<RenderSystem>::Register();
		LOG("Initializing Tempest Engine");
		LOG(glGetString(GL_VERSION));
		Service<thread_pool>::Register(thread::hardware_concurrency());

		OnInit();
	}

	void Application::OnEngineUpdate()
	{
		// Update Engine stuff first
		Service<RenderSystem>::Get().GetCamera().Update();

		OnUpdate();
	}

	void Application::OnEngineRender()
	{
		// render engine stuff first
		//RenderSystem ren;
		//Service<RenderSystem>::Get().TestRender(m_width, m_height);
		Service<RenderSystem>::Get().StartFrame();
		Service<RenderSystem>::Get().EndFrame();
		//OnRender();
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
	}

	void Application::OnKeyRelease(uint8_t key)
	{

	}
}
