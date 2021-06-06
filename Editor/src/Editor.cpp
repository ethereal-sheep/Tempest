
#include "Core.h"
#include "Tracy.hpp"

#include "Application/EntryPoint.h"

#include <iostream>
#include <thread>

#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Tempest
{
	class Editor : public Application
	{

	public:
		Editor()
			: Application(1600, 900, L"Editor") {}

		void OnInit() override
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGui_ImplWin32_Init(AppHandler::GetContext()->GetHWND());
			ImGui_ImplOpenGL3_Init("#version 460");
		}

		void OnUpdate() override
		{
			//do
			//{
			//
			//} while (true);
		}

		void OnRender() override
		{

			/*! MUST BE AT THE START ---------------------------------------------*/
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			/*--------------------------------------------------------------------*/
			
			ImGui::Begin("Hello World");
			ImGui::ShowDemoWindow();
			ImGui::End();


			/*! MUST BE AT THE END -----------------------------------------------*/ 
			ImGui::Render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			/*--------------------------------------------------------------------*/
		}

		void OnExit() override
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		}

		LRESULT WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
		}
	};

	std::unique_ptr<Tempest::Application> CreateApplication()
	{
		return std::make_unique<Editor>();
	}
}
