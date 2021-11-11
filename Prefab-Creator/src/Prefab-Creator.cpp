/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include <oleidl.h>

#include "Core.h"
#include "Util.h"

#include "Application/EntryPoint.h"

#include <iostream>
#include <thread>

#include "Extern/imgui/imgui_impl_opengl3.h"
#include "Extern/imgui/imgui_impl_win32.h"


#include "Audio/AudioEngine.h"
#include "Instance/CreatorInstance.h"
#include "Font.h"

// window includes
#include "Window/Popup/BottomRightOverlayPopup.h"
#include "Window/Popup/ErrorMsgPopup.h"
#include "Window/Popup/RetargetingPopup.h"
#include "Window/Popup/SettingsPopup.h"
#include "Window/FileBrowser/OpenFile.h"
#include "Window/FileBrowser/ImportBrowser.h"
#include "Window/FileBrowser/TargetingBrowser.h"
#include "Window/Home.h"
#include "Window/Viewport/ViewportWindow.h"
#include "Window/Menubar/Menubar.h"
#include "Window/Explorer/PrototypeExplorer.h"
#include "Window/Explorer/PrototypeInspector.h"

#include "ECS/Test/test_entity.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Tempest
{
	void init_font();
	void init_style();
	void init_file_dialog();

	

	class Prefab_Creator : public Application
	{

		CreatorInstance instance;

	public:
		Prefab_Creator()
			: Application(1600, 900, L"Prefab-Creator") {}

		void OnInit() override
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGui::StyleColorsDark();
			ImPlot::CreateContext();
			ImGuiIO& io = ImGui::GetIO();

			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

			ImGui_ImplWin32_Init(AppHandler::GetContext()->GetHWND());
			ImGui_ImplOpenGL3_Init("#version 460");

			init_font(); 
			init_style();
			init_file_dialog();

			// init windows
			instance.register_window<ViewportWindow>();
			instance.register_always<Menubar>();
			instance.register_always<BottomRightOverlayPopup>();
			instance.register_always<Home>();
			instance.register_always<ErrorMsgPopup>();
			instance.register_always<ImportBrowser>();
			instance.register_always<RetargetingPopup>();
			instance.register_always<SettingPopup>();
			instance.register_always<TargetingBrowser>();
			instance.register_always<PrototypeExplorer>();
			
			/*
				viewport window
					ball cam
					prefab editor


			*/ 
		
			//auto i = system("asset-compiler.exe");

			testing_prototype();

			instance.OnInit();
			auto& prefabMode = Service<RenderSystem>::Get().PREFABMODE;
			prefabMode = true;

		}

		void OnUpdate() override
		{
			// need to use dt
			// fps controller can be done in instance manager
			instance.OnUpdate(1.f);


		}

		void OnRender() override
		{
			/*! MUST BE AT THE START ---------------------------------------------*/
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGuizmo::BeginFrame();
			/*--------------------------------------------------------------------*/
			//ImGuiIO& io = ImGui::GetIO();
			//ImGuiViewport* viewport = ImGui::GetMainViewport();
			//ImGuiWindowFlags window_flags =
			//	ImGuiWindowFlags_NoDocking |
			//	ImGuiWindowFlags_NoTitleBar |
			//	ImGuiWindowFlags_NoCollapse |
			//	ImGuiWindowFlags_NoResize |
			//	ImGuiWindowFlags_NoMove |
			//	ImGuiWindowFlags_NoBringToFrontOnFocus |
			//	ImGuiWindowFlags_NoBackground |
			//	ImGuiWindowFlags_NoNavFocus;

			//ImGui::DockSpaceOverViewport(viewport, ImGuiDockNodeFlags_PassthruCentralNode);

			//ImGui::SetNextWindowPos(viewport->WorkPos);
			//ImGui::SetNextWindowSize(viewport->WorkSize);
			//ImGui::SetNextWindowViewport(viewport->ID);
			//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			//if (ImGui::Begin("Main", nullptr, window_flags))
			//{
			//	ImGui::PopStyleVar(3);
			//	// DockSpace
			//	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			//	{
			//		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			//		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), 0);
			//	}
			//	//ImGui::Image((ImTextureID)Service<RenderSystem>::Get().GetColourBuffer(), {1600, 900});

			//}
			//ImGui::End();
			instance.OnRender();

			/*! MUST BE AT THE END -----------------------------------------------*/
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			/*--------------------------------------------------------------------*/
		}

		void OnExit() override
		{

			instance.OnExit();


			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImPlot::DestroyContext();
			ImGui::DestroyContext();
		}

		LRESULT WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{

			switch (msg)
			{
				case WM_DROPFILES:
				{
					// do some shit here
					HDROP hDropInfo = (HDROP)wParam;
					WCHAR sItem[MAX_PATH];

					for (int i = 0; DragQueryFile(hDropInfo, i, sItem, sizeof(sItem)); i++)
					{
						//Is the item a file or a directory?
						if (GetFileAttributes(sItem) & FILE_ATTRIBUTE_DIRECTORY)
						{
							LOG("Folders not supported!");
						}
						else {
							SetFileAttributes(sItem, FILE_ATTRIBUTE_NORMAL); //Make file writable
							//DeleteFile(sItem);

							/************ TAKE NOTE ***************/
							// consider moving this into another place (in window class)
							// we just dispatch an event here
							// when calling asset-compiler
							// we need:
							// the filename (tpath s)
							// the destination of compiled resource (targetdir)
							// user should be able to choose target at the start and all subsequent will be 
							// at the dir

							char output[MAX_PATH];
							sprintf_s(output, "%ws", sItem);

							
							tpath s(output);
							// check if s is valid and resource have been targeted
							if (!fs::exists(s))
							{
								// error
								LOG_ERROR("Bad file!");
							}
							else if (!fs::exists(instance.get_full_path()))
							{
								// error
								LOG_ERROR("Resources folder not targeted!");
							}
							else
							{
								std::string cmd("Asset-Compiler\\Asset-Compiler.exe ");
								cmd += s.string();
								cmd += " ";
								cmd += instance.get_full_path().string();
								LOG("Starting compilation...");
								[[maybe_unused]] auto err = system(cmd.c_str());
								LOG("...compilation end");
							}

						}
					}
					DragFinish(hDropInfo);
				}
			}

			return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
		}
	};

	std::unique_ptr<Tempest::Application> CreateApplication()
	{
		return std::make_unique<Prefab_Creator>();
	}

	void init_font()
	{
		auto& io = ImGui::GetIO();
		ImFontConfig config;
		config.MergeMode = true;
		config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
		config.PixelSnapH = true;

		tpath body_f = tpath("Fonts") / body_font;
		tpath heavy_f = tpath("Fonts") / heavy_font;
		tpath open_f = tpath("Fonts") / open_font;


		body_f.replace_extension(".ttf");
		heavy_f.replace_extension(".ttf");

		// 0 para
		static const ImWchar fa_range[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		io.Fonts->AddFontFromFileTTF(body_f.string().c_str(), para_text_size * global_font_scale);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-solid-900.ttf", para_text_size * global_font_scale * global_icon_scale, &config, fa_range);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-regular-400.ttf", para_text_size * global_font_scale * global_icon_scale, &config, fa_range);

		// 1 body
		io.Fonts->AddFontFromFileTTF(body_f.string().c_str(), body_text_size * global_font_scale);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-solid-900.ttf", body_text_size * global_font_scale * global_icon_scale, &config, fa_range);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-regular-400.ttf", body_text_size * global_font_scale * global_icon_scale, &config, fa_range);

		// 2 Header
		io.Fonts->AddFontFromFileTTF(heavy_f.string().c_str(), header_text_size * global_font_scale);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-solid-900.ttf", header_text_size * global_font_scale * global_icon_scale, &config, fa_range);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-regular-400.ttf", header_text_size * global_font_scale * global_icon_scale, &config, fa_range);

		// 3 Sub header
		io.Fonts->AddFontFromFileTTF(heavy_f.string().c_str(), subheader_text_size * global_font_scale);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-solid-900.ttf", subheader_text_size * global_font_scale * global_icon_scale, &config, fa_range);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-regular-400.ttf", subheader_text_size * global_font_scale * global_icon_scale, &config, fa_range);

		// 4 Button
		io.Fonts->AddFontFromFileTTF(heavy_f.string().c_str(), button_text_size * global_font_scale);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-solid-900.ttf", button_text_size * global_font_scale * global_icon_scale, &config, fa_range);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-regular-400.ttf", button_text_size * global_font_scale * global_icon_scale, &config, fa_range);

		// 5 Sub button
		io.Fonts->AddFontFromFileTTF(heavy_f.string().c_str(), subbutton_text_size * global_font_scale);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-solid-900.ttf", subbutton_text_size * global_font_scale * global_icon_scale, &config, fa_range);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-regular-400.ttf", subbutton_text_size * global_font_scale * global_icon_scale, &config, fa_range);

		// 6 Sub button
		io.Fonts->AddFontFromFileTTF(open_f.string().c_str(), open_text_size * global_font_scale);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-solid-900.ttf", open_text_size * global_font_scale * global_icon_scale, &config, fa_range);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-regular-400.ttf", open_text_size * global_font_scale * global_icon_scale, &config, fa_range);


		//// 6 fk 
		//static const ImWchar fk_range[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
		//io.Fonts->AddFontFromFileTTF(heavy_f.string().c_str(), font_text_size);
		//io.Fonts->AddFontFromFileTTF("Fonts/forkawesome-webfont.ttf", font_icon_size, &config, fk_range);

		//// 8 Button
		//static const ImWchar ki_range[] = { ICON_MIN_KI, ICON_MAX_KI, 0 };
		//io.Fonts->AddFontFromFileTTF(def_f.string().c_str(), font_text_size);
		//io.Fonts->AddFontFromFileTTF("Fonts/kenney-icon-font.ttf", font_icon_size, &config, ki_range);

		//// 9 Sub button
		//static const ImWchar ki_range[] = { ICON_MIN_KI, ICON_MAX_KI, 0 };
		//io.Fonts->AddFontFromFileTTF(def_f.string().c_str(), font_text_size);
		//io.Fonts->AddFontFromFileTTF("Fonts/kenney-icon-font.ttf", font_icon_size, &config, ki_range);


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
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.f, 0.f, 0.f, 255.f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.46f, 0.46f, 0.46f, 1.f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
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
		style.ChildRounding = 0;
		style.FrameRounding = 0;
		style.PopupRounding = 5;
		style.ScrollbarRounding = 0;
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

	void init_file_dialog()
	{
		ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* {

			GLuint tex;
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			return UIntToPtr(tex);
		};

		ifd::FileDialog::Instance().DeleteTexture = [](void* tex) {

			GLuint texID = PtrToUint(tex);
			glDeleteTextures(1, &texID);
		};
	}
}
