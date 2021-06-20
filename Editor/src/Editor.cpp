
#include "Core.h"
#include "Util.h"
#include "Tracy.hpp"

#include "Application/EntryPoint.h"

#include <iostream>
#include <thread>


#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/ImGuiFileBrowser.h"
#include "Window/WindowManager.h"
#include "Window/MenuBar.h"

#include "Font.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Tempest
{
	void init_font();
	void init_style();

	class Editor : public Application
	{
		UI::WindowManager m_WindowManager;
		imgui_addons::ImGuiFileBrowser m_FileExplorer{};
		UI::MenuBar m_MenuBar;
	public:
		Editor()
			: Application(1600, 900, L"Editor") {}
		
		void OnInit() override
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGui::StyleColorsDark();
			ImGui_ImplWin32_Init(AppHandler::GetContext()->GetHWND());
			ImGui_ImplOpenGL3_Init("#version 460");
			ImGuiIO& io = ImGui::GetIO();

			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			ImGuiStyle& style = ImGui::GetStyle();

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			init_font();
			init_style();

			m_WindowManager.Initialize();
			m_WindowManager.StartupWindows();
		}

		void OnUpdate() override
		{

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

			bool popupImport = false;
			bool popupImportSuccess = false;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			if (ImGui::Begin("Main", nullptr, window_flags))
			{
				ImGui::PopStyleVar();

				ImGui::PopStyleVar(2);

				if (ImGui::BeginMenuBar())
				{
					if (ImGui::MenuItem(ICON_FA_MAP_PIN " Font Awesome"))
					{

					}
					ImGui::PushFont(FONT_FAD);
					if (ImGui::MenuItem(ICON_FAD_RANDOM_1DICE ICON_FAD_RANDOM_2DICE))
					{

					}

					ImGui::PopFont();
					ImGui::PushFont(FONT_FK);
					if (ImGui::MenuItem(ICON_FK_MAP_PIN " Fork Awesome"))
					{

					}
					ImGui::PopFont();
					ImGui::PushFont(FONT_KI);
					if (ImGui::MenuItem(ICON_KI_ARROW_BOTTOM " Kenny"))
					{

					}
					ImGui::PopFont();
					ImGui::PushFont(FONT_BOLD);
					if (ImGui::MenuItem("HELLO"))
					{

					}
					ImGui::PopFont();
					ImGui::EndMenuBar();
				}
				
				// DockSpace
				ImGuiIO& io = ImGui::GetIO();
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), 0);
				}
				
			}
			ImGui::End();
			m_WindowManager.InitMenuBar(m_MenuBar);
			m_WindowManager.DisplayWindows();
			m_MenuBar.Show();

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

	void init_style()
	{
		auto& style = ImGui::GetStyle();

		ImVec4* colors = style.Colors;
		colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.85f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.14f, 0.92f);
		colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.43f, 0.43f, 0.43f, 0.39f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.47f, 0.47f, 0.69f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.42f, 0.41f, 0.64f, 0.69f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.27f, 0.27f, 0.54f, 0.83f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.32f, 0.32f, 0.63f, 0.87f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.41f, 0.39f, 0.80f, 0.60f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
		colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.41f, 0.39f, 0.80f, 0.60f);
		colors[ImGuiCol_Button] = ImVec4(0.35f, 0.40f, 0.61f, 0.62f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.48f, 0.71f, 0.79f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.46f, 0.54f, 0.80f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
		colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.60f, 0.60f, 0.70f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.90f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.78f, 0.82f, 1.00f, 0.60f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.82f, 1.00f, 0.90f);
		colors[ImGuiCol_Tab] = ImVec4(0.34f, 0.34f, 0.68f, 0.79f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
		colors[ImGuiCol_TabActive] = ImVec4(0.40f, 0.40f, 0.73f, 0.84f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.28f, 0.28f, 0.57f, 0.82f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.35f, 0.35f, 0.65f, 0.84f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.40f, 0.40f, 0.90f, 0.31f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.27f, 0.27f, 0.38f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.45f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.26f, 0.26f, 0.28f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);


		style.WindowRounding = 5;
		style.ChildRounding = 5;
		style.FrameRounding = 5;
		style.PopupRounding = 5;
		style.ScrollbarRounding = 5;
		style.GrabRounding = 5;
		style.TabRounding = 5;

		style.WindowTitleAlign = { 0.5f, 0.5f };
		style.WindowMenuButtonPosition = ImGuiDir_Right;
		
		style.ButtonTextAlign = { 0.5f, 0.5f };
		style.SelectableTextAlign = { 0.0f, 0.5f };

		style.WindowBorderSize = 0;
		style.FrameBorderSize = 0;
		style.PopupBorderSize = 0;
	}

	void init_font()
	{
		auto io = ImGui::GetIO();
		ImFontConfig config;
		config.MergeMode = true;
		config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
		config.PixelSnapH = true;

		tpath def_f = tpath("Fonts") / default_font;
		tpath bold_f = tpath("Fonts") / bold_font;
		def_f.replace_extension(".ttf");
		bold_f.replace_extension(".ttf");


		static const ImWchar fa_range[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		io.Fonts->AddFontFromFileTTF(def_f.string().c_str(), font_text_size); // change this to change default font
		io.Fonts->AddFontFromFileTTF("Fonts/fa-regular-400.ttf", font_icon_size, &config, fa_range);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-solid-900.ttf", font_icon_size, &config, fa_range);

		io.Fonts->AddFontFromFileTTF(bold_f.string().c_str(), font_text_size);

		static const ImWchar fk_range[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
		io.Fonts->AddFontFromFileTTF(def_f.string().c_str(), font_text_size);
		io.Fonts->AddFontFromFileTTF("Fonts/forkawesome-webfont.ttf", font_icon_size, &config, fk_range);

		config.PixelSnapH = true;
		config.GlyphOffset = { 0,2 };
		static const ImWchar fad_range[] = { ICON_MIN_FAD, ICON_MAX_FAD, 0 };
		io.Fonts->AddFontFromFileTTF(def_f.string().c_str(), font_text_size);
		io.Fonts->AddFontFromFileTTF("Fonts/fontaudio.ttf", font_icon_size, &config, fad_range);

		static const ImWchar ki_range[] = { ICON_MIN_KI, ICON_MAX_KI, 0 };
		io.Fonts->AddFontFromFileTTF(def_f.string().c_str(), font_text_size);
		io.Fonts->AddFontFromFileTTF("Fonts/kenney-icon-font.ttf", font_icon_size, &config, ki_range);

		def = io.Fonts->Fonts[0];
		bold = io.Fonts->Fonts[1];
		fk = io.Fonts->Fonts[2];
		fad = io.Fonts->Fonts[3];
		ki = io.Fonts->Fonts[4];
	}
}
