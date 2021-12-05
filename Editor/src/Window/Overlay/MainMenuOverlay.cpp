/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "SimulateOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"
#include "MainMenuOverlay.h"

namespace Tempest
{
	void MainMenuOverlay::open_popup(const Event&)
	{
		OverlayOpen = true;
		MapTitle = "";
		SelectedConflictRes = 0;
		SelectedSequences.clear();
		MainMenuUI = UI_SHOW::INITIAL;
		window_flags =  ImGuiWindowFlags_NoTitleBar;
	}

	void MainMenuOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		if (OverlayOpen)
		{
			instance.window_manager.close_all();

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

			if (ImGui::Begin("MainMenu", nullptr, window_flags))
			{
				// Draw the background
				auto BGimg = tex_map["Assets/StartScreenBG.png"];
				ImVec2 point = ImGui::GetCursorScreenPos();
				ImVec2 Min{ point.x, point.y };
				ImVec2 Max{ Min.x + viewport->Size.x, Min.y + viewport->Size.y };
				ImGui::GetBackgroundDrawList()->AddImage((void*)static_cast<size_t>(BGimg->GetID()), Min, Max);

				OpenLocalUI(instance, *viewport);
			}
			ImGui::End();

			ImGui::PopStyleVar();
			ImGui::PopStyleColor();

			// this is a test run
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
			{
				Service<EventManager>::Get().instant_dispatch<PauseOverlayTrigger>();
			}
		}
	}

	void MainMenuOverlay::OpenLocalUI(Instance& instance, const ImGuiViewport& viewport)
	{
		ImVec2 button_pos{ 0, viewport.Size.y * 0.55f };

		// render the title image
		if (MainMenuUI < UI_SHOW::CONFLICT_RES)
		{
			auto title_img = tex_map["Assets/MainMenuTitle.png"];
			const ImVec2 title_size{ title_img->GetWidth() * 1.0f, title_img->GetHeight() * 1.0f };
			button_pos.x = viewport.Size.x * 0.2f - title_size.x * 0.5f;
			ImGui::SetCursorPos(ImVec2{ button_pos.x, viewport.Size.y * 0.45f - title_size.y * 0.5f });
			ImGui::Image((void*)static_cast<size_t>(title_img->GetID()), title_size);
		}

		switch (MainMenuUI)
		{
		case Tempest::MainMenuOverlay::UI_SHOW::INITIAL:
		{
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4{ 0,0,0,0 });
			// render the selectables
			std::string selectable = "";
			ImGui::PushFont(FONT_BTN);
			selectable = "Projects";
			ImGui::SetCursorPos(button_pos);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			
			if (ImGui::Selectable(selectable.c_str(), false))
				MainMenuUI = UI_SHOW::PROJECTS;

			if (ImGui::IsItemHovered())
			{
				ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
				ImGui::Text(">");
			}

			button_pos.y += 40.0f;

			selectable = "Settings";
			ImGui::SetCursorPos(button_pos);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			
			ImGui::Selectable(selectable.c_str(), false);
			if (ImGui::IsItemHovered())
			{
				ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
				ImGui::Text(">");
			}

			button_pos.y += 40.0f;

			selectable = "Credits";
			ImGui::SetCursorPos(button_pos);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			ImGui::Selectable(selectable.c_str(), false);
			if (ImGui::IsItemHovered())
			{
				ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
				ImGui::Text(">");
			}

			button_pos.y += 40.0f;

			selectable = "Quit";
			ImGui::SetCursorPos(button_pos);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			if (ImGui::Selectable(selectable.c_str(), false))
			{
				OverlayOpen = false;
				instance.window_manager.show_all();
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
				ImGui::Text(">");
			}

			ImGui::PopFont();
			ImGui::PopStyleColor(2);
		}
			
			break;

		case Tempest::MainMenuOverlay::UI_SHOW::PROJECTS:
		{
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4{ 0,0,0,0 });

			// render the selectables
			std::string selectable = "";
			ImGui::PushFont(FONT_BTN);
			selectable = "New Project";
			ImGui::SetCursorPos(button_pos);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX());

			if (ImGui::Selectable(selectable.c_str(), false))
				MainMenuUI = UI_SHOW::NEW_PROJECT;

			if (ImGui::IsItemHovered())
			{
				ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
				ImGui::Text(">");
			}

			button_pos.y += 40.0f;

			selectable = "Load Project";
			ImGui::SetCursorPos(button_pos);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			ImGui::Selectable(selectable.c_str(), false);
			if (ImGui::IsItemHovered())
			{
				ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
				ImGui::Text(">");
			}

			button_pos.y += 40.0f;

			selectable = "Back";
			ImGui::SetCursorPos(button_pos);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			if (ImGui::Selectable(selectable.c_str(), false))
				MainMenuUI = UI_SHOW::INITIAL;
			if (ImGui::IsItemHovered())
			{
				ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
				ImGui::Text(">");
			}

			ImGui::PopFont();
			ImGui::PopStyleColor(2);
		}
			break;

		case Tempest::MainMenuOverlay::UI_SHOW::NEW_PROJECT:
		{
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4{ 0,0,0,0 });

			// render the selectables
			std::string selectable = "";
			ImGui::PushFont(FONT_BTN);
			selectable = "Conflict Resolutions";
			ImGui::SetCursorPos(button_pos);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			if (ImGui::Selectable(selectable.c_str(), false))
				MainMenuUI = UI_SHOW::CONFLICT_RES;

			if (ImGui::IsItemHovered())
			{
				ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
				ImGui::Text(">");
			}

			button_pos.y += 40.0f;

			selectable = "New Game";
			ImGui::SetCursorPos(button_pos);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			if (ImGui::Selectable(selectable.c_str(), false))
			{
				MapTitle = selectable;
				MainMenuUI = UI_SHOW::SELECT_MAP;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
				ImGui::Text(">");
			}

			button_pos.y += 40.0f;
				

			selectable = "Map Builder";
			ImGui::SetCursorPos(button_pos);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			if (ImGui::Selectable(selectable.c_str(), false))
			{
				MapTitle = selectable;
				MainMenuUI = UI_SHOW::SELECT_MAP;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
				ImGui::Text(">");
			}

			button_pos.y += 40.0f;

			selectable = "Back";
			ImGui::SetCursorPos(button_pos);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			if (ImGui::Selectable(selectable.c_str(), false))
				MainMenuUI = UI_SHOW::PROJECTS;
			if (ImGui::IsItemHovered())
			{
				ImGui::SetCursorPos(ImVec2{ button_pos.x - 20.0f, button_pos.y });
				ImGui::Text(">");
			}

			ImGui::PopFont();
			ImGui::PopStyleColor(2);
		}
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::CONFLICT_RES:
		{
			auto image = tex_map["Assets/ConflictBG.png"];
			ImVec2 point = ImGui::GetCursorScreenPos();
			ImVec2 Min{ point.x, point.y };
			ImVec2 Max{ Min.x + viewport.Size.x, Min.y + viewport.Size.y };
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(image->GetID()), Min, Max);

			// render title
			ImGui::SetCursorPos(ImVec2{ 0,0 });
			ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
			UI::SubHeader("Conflict Resolution");
			ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });

			auto cur_pos = ImGui::GetCursorPos();
			for (int i = 0; i < 3; i++)
			{
				ImGui::SetCursorPos(cur_pos);
				ImGui::Dummy({ viewport.Size.x * 0.3f, 0.f });
				ImGui::SameLine();
				string str = "CONFLICT RES##" + std::to_string(i);
				UI::UIConflictSelectable(str.c_str(), false, 1);
				ImGui::Dummy({ 0.f, viewport.Size.y * 0.07f });
				cur_pos = ImGui::GetCursorPos();
			}

			// render back button
			ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.02f,viewport.Size.y * 0.03f });
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
			image = tex_map["Assets/BackMenuBtn.png"];

			if (ImGui::ImageButton((void*)static_cast<size_t>(image->GetID()), ImVec2{ image->GetWidth() * 0.7f, image->GetHeight() * 0.7f }))
				MainMenuUI = UI_SHOW::NEW_PROJECT;

			ImGui::PopStyleColor(3);

		}
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::SELECT_MAP:
		{
			// render the select map image
			auto image = tex_map["Assets/MM_SelectMap.png"];
			ImVec2 point = ImGui::GetCursorScreenPos();
			ImVec2 Min{ point.x, point.y };
			ImVec2 Max{ Min.x + viewport.Size.x, Min.y + viewport.Size.y };
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(image->GetID()), Min, Max);

			// render title
			ImGui::SetCursorPos(ImVec2{ 0,0 });
			ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
			UI::SubHeader(MapTitle.c_str());
			ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });


			// render back button
			ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.02f,viewport.Size.y * 0.03f });
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
			image = tex_map["Assets/BackMenuBtn.png"];

			if (ImGui::ImageButton((void*)static_cast<size_t>(image->GetID()), ImVec2{ image->GetWidth() * 0.7f, image->GetHeight() * 0.7f }))
				MainMenuUI = UI_SHOW::NEW_PROJECT;

			ImGui::PopStyleColor(3);

			ImGui::PushFont(FONT_BTN);
			// render bottom two buttons
			if (UI::UIButton_2("New Map", "New Map", ImVec2{ viewport.Size.x * 0.34f, viewport.Size.y * 0.85f }, { 0,0 }, FONT_BTN))
			{

			}

			if (UI::UIButton_2("Load Map", "Load Map", ImVec2{ viewport.Size.x * 0.66f, viewport.Size.y * 0.85f }, { 0,0 }, FONT_BTN))
				MainMenuUI = UI_SHOW::LOAD_MAP;
			
			ImGui::PopFont();
		}
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::LOAD_MAP:
		{
			// render the select map image
			auto image = tex_map["Assets/MM_LoadMap.png"];
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
			ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.02f,viewport.Size.y * 0.03f });
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
			image = tex_map["Assets/BackMenuBtn.png"];

			if (ImGui::ImageButton((void*)static_cast<size_t>(image->GetID()), ImVec2{ image->GetWidth() * 0.7f, image->GetHeight() * 0.7f }))
				MainMenuUI = UI_SHOW::SELECT_MAP;

			ImGui::PopStyleColor(3);

			// draw the child
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 10.0f, 10.f });
			const ImVec2 child_size{viewport.Size.x * 0.5f, viewport.Size.y * 0.55f};
			ImGui::SetCursorPos(ImVec2{viewport.Size.x * 0.65f - child_size.x * 0.5f, viewport.Size.y * 0.5f - child_size.y * 0.5f });
			if (ImGui::BeginChild("##LoadMapMainMenu", child_size, true))
			{
				const std::pair<bool, bool> map_pair = UI::UIMapSelectable("MAP_01", "Date created: 12/31/2021", false, 1);

				// render all the maps here
				if (map_pair.first)
				{
					MainMenuUI = UI_SHOW::SELECT_CONFLICT_RES;
				}

				else if (map_pair.second)
				{
					// TODO: delete map
				}
			}

			ImGui::EndChild();
			ImGui::PopStyleVar();
		}
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::SELECT_CONFLICT_RES:
		{
			// render the select map image
			auto image = tex_map["Assets/MM_SelectCR.png"];
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
			ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.02f,viewport.Size.y * 0.03f });
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
			image = tex_map["Assets/BackMenuBtn.png"];

			if (ImGui::ImageButton((void*)static_cast<size_t>(image->GetID()), ImVec2{ image->GetWidth() * 0.7f, image->GetHeight() * 0.7f }))
				MainMenuUI = UI_SHOW::LOAD_MAP;

			ImGui::PopStyleColor(3);

			// draw the child
			const ImVec2 child_size{ viewport.Size.x * 0.25f, viewport.Size.y * 0.55f };
			ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.5f - child_size.x * 0.5f, viewport.Size.y * 0.55f - child_size.y * 0.5f });
			if (ImGui::BeginChild("##LoadConflictResMainMenu", child_size, true))
			{
				const ImVec2 cusor{ ImGui::GetCursorPosX() + 200.0f, ImGui::GetCursorPosY() + 40.0f };
				// TODO: load the conflict stuff here

				for (int i = 0; i < 4; i++)
				{
					ImGui::PushID(std::string{ "ConflictRes" + std::to_string(i) }.c_str());
					if (UI::UIButton_2("Sample_Conflict", "Sample_Conflict", ImVec2{ cusor.x, cusor.y + i * 90.0f}, { 50,20 }, FONT_BTN, SelectedConflictRes == i))
					{
						SelectedConflictRes = i;
					}
					ImGui::PopID();
				}
			}

			ImGui::EndChild();

			ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.8f - child_size.x * 0.5f, viewport.Size.y * 0.55f - child_size.y * 0.5f });
			if (ImGui::BeginChild("##LoadSequenceMainMenu", child_size, true))
			{
				const ImVec2 cusor{ ImGui::GetCursorPosX() + 200.0f, ImGui::GetCursorPosY() + 40.0f };

				// TODO: render all the sequences from selected conflict
				// TODO: make a popup menu
				for (int i = 0; i < 8; i++)
				{
					std::string seq_name = "Sample_Seq" + std::to_string(i);
					ImGui::PushID(seq_name.c_str());
					bool selected = std::find(SelectedSequences.begin(), SelectedSequences.end(), i) != SelectedSequences.end();
					if (UI::UIButton_2(seq_name.c_str(), seq_name.c_str(), ImVec2{ cusor.x, cusor.y + i * 90.0f }, { 50,20 }, FONT_BTN, selected))
					{
						Service<EventManager>::Get().instant_dispatch< MainMenuSequencePopupTrigger>(SelectedSequences);
					}
					ImGui::PopID();
				}
			}

			ImGui::EndChild();

			if (UI::UIButton_2("Next", "Next", ImVec2{viewport.Size.x * 0.9f, viewport.Size.y * 0.95f }, { -20,20 }, FONT_BTN))
			{
				// TODO: bring to add units page
			}
		}
			break;
		default:
			break;
		}
	}
}