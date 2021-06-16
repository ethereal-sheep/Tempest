#include "Application.h"

#include "Util.h"
#include "Instance/RuntimeInstance.h"
#include "Events/Test/event_test.h"

#include "Graphics/OpenGL/RenderSystem.h"

#include "Scripting/Test/test_scripting.h"
#include "Scripting/GMS.h"

#include "ECS/ECS.h"
#include "ECS/Test/test_entity.h"
#include "Util/view.h"

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
		LOG("Initializing Tempest Engine");


		//RuntimeInstance i(path, MemoryStrategy{ DebugFlag::DEBUG_STRICT });
		//EventManager em;

		testing_ecs();


		OnInit();
	}

	void Application::OnEngineUpdate()
	{
		// Update Engine stuff first


		OnUpdate();
	}

	void Application::OnEngineRender()
	{
		// render engine stuff first
		RenderSystem ren;
		//ren.TestRender(m_width, m_height);
		OnRender();
	}

	void Application::OnEngineExit()
	{
		// exit application stuff first
		OnExit();

		// then exit engine stuff
	}
}
