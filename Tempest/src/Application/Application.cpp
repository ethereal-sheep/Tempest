#include "Application.h"

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
