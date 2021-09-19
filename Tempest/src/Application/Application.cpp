#include "Application.h"

#include "Util.h"
#include "Events/Test/event_test.h"

#include "Scripting/Test/test_scripting.h"
#include "Scripting/GMS.h"

#include "ECS/ECS.h"
#include "ECS/Test/test_entity.h"
#include "Util/view.h"
#include "Util/thread_pool.h"
#include "Util/Service.h"
#include "Audio/AudioEngine.h"

#include "Graphics/Basics/RenderSystem.h"
#include "Graphics/Test/TestModel.h"

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
		Service<RenderSystem>::Register(m_width, m_height);
		LOG("Initializing Tempest Engine");
		Service<thread_pool>::Register(thread::hardware_concurrency());
		Service<EventManager>::Register();

		AudioEngine::Init();

		OnInit();
	}

	void Application::OnEngineUpdate()
	{
		// Update Engine stuff first
		AudioEngine::Update();
		Service<RenderSystem>::Get().GetCamera().Update();
		OnUpdate();
		
		//testing_physics_7_2();
	}

	void Application::OnEngineRender()
	{

		Service<RenderSystem>::Get().Draw();
		//Service<RenderSystem>::Get().EndFrame();
		OnRender();
	}

	void Application::OnEngineExit()
	{
		// exit application stuff first
		OnExit();

		// then exit engine stuff

		AudioEngine::Shutdown();
	}

	void Application::OnKeyPress(uint8_t key, uint8_t repeat)
	{
		//Service<RenderSystem>::Get().GetCamera().OnKeyPress(key);
		if(key == 'G')
			Service<RenderSystem>::Get().RenderGrid(true);

		if (key == 'A')
			Service<RenderSystem>::Get().SubmitModel("Models/HandgunB.fbx", Tempest::tc::Transform{});
		(void)repeat;
	}

	void Application::OnKeyRelease(uint8_t key)
	{
		(void)key;
	}

	void Application::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
			return;

		if(Service<RenderSystem>::GetIf())
			Service<RenderSystem>::Get().Resize(width, height);

		m_width = width;
		m_height = height;
	}
}
