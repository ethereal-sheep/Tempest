/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Core.h"
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
#include "Profiler/Profiler.h"

// Testing 
#include "Particles/Particles_3D/EmitterSystem_3D.h"
#include "FrameRate/FrameRateManager.h"
#include "Animation/AnimMultithreadHelper.h"



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
		Profile::Profiler::Init();

		Service<RenderSystem>::Register(m_width, m_height);
		LOG("Initializing Tempest Engine");
		Service<thread_pool>::Register(std::thread::hardware_concurrency());
		Service<EventManager>::Register();

		Service<AnimMultithreadHelper>::Register();
		Service<AnimMultithreadHelper>::Get().init();

		auto v = glm::vec3{ .33f, -1.f, -.33f };
		v = glm::normalize(v);
		Service<RenderSystem>::Get().dir_lights[0].Direction = v;
		Service<RenderSystem>::Get().gammaValue = 1.0f;

		// Profiler

		AudioEngine::Init();

		OnInit();
	}

	void Application::OnEngineUpdate()
	{
		T_FrameRateManager.FrameStart();
		// Update Engine stuff first

		Profile::Profiler::FrameStart();
		PROFILER_MARKER(UPDATE);

		// Update Engine stuff first
		NAMED_PROFILER_MARKER(Audio, AUDIO);
		AudioEngine::Update();
		END_NAMED_MARKER(Audio);

		Service<RenderSystem>::Get().GetCamera().Update();

		OnUpdate();
		
		//testing_physics_7_2();

		T_FrameRateManager.FrameEnd();

		//LOG_INFO("DT: {0}", T_FrameRateManager.GetDT());
	}

	void Application::OnEngineRender()
	{
		PROFILER_MARKER(RENDER);

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
		Profile::Profiler::Shutdown();
		Service<AnimMultithreadHelper>::Get().shutdown();
	}

	void Application::OnKeyPress([[maybe_unused]] uint8_t key, [[maybe_unused]] uint8_t repeat)
	{
		//Service<RenderSystem>::Get().GetCamera().OnKeyPress(key);
		
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
