#include "Application.h"

#include "Util.h"
#include "Instance/Instances/RuntimeInstance.h"
#include "Events/Test/event_test.h"

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
		LOG("Initializing Tempest");
		const char* s = R"(S:\Development\Projects)";
		tpath path(s);

		RuntimeInstance i(path, MemoryStrategy{ DebugFlag::NONE });
		LOG("{}", i.has_debug());

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

		OnRender();
	}

	void Application::OnEngineExit()
	{
		// exit application stuff first
		OnExit();

		// then exit engine stuff
	}
}
