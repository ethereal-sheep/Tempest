/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "SimulateOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "MainMenuOverlay.h"
#include <Tempest/src/Instance/EditTimeInstance.h>
#include <Editor/src/InstanceManager/InstanceConfig.h>
#include "Util/quitter.h"

namespace Tempest
{
	void MainMenuOverlay::change_state(UI_SHOW state)
	{
		MainMenuUI = state;
		inter.start(-0.1f, 0.02f, .25f, 0, [](float x) { return glm::cubicEaseOut(x); }); // back

		switch (MainMenuUI)
		{
		case Tempest::MainMenuOverlay::UI_SHOW::INITIAL: [[fallthrough]];
		case Tempest::MainMenuOverlay::UI_SHOW::PROJECTS: [[fallthrough]];
		case Tempest::MainMenuOverlay::UI_SHOW::NEW_PROJECT:
			inter_nest[0].start(0.f, 1, .25f, 0.f, 
				[](float x) { return glm::sineEaseOut(x); 
			});
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::CONFLICT_RES:
			for (int i = 0; i < inter_nest.size(); ++i)
			{
				inter_nest[i].start(.8f, .3f, .5f, i * .05f, [](float x) { return glm::cubicEaseOut(x); });
			}
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::SELECT_MAP:
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::LOAD_MAP:
			inter_nest[0].start(0.5f, 0.5f, .5f, 0.f, [](float x) { return glm::backEaseIn(x); });
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::SELECT_CONFLICT_RES:
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::MAIN_PAGE:
			inter_nest[0].start(1.0f, 0.0f, .5f, .1f, [](float x) { return glm::cubicEaseOut(x); });
			inter_nest[1].start(1.0f, 0.0f, .5f, .24f, [](float x) { return glm::exponentialEaseOut(x); });
			inter_nest[2].start(1.0f, 0.0f, .5f, .17f, [](float x) { return glm::quinticEaseOut(x); });
			break;
		default:
			break;
		}

	}

	void MainMenuOverlay::open_popup(const Event& e)
	{
		auto a = event_cast<OpenMainMenuTrigger>(e);
		OverlayOpen = true;
		MapTitle = "";
		SelectedConflictRes = 0;
		SelectedSequences.clear();
		change_state(static_cast<UI_SHOW>(a.menuType));
		window_flags |=  ImGuiWindowFlags_NoTitleBar;
	}

	void MainMenuOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		{
			float dt = ImGui::GetIO().DeltaTime;
			for (auto& i : inter_nest)
				i.update(dt);

			inter.update(dt);
		}

		if (OverlayOpen)
		{
			if (appearing)
			{
				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(0.f, .3f, 0.f);
				appearing = false;
				return;
			}



			instance.window_manager.close_all();

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

			if (ImGui::Begin("MainMenu", nullptr, window_flags))
			{
				// Draw the background
				auto BGimg = tex_map["Assets/StartScreenBG.dds"];
				ImVec2 point = ImGui::GetCursorScreenPos();
				ImVec2 Min{ point.x, point.y };
				ImVec2 Max{ Min.x + viewport->Size.x, Min.y + viewport->Size.y };
				ImGui::GetBackgroundDrawList()->AddImage((void*)static_cast<size_t>(BGimg->GetID()), Min, Max);

				OpenLocalUI(instance, *viewport);
			}
			ImGui::End();

			ImGui::PopStyleVar();
			ImGui::PopStyleColor();

			if (!ImGui::GetHoveredID())
				HoveredID = 0;
			else if (HoveredID != ImGui::GetHoveredID())
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Highlight.wav", "SFX", 0.8f);
				HoveredID = ImGui::GetHoveredID();
			}
		}
	}

	void MainMenuOverlay::OpenLocalUI(Instance& instance, const ImGuiViewport& viewport)
	{
		ImVec2 button_pos{ 0, viewport.Size.y * 0.42f };

		// render the title image
		/*if (MainMenuUI < UI_SHOW::CONFLICT_RES)
		{
			auto title_img = tex_map["Assets/MainMenuTitle.dds"];
			const ImVec2 title_size{ title_img->GetWidth() * 1.0f, title_img->GetHeight() * 1.0f };
			button_pos.x = viewport.Size.x * 0.5f - title_size.x * 0.5f;
			ImGui::SetCursorPos(ImVec2{ button_pos.x, viewport.Size.y * 0.35f - title_size.y * 0.5f });
			ImGui::Image((void*)static_cast<size_t>(title_img->GetID()), title_size);
		}*/

		switch (MainMenuUI)
		{
		case Tempest::MainMenuOverlay::UI_SHOW::INITIAL:
		{
			//ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4{ 0,0,0,0 });
			//ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4{ 0,0,0,0 });
			//// render the selectables
			//std::string selectable = "";
			//ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1, 1, 1, inter_nest[0].get()});
			//ImGui::PushFont(FONT_BTN);
			//selectable = "Projects";
			//ImGui::SetCursorPos(button_pos);
			//ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			//
			//if (ImGui::Selectable(selectable.c_str(), false))
			//{
			//	AudioEngine ae;
			//	ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			//	auto fn = [&]()
			//	{
			//		change_state(UI_SHOW::PROJECTS);
			//	};
			//	inter_nest[0].start(1, 0, .25f, 0.f, [](float x) { return glm::sineEaseOut(x); });
			//	Service<EventManager>::Get().instant_dispatch<DelayTrigger>(.25f, fn);
			//}
			//	

			//if (ImGui::IsItemHovered())
			//{
			//	ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
			//	ImGui::Text(">");
			//}

			//button_pos.y += 40.0f;

			//selectable = "Settings";
			//ImGui::SetCursorPos(button_pos);
			//ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			//
			//if (ImGui::Selectable(selectable.c_str(), false))
			//{
			//	AudioEngine ae;
			//	ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			//	Service<EventManager>::Get().instant_dispatch<SettingsTrigger>();

			//}

			//if (ImGui::IsItemHovered())
			//{
			//	ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
			//	ImGui::Text(">");
			//}

			//button_pos.y += 40.0f;

			//selectable = "Credits";
			//ImGui::SetCursorPos(button_pos);
			//ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			//if (ImGui::Selectable(selectable.c_str(), false))
			//{
			//	AudioEngine ae;
			//	ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			//}

			//if (ImGui::IsItemHovered())
			//{
			//	ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
			//	ImGui::Text(">");
			//}

			//button_pos.y += 40.0f;

			//selectable = "Quit";
			//ImGui::SetCursorPos(button_pos);
			//ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			//if (ImGui::Selectable(selectable.c_str(), false))
			//{
			//	AudioEngine ae;
			//	ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			//	//get_quitter().quit = true;
			//	OverlayOpen = false;
			//}
			//if (ImGui::IsItemHovered())
			//{
			//	ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
			//	ImGui::Text(">");
			//}

			//ImGui::PopFont();
			//ImGui::PopStyleColor(3);
			
			auto title_img = tex_map["Assets/MainMenuTitle.dds"];
			const ImVec2 title_size{ title_img->GetWidth() * 1.0f, title_img->GetHeight() * 1.0f };
			button_pos.x = viewport.Size.x * 0.5f - title_size.x * 0.5f;
			ImGui::SetCursorPos(ImVec2{ button_pos.x, viewport.Size.y * 0.35f - title_size.y * 0.5f });
			ImGui::Image((void*)static_cast<size_t>(title_img->GetID()), title_size);

			if (UI::UIButton_1("START", "START", { viewport.Size.x * 0.5f, viewport.Size.y * 0.6f }, { 50.f, 10.f }, FONT_BTN))
			{
				auto fn = [&]()
				{
					change_state(UI_SHOW::PROJECTS);
				};
				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);
			}

			if (UI::UIButton_1("QUIT", "QUIT", { viewport.Size.x * 0.5f, viewport.Size.y * 0.65f }, { 50.f, 10.f }, FONT_BTN))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
				auto fn = [&]()
				{
					get_quitter().quit = true;
				};
				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);
			}
		}
			
			break;

		case Tempest::MainMenuOverlay::UI_SHOW::PROJECTS:
		{
			//ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4{ 0,0,0,0 });
			//ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4{ 0,0,0,0 });

			//ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1, 1, 1, inter_nest[0].get() });
			//// render the selectables
			//std::string selectable = "";
			//ImGui::PushFont(FONT_BTN);
			//if (dynamic_cast<EditTimeInstance*>(&instance))
			//{
			//	selectable = "Current Project";
			//	ImGui::SetCursorPos(button_pos);
			//	ImGui::SetCursorPosX(ImGui::GetCursorPosX());

			//	if (ImGui::Selectable(selectable.c_str(), false))
			//	{
			//		AudioEngine ae;
			//		ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			//		auto fn = [&]()
			//		{
			//			change_state(UI_SHOW::NEW_PROJECT);
			//		};
			//		inter_nest[0].start(1, 0, .25f, 0.f, [](float x) { return glm::sineEaseOut(x); });
			//		Service<EventManager>::Get().instant_dispatch<DelayTrigger>(.25f, fn);
			//	
			//	}
			//	if (ImGui::IsItemHovered())
			//	{
			//		ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
			//		ImGui::Text(">");
			//	}

			//	button_pos.y += 40.0f;
			//}
			//selectable = "New Project";
			//ImGui::SetCursorPos(button_pos);
			//ImGui::SetCursorPosX(ImGui::GetCursorPosX());

			//if (ImGui::Selectable(selectable.c_str(), false))
			//{
			//	AudioEngine ae;
			//	ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			//	Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Creating new project...");
			//	Service<EventManager>::Get().instant_dispatch<NewProjectTrigger>();
			//	//change_state(UI_SHOW::NEW_PROJECT;
			//	/*if (dynamic_cast<EditTimeInstance*>(&instance))
			//	{
			//		change_state(UI_SHOW::NEW_PROJECT);
			//	}*/
			//}
			//	

			//if (ImGui::IsItemHovered())
			//{
			//	ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
			//	ImGui::Text(">");
			//}

			//button_pos.y += 40.0f;

			//selectable = "Load Project";
			//ImGui::SetCursorPos(button_pos);
			//ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			//if (ImGui::Selectable(selectable.c_str(), false))
			//{
			//	AudioEngine ae;
			//	ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			//	Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Opening...");
			//	Service<EventManager>::Get().instant_dispatch<OpenProjectTrigger>();
			//}
			//if (ImGui::IsItemHovered())
			//{
			//	ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
			//	ImGui::Text(">");
			//}

			//button_pos.y += 40.0f;



			//selectable = "Back";
			//ImGui::SetCursorPos(button_pos);
			//ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			//if (ImGui::Selectable(selectable.c_str(), false))
			//{
			//	AudioEngine ae;
			//	ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			//	auto fn = [&]()
			//	{
			//		change_state(UI_SHOW::INITIAL);
			//	};
			//	inter_nest[0].start(1, 0, .25f, 0.f, [](float x) { return glm::sineEaseOut(x); });
			//	Service<EventManager>::Get().instant_dispatch<DelayTrigger>(.25f, fn);
			//}
			//	
			//if (ImGui::IsItemHovered())
			//{
			//	ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
			//	ImGui::Text(">");
			//}

			//ImGui::PopFont();
			//ImGui::PopStyleColor(3);




			auto tex = tex_map["Assets/BackMenuBtn.dds"];
			ImGui::Dummy({ 0, viewport.Size.y * 0.12f });
			ImGui::Dummy({ viewport.Size.x * 0.07f, 0 });
			ImGui::SameLine();

			//Back button
			if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ (float)tex->GetWidth(),  (float)tex->GetHeight() }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
				auto fn = [&]()
				{
					if (dynamic_cast<EditTimeInstance*>(&instance))
						change_state(UI_SHOW::MAIN_PAGE);
					else
						change_state(UI_SHOW::INITIAL);
				};
				inter_nest[0].start(1, 0, .25f, 0.f, [](float x) { return glm::sineEaseOut(x); });
				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);
			}

			//Project File area
			tex = tex_map["Assets/ProjectBg.dds"];
			auto center = viewport.GetCenter();
			ImVec2 ProjectBGMin = { center.x - tex->GetWidth() * 0.5f ,center.y - tex->GetHeight() * 0.5f };
			ImVec2 ProjectBGMax = { ProjectBGMin.x + tex->GetWidth() ,ProjectBGMin.y + tex->GetHeight() };

			ImGui::GetBackgroundDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), ProjectBGMin, ProjectBGMax);
			




			//New Project Area
			{
				auto img = tex_map["Assets/TemplateBtn.dds"];
				ImVec4 tintPressed = { 0.305f, 0.612f, 0.717f, 1.f };
				ImVec4 tintHover = { 0.443f, 0.690f, 0.775f, 1.f };
				ImGui::PushFont(FONT_BTN);

				string str = "Gurps";
				ImVec2 Pos = { ProjectBGMin.x + 70.f, ProjectBGMax.y * 0.25f };
				ImVec2 strPos;

				for (auto it : fs::directory_iterator("ConflictResolutions"))
				{
					str = it.path().stem().string();
					ImGui::PushID((int)Pos.x);
					ImGui::SetCursorPos(Pos);
					if (UI::UIImageButton((void*)static_cast<size_t>(img->GetID()), ImVec2{ (float)img->GetWidth(), (float)img->GetHeight() }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
					{
						AudioEngine ae;
						ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
						NewProjectName = str;
						auto fn = [&]()
						{
							//create_new_project(get_user_path() / "Projects", NewProjectName);
							change_state(UI_SHOW::NEW_PROJECT);
						};
						Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);
					}
					strPos = { Pos.x + (float)img->GetWidth() * 0.5f - ImGui::CalcTextSize(str.c_str()).x * 0.5f, Pos.y + (float)img->GetHeight() };
					ImGui::GetWindowDrawList()->AddText(strPos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
					ImGui::PopID();

					Pos.x += 150.f;
				}


				//Blank Template
				str = "Blank";
				ImGui::SetCursorPos(Pos);
				if (UI::UIImageButton((void*)static_cast<size_t>(img->GetID()), ImVec2{ (float)img->GetWidth(), (float)img->GetHeight() }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
				{
					AudioEngine ae;
					ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
					NewProjectName = str;
					auto fn = [&]()
					{
						//create_new_project(get_user_path() / "Projects", NewProjectName);
						change_state(UI_SHOW::NEW_PROJECT);
					};
					Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);
				}
				strPos = { Pos.x + (float)img->GetWidth() * 0.5f - ImGui::CalcTextSize(str.c_str()).x * 0.5f, Pos.y + (float)img->GetHeight() };
				ImGui::GetWindowDrawList()->AddText(strPos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

				////D&D Template
				//str = "D&D";
				//Pos.x += 150.f;
				//ImGui::SetCursorPos(Pos);
				//if (UI::UIImageButton((void*)static_cast<size_t>(img->GetID()), ImVec2{ (float)img->GetWidth(), (float)img->GetHeight() }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
				//{
				//	//TODO
				//}
				//strPos = { Pos.x + (float)img->GetWidth() * 0.5f - ImGui::CalcTextSize(str.c_str()).x * 0.5f, Pos.y + (float)img->GetHeight() };
				//ImGui::GetWindowDrawList()->AddText(strPos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

				////Tutorial Template
				//str = "Tutorial";
				//Pos.x += 150.f;
				//ImGui::SetCursorPos(Pos);
				//if (UI::UIImageButton((void*)static_cast<size_t>(img->GetID()), ImVec2{ (float)img->GetWidth(), (float)img->GetHeight() }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
				//{
				//	//TODO
				//}
				//strPos = { Pos.x + (float)img->GetWidth() * 0.5f - ImGui::CalcTextSize(str.c_str()).x * 0.5f, Pos.y + (float)img->GetHeight() };
				//ImGui::GetWindowDrawList()->AddText(strPos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

				ImGui::PopFont();
			}

			//Load Recent Project Area
			ImGui::SetCursorPos({ ProjectBGMin.x + 60.f, ProjectBGMax.y * 0.55f + 20.f});
			ImGui::BeginChild("##LoadRecentProj", { tex->GetWidth() * 0.85f, tex->GetHeight() * 0.35f }, false);

			auto user_path = get_user_path();

			if (user_path != "")
			{
				auto projects_path = user_path / "Projects";
				if (!fs::exists(projects_path))
					fs::create_directories(projects_path);

				int i = 0;
				for (auto it : fs::directory_iterator(projects_path))
				{
					if (!fs::is_directory(it.path()))
						continue;

					fs::path json_path;
					for (auto js : fs::directory_iterator(it.path()))
						if (js.path().extension() == ".json")
							json_path = js.path();
					
					if (json_path.empty())
						continue;

					string str = json_path.stem().string();
					ImGui::PushID(i);

					if (UI::UILoadProject(str.c_str()))
					{
						AudioEngine ae;
						ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
						auto fn = [&, json_path]()
						{
							AudioEngine ae;
							ae.StopAllChannels();
							Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(json_path, MemoryStrategy{}, InstanceType::EDIT_TIME);
						};
						Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.3f, .3f, .0f, fn);
					}
					ImGui::PopID();
					ImGui::Dummy({ 0, 5.f });
					++i;
				}

				ImGui::EndChild();
			}

		}
			break;

		case Tempest::MainMenuOverlay::UI_SHOW::NEW_PROJECT:
		{
			//ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4{ 0,0,0,0 });
			//ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4{ 0,0,0,0 });

			//ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1, 1, 1, inter_nest[0].get() });
			//// render the selectables
			//std::string selectable = "";
			//ImGui::PushFont(FONT_BTN);
			//selectable = "Conflict Resolutions";
			//ImGui::SetCursorPos(button_pos);
			//ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			//if (ImGui::Selectable(selectable.c_str(), false))
			//{
			//	AudioEngine ae;
			//	ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);

			//	auto fn = [&]()
			//	{
			//		change_state(UI_SHOW::CONFLICT_RES);
			//	};
			//	// fade in, fade out, visible
			//	Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);


			//}
			//	

			//if (ImGui::IsItemHovered())
			//{
			//	ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
			//	ImGui::Text(">");
			//}

			//button_pos.y += 40.0f;

			//selectable = "New Game";
			//ImGui::SetCursorPos(button_pos);
			//ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			//if (ImGui::Selectable(selectable.c_str(), false))
			//{
			//	AudioEngine ae;
			//	ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);

			//	if (instance.get_scene_paths().empty())
			//	{
			//		Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Map found!");
			//	}
			//	else
			//	{
			//		// check every con res
			//		bool empty = true;
			//		for (auto [b, load_path] : instance.get_conflict_resolution_paths()) {
			//			if(b) empty = false;
			//		}

			//		if (empty)
			//		{
			//			Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Unit or Sequence found!"); // no conres
			//		}
			//		else
			//		{
			//			auto check_unit_and_seq = [](tpath& load_path, std::vector<std::pair<Entity, string>>& sequences) {
			//				
			//				ECS ecs;
			//				string res_name = load_path.stem().string();
			//				auto path = load_path.parent_path();
			//				int a = std::atoi(path.stem().string().c_str());
			//				if (a >= 1 && a <= 3)
			//				{
			//					ecs.load(path, res_name);

			//					if(ecs.view_first<tc::Character>() && ecs.view_first<tc::ConflictGraph>())
			//					{
			//						for (auto id : ecs.view<tc::ConflictGraph>())
			//						{
			//							const auto& name = ecs.get<tc::Graph>(id).g.get_name();
			//							sequences.push_back(std::make_pair(id, name));
			//						}
			//						return true;
			//					}
			//				}

			//				return false;
			//			};

			//			bool allowed = false;
			//			int i = 0;
			//			for (auto [b, load_path] : instance.get_conflict_resolution_paths())
			//			{
			//				ConResSequences[i].clear();
			//				if (check_unit_and_seq(load_path, ConResSequences[i]))
			//				{
			//					OkayConRes[i] = true;
			//					allowed = true;
			//				}
			//				++i;
			//			}

			//			if (allowed)
			//			{
			//				// go ahead
			//				auto fn = [&, selectable]()
			//				{
			//					MapTitle = selectable;
			//					change_state(UI_SHOW::LOAD_MAP);
			//				};
			//				// fade in, fade out, visible
			//				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);

			//			}
			//			else
			//			{
			//				Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Unit or Sequence found!");
			//			}
			//		}


			//	}
			//	/*else if(!instance.ecs.view_first<tc::Character>() && !instance.ecs.view_first<tc::ConflictGraph>())
			//	{
			//		Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Unit or Sequence found!");
			//	}
			//	else if (!instance.ecs.view_first<tc::ConflictGraph>())
			//	{
			//		Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Sequence found!");
			//	}
			//	else
			//	{
			//		Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Unit found!");
			//	}*/

			//}
			//if (ImGui::IsItemHovered())
			//{
			//	ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
			//	ImGui::Text(">");
			//}

			//button_pos.y += 40.0f;
			//	

			//selectable = "Map Builder";
			//ImGui::SetCursorPos(button_pos);
			//ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			//if (ImGui::Selectable(selectable.c_str(), false))
			//{

			//	AudioEngine ae;
			//	ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			//	// go ahead
			//	auto fn = [&, selectable]()
			//	{
			//		MapTitle = selectable;
			//		change_state(UI_SHOW::SELECT_MAP);
			//	};
			//	// fade in, fade out, visible
			//	Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);

			//}
			//if (ImGui::IsItemHovered())
			//{
			//	ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
			//	ImGui::Text(">");
			//}

			//button_pos.y += 40.0f;

			//selectable = "Back";
			//ImGui::SetCursorPos(button_pos);
			//ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			//if (ImGui::Selectable(selectable.c_str(), false))
			//{
			//	AudioEngine ae;
			//	ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			//	auto fn = [&]()
			//	{
			//		change_state(UI_SHOW::PROJECTS);
			//	};
			//	inter_nest[0].start(1, 0, .25f, 0.f, [](float x) { return glm::sineEaseOut(x); });
			//	Service<EventManager>::Get().instant_dispatch<DelayTrigger>(.25f, fn);
			//}
			//	
			//if (ImGui::IsItemHovered())
			//{
			//	ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
			//	ImGui::Text(">");
			//}

			//ImGui::PopFont();
			//ImGui::PopStyleColor(3);

			auto tex = tex_map["Assets/BackMenuBtn.dds"];
			ImGui::Dummy({ 0, viewport.Size.y * 0.12f });
			ImGui::Dummy({ viewport.Size.x * 0.07f, 0 });
			ImGui::SameLine();

			//Back button
			if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ (float)tex->GetWidth(),  (float)tex->GetHeight()}, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
				auto fn = [&]()
				{
					change_state(UI_SHOW::PROJECTS);
				};
				inter_nest[0].start(1, 0, .25f, 0.f, [](float x) { return glm::sineEaseOut(x); });
				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, 0.f, fn);
			}

			//MapBG
			tex = tex_map["Assets/PrisonMapBG.dds"];
			auto center = viewport.GetCenter();
			ImVec2 ProjectBGMin = { center.x - tex->GetWidth() * 0.5f ,center.y - tex->GetHeight() * 0.5f };
			ImVec2 ProjectBGMax = { ProjectBGMin.x + tex->GetWidth() ,ProjectBGMin.y + tex->GetHeight() };

			

			//Select Map Area
			ImGui::SetCursorPos({ ProjectBGMin.x + 60.f, ProjectBGMax.y * 0.2f });
			ImGui::BeginChild("##LoadRecentProj", { tex->GetWidth() * 0.5f, tex->GetHeight() * 0.7f }, false);
			
			auto img = tex_map["Assets/TemplateBtn.dds"];
			
			int i = 0;
			for (auto it : fs::directory_iterator("Scenes"))
			{
				string str = it.path().stem().string();
				ImGui::PushID(i);

				auto display = str;
				if (SelectedMaps.count(str))
					display += "*";

				if (UI::UIImgBtnWithText((void*)static_cast<size_t>(img->GetID()), ImVec2{ (float)img->GetWidth(), (float)img->GetHeight() }, display))
				{
					if (SelectedMaps.count(str))
						SelectedMaps.erase(str);
					else
						SelectedMaps.insert(str);
				}
				ImGui::PopID();

				if ((i + 1) % 4 != 0)
				{
					ImGui::SameLine();
					ImGui::Dummy({ 5.f, 0.f });
					ImGui::SameLine();
				}
				else
				{
					ImGui::Dummy({ 0.f, 20.f });
				}

				++i;
			}

			ImGui::EndChild();

			auto nextBtn = tex_map["Assets/whiteNextBtn.dds"];
			ImGui::Dummy({ 0, tex->GetHeight() * 0.15f });
			ImGui::Dummy({ viewport.Size.x * 0.7f,0.f  });
			ImGui::SameLine();
			if (UI::UIImageButton((void*)static_cast<size_t>(nextBtn->GetID()), ImVec2{ (float)nextBtn->GetWidth(),  (float)nextBtn->GetHeight() }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
			{
				auto create_new_project = [](fs::path path, string name)
				{
					if (!std::filesystem::exists(path))
						throw std::invalid_argument("Filepath does not exist!");

					if (!std::filesystem::create_directory(path / name))
						if (!std::filesystem::is_empty(path / name))
							throw std::invalid_argument(name + " project folder already exists!");

					std::ofstream out(path / name / (name + ".json"));
					if (!out.good())
						throw std::invalid_argument(name + " project files could not be created!");
					out << "";
					out.close();

					return path / name;
				};

				try
				{
					std::unordered_set<string> project_list;

					for (auto it : fs::directory_iterator(get_user_path() / "Projects"))
					{
						project_list.insert(it.path().stem().string());
					}

					auto actual = algo::get_next_name(NewProjectName, project_list);
					auto project_path = create_new_project(get_user_path() / "Projects", actual);
					auto json_path = project_path / (actual + ".json");

					if (NewProjectName != "Blank")
					{
						fs::path cr_paths{ "ConflictResolutions" };
						if (fs::exists(cr_paths / NewProjectName))
						{
							fs::create_directories(project_path / "conflict_resolutions/1" / NewProjectName);
							fs::copy(cr_paths / NewProjectName, project_path / "conflict_resolutions/1" / NewProjectName, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
						}
					}


					for (auto& str : SelectedMaps)
					{
						fs::path scene_paths{ "Scenes" };
						if (fs::exists(scene_paths / str))
						{
							fs::create_directories(project_path / "scenes" / str);
							fs::copy(scene_paths / str, project_path / "scenes" / str, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
						}
					}

					auto fn = [&, json_path]()
					{
						AudioEngine ae;
						ae.StopAllChannels();
						Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(json_path, MemoryStrategy{}, InstanceType::EDIT_TIME);
					};

					Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, 0.f, fn);

				}
				catch (std::exception a)
				{
					change_state(UI_SHOW::PROJECTS);
					Service<EventManager>::Get().instant_dispatch<ErrorTrigger>(a.what());

				}
			}

			ImGui::GetBackgroundDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), ProjectBGMin, ProjectBGMax);
		}
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::MAIN_PAGE:
		{
			float y_pos = 800.f;

			//Conflict Resolution Button
			auto tex = tex_map["Assets/ConflictResolutionButton.dds"];
			ImGui::SetCursorPos({viewport.Size.x * 0.15f, viewport.Size.y * 0.25f - inter_nest[0].get() * y_pos });
			if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ (float)tex->GetWidth(),  (float)tex->GetHeight() }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
			{

				auto fn = [&]()
				{
					change_state(UI_SHOW::CONFLICT_RES);
				};
				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);
			}

			//Start Game Button
			tex = tex_map["Assets/StartGameButton.dds"];
			ImGui::SetCursorPos({ viewport.Size.x * 0.37f, viewport.Size.y * 0.2f - inter_nest[1].get() * y_pos });
			if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ (float)tex->GetWidth(),  (float)tex->GetHeight() }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
			{
				string selectable = "New Game";
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);

				if (instance.get_scene_paths().empty())
				{
					Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Map found!");
				}
				else
				{
					// check every con res
					bool empty = true;
					for (auto [b, load_path] : instance.get_conflict_resolution_paths()) {
						if(b) empty = false;
					}

					if (empty)
					{
						Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Unit or Sequence found!"); // no conres
					}
					else
					{
						auto check_unit_and_seq = [](tpath& load_path, std::vector<std::pair<Entity, string>>& sequences) {
								
							ECS ecs;
							string res_name = load_path.stem().string();
							auto path = load_path.parent_path();
							int a = std::atoi(path.stem().string().c_str());
							if (a >= 1 && a <= 3)
							{
								ecs.load(path, res_name);

								if(ecs.view_first<tc::Character>() && ecs.view_first<tc::ConflictGraph>())
								{
									for (auto id : ecs.view<tc::ConflictGraph>())
									{
										const auto& name = ecs.get<tc::Graph>(id).g.get_name();
										sequences.push_back(std::make_pair(id, name));
									}
									return true;
								}
							}

							return false;
						};

						bool allowed = false;
						int i = 0;
						for (auto [b, load_path] : instance.get_conflict_resolution_paths())
						{
							ConResSequences[i].clear();
							if (check_unit_and_seq(load_path, ConResSequences[i]))
							{
								OkayConRes[i] = true;
								allowed = true;
							}
							++i;
						}

						if (allowed)
						{
							// go ahead
							auto fn = [&, selectable]()
							{
								MapTitle = selectable;
								change_state(UI_SHOW::LOAD_MAP);
							};
							// fade in, fade out, visible
							Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);

						}
						else
						{
							Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Unit or Sequence found!");
						}
					}


				}
				/*else if(!instance.ecs.view_first<tc::Character>() && !instance.ecs.view_first<tc::ConflictGraph>())
				{
					Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Unit or Sequence found!");
				}
				else if (!instance.ecs.view_first<tc::ConflictGraph>())
				{
					Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Sequence found!");
				}
				else
				{
					Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Unit found!");
				}*/

			}

			//Map Builder Button
			tex = tex_map["Assets/MapBuilderButton.dds"];
			ImGui::SetCursorPos({ viewport.Size.x * 0.6f, viewport.Size.y * 0.3f - inter_nest[2].get() * y_pos });
			if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ (float)tex->GetWidth(),  (float)tex->GetHeight() }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
			{
				auto fn = [&]()
				{
					MapTitle = "Map Builder";
					change_state(UI_SHOW::SELECT_MAP);
				};
				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);
			}

			//Project Icon Btn
			tex = tex_map["Assets/ProjectIconBtn.dds"];
			ImGui::SetCursorPos({ viewport.Size.x * 0.05f, viewport.Size.y * 0.75f });
			if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ (float)tex->GetWidth(),  (float)tex->GetHeight() }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
			{
				auto fn = [&]()
				{
					change_state(UI_SHOW::PROJECTS);
				};
				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);
			}

			//Setting Icon Btn
			tex = tex_map["Assets/SettingIconBtn.dds"];
			ImGui::SetCursorPos({ viewport.Size.x * 0.13f, viewport.Size.y * 0.8f });
			if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ (float)tex->GetWidth(),  (float)tex->GetHeight() }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
				Service<EventManager>::Get().instant_dispatch<SettingsTrigger>();
			}

			//QuitIconBtn
			tex = tex_map["Assets/QuitIconBtn.dds"];
			ImGui::SetCursorPos({ viewport.Size.x * 0.21f, viewport.Size.y * 0.8f });
			if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ (float)tex->GetWidth(),  (float)tex->GetHeight() }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
			{

				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
				auto fn = [&]()
				{
					get_quitter().quit = true;
				};
				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);
			}

		}
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::CONFLICT_RES:
		{
			auto image = tex_map["Assets/ConflictBG.dds"];
			ImVec2 point = ImGui::GetCursorScreenPos();
			ImVec2 Min{ point.x, point.y - 1.f };
			ImVec2 Max{ Min.x + viewport.Size.x, Min.y + viewport.Size.y };
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(image->GetID()), Min, Max);

			// render title
			ImGui::SetCursorPos(ImVec2{ 0,0 });
			ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
			UI::SubHeader("Conflict Resolution");
			ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });

			auto cur_pos = ImGui::GetCursorPos();
			
			int i = 1;
			for (auto& [b, path] : instance.get_conflict_resolution_paths())
			{
				ImGui::SetCursorPos(cur_pos);
				ImGui::Dummy({ viewport.Size.x * inter_nest[i-1].get(), 0.f });
				ImGui::SameLine();
				string str = path.stem().string() + " " + std::to_string(i);

				auto [selected, deleted] = UI::UIConflictSelectable(str.c_str(), false, b ? i : 0);

				if (selected)
				{
					AudioEngine ae;
					ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
					if (b) // if b is true, there is a conres at this position
					{

						inter_nest[i-1].start(.3f, .5f, .5f, 0.f, [](float x) { return glm::backEaseIn(x); });

						auto fn = [&, path = path, str]()
						{
							OverlayOpen = false;
							instance.tutorial_temp_enable = true;
							Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>(instance);
							Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Opening " + str);
							instance.load_new_conflict_resolution_by_path(path);
						};

						Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.5f, .2f, 0.f, fn);


					}
					else // there is none, create
					{
						if (auto edit = dynamic_cast<EditTimeInstance*>(&instance))
						{
							edit->create_new_conflict_resolution(i, "Conflict Resolution");
						}
					}
				}
				else if (deleted)
				{
					ImGui::OpenPopup(string("DeleteCR##" + std::to_string(i)).c_str());
				}

				if (UI::ConfirmDeletePopup(string("DeleteCR##" + std::to_string(i)).c_str(), "Delete this conflict resolution?"))
				{
					if (auto edit = dynamic_cast<EditTimeInstance*>(&instance))
					{
						edit->delete_conflict_resolution(i);
					}
				}

				ImGui::Dummy({ 0.f, viewport.Size.y * 0.07f });
				cur_pos = ImGui::GetCursorPos();
				++i;
			}

			/*for (int i = 1; i < 4; i++)
			{
				ImGui::SetCursorPos(cur_pos);
				ImGui::Dummy({ viewport.Size.x * 0.3f, 0.f });
				ImGui::SameLine();
				string str = "CONFLICT RES##" + std::to_string(i);
				int jank = 1;
				if (i > jank)
					jank = 0;
				if (UI::UIConflictSelectable(str.c_str(), false, jank).first)
				{
					AudioEngine ae;
					ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
					if (jank)
					{
						OverlayOpen = false;
						Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>(instance);
					}
				}
				ImGui::Dummy({ 0.f, viewport.Size.y * 0.07f });
				cur_pos = ImGui::GetCursorPos();
			}*/

			// render back button
			ImGui::SetCursorPos(ImVec2{ viewport.Size.x * inter.get(),viewport.Size.y * 0.03f });
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
			image = tex_map["Assets/BackMenuBtn.dds"];

			if (UI::UIImageButton((void*)static_cast<size_t>(image->GetID()), ImVec2{ image->GetWidth() * 0.7f, image->GetHeight() * 0.7f }))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);

				auto fn = [&]()
				{
					change_state(UI_SHOW::MAIN_PAGE);
				};

				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);
			}
				

			ImGui::PopStyleColor(3);

		}
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::SELECT_MAP:
		{
			// render the select map image
			auto image = tex_map["Assets/MM_SelectMap.dds"];
			ImVec2 point = ImGui::GetCursorScreenPos();
			ImVec2 Min{ point.x, point.y };
			ImVec2 Max{ Min.x + viewport.Size.x, Min.y + viewport.Size.y };
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(image->GetID()), Min, Max);

			// render title
			ImGui::SetCursorPos(ImVec2{ 0,0 });
			ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
			UI::SubHeader("Map Builder");
			ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });


			// render back button
			ImGui::SetCursorPos(ImVec2{ viewport.Size.x * inter.get(),viewport.Size.y * 0.03f });
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
			image = tex_map["Assets/BackMenuBtn.dds"];

			if (UI::UIImageButton((void*)static_cast<size_t>(image->GetID()), ImVec2{ image->GetWidth() * 0.7f, image->GetHeight() * 0.7f }))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
				auto fn = [&]()
				{
					change_state(UI_SHOW::MAIN_PAGE);
				};
				// fade in, fade out, visible
				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);
			}
				

			ImGui::PopStyleColor(3);

			ImGui::PushFont(FONT_BTN);
			// render bottom two buttons
			if (UI::UIButton_2("New Map", "New Map", ImVec2{ viewport.Size.x * 0.34f, viewport.Size.y * 0.85f }, { 0,0 }, FONT_BTN))
			{
				ImGui::OpenPopup("NEW MAP");
				NewMapName = "Map";
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			}

			if (UI::UIButton_2("Load Map", "Load Map", ImVec2{ viewport.Size.x * 0.66f, viewport.Size.y * 0.85f }, { 0,0 }, FONT_BTN))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
				auto fn = [&]()
				{
					change_state(UI_SHOW::LOAD_MAP);
				};
				// fade in, fade out, visible
				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);

			}
			
			ImGui::PopFont();

			if (UI::ConfirmInputNamePopup("NEW MAP", NewMapName))
			{
				auto fn = [&]()
				{
					if (auto edit = dynamic_cast<EditTimeInstance*>(&instance))
					{
						auto name = edit->create_new_scene(NewMapName);
						if (name != "" && edit->load_new_scene_by_name(name))
						{
							AudioEngine ae;
							ae.StopAllChannels();
							Service<EventManager>::Get().instant_dispatch<OpenBuildModeOverlay>();
							OverlayOpen = false;
						}
					}
				};
				// fade in, fade out, visible
				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);
			}
		}
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::LOAD_MAP:
		{
			// render the select map image
			auto image = tex_map["Assets/MM_LoadMap.dds"];
			ImVec2 point = ImGui::GetCursorScreenPos();
			ImVec2 Min{ point.x, point.y };
			ImVec2 Max{ Min.x + viewport.Size.x, Min.y + viewport.Size.y };
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(image->GetID()), Min, Max);

			// render title
			ImGui::SetCursorPos(ImVec2{ 0,0 });
			ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
			UI::SubHeader("Load Map");
			ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });


			// render back button
			ImGui::SetCursorPos(ImVec2{ viewport.Size.x * inter.get(), viewport.Size.y * 0.03f });
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
			image = tex_map["Assets/BackMenuBtn.dds"];



			if (UI::UIImageButton((void*)static_cast<size_t>(image->GetID()), ImVec2{ image->GetWidth() * 0.7f, image->GetHeight() * 0.7f }))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);

				if (MapTitle == "Map Builder")
				{
					auto fn = [&]()
					{
						change_state(UI_SHOW::MAIN_PAGE);
					};
					// fade in, fade out, visible
					Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);
				}
				else
				{
					auto fn = [&]()
					{
						change_state(UI_SHOW::MAIN_PAGE);
					};
					// fade in, fade out, visible
					Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);
				}

			}
				

			ImGui::PopStyleColor(3);

			// draw the child
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 10.0f, 10.f });
			const ImVec2 child_size{viewport.Size.x * 0.5f, viewport.Size.y * 0.55f};
			ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.65f - child_size.x * 0.5f, viewport.Size.y * 0.5f - child_size.y * 0.5f });

			ImGui::PushStyleColor(ImGuiCol_Border, { 0,0,0,0 });
			for (auto& [b, path] : instance.get_scene_paths())
			{
				auto scene_name = path.stem().string();
				ImGui::SetCursorPosX(viewport.Size.x * 0.65f - child_size.x * (SelectedMap == scene_name ? inter_nest[0].get() : 0.5f));

				const std::pair<bool, bool> map_pair = UI::UIMapSelectable(scene_name.c_str(), "Date created: WIP", false, MapTitle == "Map Builder", 1);

				// render all the maps here
				if (map_pair.first)
				{
					AudioEngine ae;
					ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);

					if (MapTitle == "Map Builder")
					{
						SelectedMap = scene_name;
						inter_nest[0].start(.5f, -.5f, .5f, 0.f, [](float x) { return glm::backEaseIn(x); });
						auto fn = [&, p = path]()
						{
							AudioEngine ae;
							ae.StopAllChannels();
							Service<EventManager>::Get().instant_dispatch<OpenBuildModeOverlay>();
							instance.load_new_scene_by_path(p);
							OverlayOpen = false;
						};
						// fade in, fade out, visible
						Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.5f, .15f, .0f, fn);
					}
					else
					{
						SelectedMap = scene_name;
						MainMenuUI = UI_SHOW::SELECT_CONFLICT_RES;
					}
				}

				if (map_pair.second)
				{
					ImGui::OpenPopup(string("DeleteMap##" + scene_name).c_str());
				}

				if (UI::ConfirmDeletePopup(string("DeleteMap##" + scene_name).c_str(), "Delete this map?"))
				{
					if (auto edit = dynamic_cast<EditTimeInstance*>(&instance))
					{
						edit->delete_scene(scene_name);
					}
				}
			}


			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::SELECT_CONFLICT_RES:
		{
			// render the select map image
			auto image = tex_map["Assets/MM_SelectCR.dds"];
			ImVec2 point = ImGui::GetCursorScreenPos();
			ImVec2 Min{ point.x, point.y };
			ImVec2 Max{ Min.x + viewport.Size.x, Min.y + viewport.Size.y };
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(image->GetID()), Min, Max);

			// render title
			ImGui::SetCursorPos(ImVec2{ 0,0 });
			ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
			UI::SubHeader("Select Conflict Resolution");
			ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });


			// render back button
			ImGui::SetCursorPos(ImVec2{ viewport.Size.x * inter.get(),viewport.Size.y * 0.03f });
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
			image = tex_map["Assets/BackMenuBtn.dds"];

			if (ImGui::ImageButton((void*)static_cast<size_t>(image->GetID()), ImVec2{ image->GetWidth() * 0.7f, image->GetHeight() * 0.7f }))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
				MainMenuUI = UI_SHOW::LOAD_MAP;

				//auto fn = [&]()
				//{
				//};
				//// fade in, fade out, visible
				//Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);


			}
				

			ImGui::PopStyleColor(3);

			// draw the child
			const ImVec2 child_size{ viewport.Size.x * 0.25f, viewport.Size.y * 0.55f };
			ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.5f - child_size.x * 0.5f, viewport.Size.y * 0.55f - child_size.y * 0.5f });

			ImGui::PushStyleColor(ImGuiCol_Border, { 0,0,0,0 });
			if (ImGui::BeginChild("##LoadConflictResMainMenu", child_size, true))
			{
				const ImVec2 cusor{ ImGui::GetCursorPosX() + 200.0f, ImGui::GetCursorPosY() + 40.0f };
				// TODO: load the conflict stuff here
				int i = 0, u = 0; // i for positioning, u for index
				for (auto& [b, path] : instance.get_conflict_resolution_paths())
				{

					string name = path.stem().string();

					ImGui::PushID(std::string{ name + std::to_string(u) }.c_str());

					if (OkayConRes[u])
					{
						if (UI::UIButton_2(
							std::string{ name + " " + std::to_string(u+1) }.c_str(),
							std::string{ name + " " + std::to_string(u+1) }.c_str(),
							ImVec2{ cusor.x, cusor.y + i * 90.0f }, { 50,20 },
							FONT_BTN, SelectedConflictRes == u))
						{
							SelectedConflictRes = u;
						}
						++i;
					}
					u++;
					ImGui::PopID();
				}
			}

			ImGui::EndChild();

			ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.8f - child_size.x * 0.5f, viewport.Size.y * 0.55f - child_size.y * 0.5f });
			if (ImGui::BeginChild("##LoadSequenceMainMenu", child_size, true) && SelectedConflictRes >= 0 && SelectedConflictRes < 3 && OkayConRes[SelectedConflictRes])
			{
				const ImVec2 cusor{ ImGui::GetCursorPosX() + 200.0f, ImGui::GetCursorPosY() + 40.0f };

				// TODO: render all the sequences from selected conflict
				// TODO: make a popup menu

				int i = 0;
				//bool selected = SelectedSequences.size();

				for (auto& [id, name] : ConResSequences[SelectedConflictRes])
				{
					ImGui::PushID(name.c_str());
					if (UI::UIButton_2(name.c_str(), name.c_str(), ImVec2{ cusor.x, cusor.y + i * 90.0f }, { 50, 20 }, FONT_BTN, SelectedSequences.empty() ? false : SelectedSequences[0] == id))
					{
						SelectedSequences.clear();
						SelectedSequences.push_back(id);
						//Service<EventManager>::Get().instant_dispatch<MainMenuSequencePopupTrigger>(SelectedSequences);
					}
					i++;
					ImGui::PopID();
				}



				++i;
			}

			ImGui::EndChild();

			ImGui::PopStyleColor();

			if (UI::UIButton_2("Next", "Next", ImVec2{viewport.Size.x * 0.9f, viewport.Size.y * 0.95f }, { -20,20 }, FONT_BTN))
			{
				AudioEngine ae;
				ae.StopAllChannels();
				ae.Play("Sounds2D/BGM_1.wav", "BGM", 0.3f, true);
				OverlayOpen = false;
				dynamic_cast<EditTimeInstance&>(instance).save();
				Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(
					dynamic_cast<EditTimeInstance&>(instance).get_full_path(),
					MemoryStrategy{},
					InstanceType::RUN_TIME,
					SelectedMap,
					SelectedConflictRes + 1,
					SelectedSequences);
			}

		}
			break;
		default:
			break;
		}
	}
}