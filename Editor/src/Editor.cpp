
#include "Core.h"
#include "Tracy.hpp"

#include "Application/EntryPoint.h"

#include <iostream>
#include <thread>

#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"



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
			ImGui::StyleColorsDark();
			ImGuiIO& io = ImGui::GetIO();

			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			ImGuiStyle& style = ImGui::GetStyle();

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

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


			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoBringToFrontOnFocus |
				ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			if (ImGui::Begin("Main", nullptr, window_flags))
			{
				ImGui::PopStyleVar();

				ImGui::PopStyleVar(2);

				if (ImGui::BeginMenuBar())
				{
					if (ImGui::MenuItem("File"))
					{

					}
					if (ImGui::MenuItem("Edit"))
					{

					}
					if (ImGui::MenuItem("Help"))
					{

					}
					ImGui::EndMenuBar();
				}

				// DockSpace
				ImGuiIO& io = ImGui::GetIO();
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_AutoHideTabBar);
				}
			}

			ImGui::End();


			ImGui::ShowDemoWindow();

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
