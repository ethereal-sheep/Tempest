#include "SimulateOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"
#include "MainMenuOverlay.h"

namespace Tempest
{
	void MainMenuOverlay::open_popup(const Event&)
	{
		OverlayOpen = true;
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
		}
	}

	void MainMenuOverlay::OpenLocalUI(Instance& instance, const ImGuiViewport& viewport)
	{
		if (MainMenuUI == UI_SHOW::NONE)
			return;

		// Title
		if (MainMenuUI < UI_SHOW::LOAD_MAP_UI)
		{
			std::string title = "INSERT PROJECT NAME HERE";
			ImGui::PushFont(FONT_HEAD);
			ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.5f - ImGui::CalcTextSize(title.c_str()).x * 0.5f, viewport.Size.y * 0.2f });
			ImGui::Text(title.c_str());
			ImGui::PopFont();
		}


		ImVec2 buttonPos{ 0.f, viewport.Size.y * 0.7f };
		switch (MainMenuUI)
		{
			// TODO: more local functions
		case Tempest::MainMenuOverlay::UI_SHOW::INITIAL:
			ImGui::PushFont(FONT_BTN);
			ImGui::SetCursorPos(buttonPos);
			buttonPos.y += 40.0f;
			if (UI::UISelectable("Maps", false, 0))
				MainMenuUI = UI_SHOW::MAP_UI;

			ImGui::SetCursorPos(buttonPos);
			buttonPos.y += 40.0f;
			if (UI::UISelectable("Conflict Resolutions", false, 0))
				MainMenuUI = UI_SHOW::CONFLICT_UI;

			ImGui::SetCursorPos(buttonPos);
			buttonPos.y += 40.0f;
			UI::UISelectable("Settings", false);

			ImGui::SetCursorPos(buttonPos);
			if (UI::UISelectable("Quit", false))
			{
				OverlayOpen = false;
				instance.window_manager.show_all();
			}

			ImGui::PopFont();
			break;

		case Tempest::MainMenuOverlay::UI_SHOW::MAP_UI:
			ImGui::PushFont(FONT_BTN);
			ImGui::SetCursorPos(buttonPos);
			buttonPos.y += 40.0f;
			if (UI::UISelectable("New", false, 0)) {}

			ImGui::SetCursorPos(buttonPos);
			buttonPos.y += 40.0f;
			if (UI::UISelectable("Load", false, 0))
			{
				MainMenuUI = UI_SHOW::LOAD_MAP_UI;
			}

			ImGui::SetCursorPos(buttonPos);
			if (UI::UISelectable("Back", false))
				MainMenuUI = UI_SHOW::INITIAL;

			ImGui::PopFont();
			break;

		case Tempest::MainMenuOverlay::UI_SHOW::LOAD_MAP_UI:
		{
			ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
			UI::SubHeader("Load Map");
			ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });

			// Content region
			ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.5f - (viewport.Size.x * 0.6f * 0.5f), viewport.Size.y * 0.25f});
			ImGui::BeginChild("##DragDropSectionSimulate", ImVec2{ viewport.Size.x * 0.6f, viewport.Size.y * 0.5f }, true, window_flags);
			int selectedItem = 0;

			const ImVec2 itemSize{ viewport.Size.x * 0.6f * 0.8f, viewport.Size.y * 0.5f * 0.25f };

			for(int i = 0 ; i < 5; i++)
			{
				ImGui::Dummy(ImVec2{0.f,20.f});
				ImGui::Dummy(ImVec2{20.f,0.f});
				ImGui::SameLine();
				std::string itemid = "##" + std::to_string(i);
				ImGui::PushID(i);
				if (ImGui::Selectable("", i == selectedItem, 0, itemSize))
				{
					selectedItem = i;
				
					LOG("SELECTED TRUE");
				}
				// ImGui::SameLine();
				// auto tex = tex_map["Assets/test_photo2.png"];
				// ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2( 30.f,20.f));
				// ImGui::SameLine();
				// ImGui::RenderText(ImVec2{ 30,40 }, "NAMEEE");
				// // ImGui::Text("DATEE");
				// // ImGui::Text("NUMBER OF UNIT CREATED");
				 ImGui::PopID();
			//	ImGui::Columns(1, "NumMaps", true);
			//	ImGuiOldColumnFlags ColFlags = ImGuiOldColumnFlags_NoResize;
			//	ImGuiWindow* window = ImGui::GetCurrentWindow();
			//	ImGuiOldColumns* columns = window->DC.CurrentColumns;
			//	columns->Flags = ColFlags;
			
				
			//	ImGui::NextColumn();
			// 	float posy = GetContentRegionAvail.y();
			// 	ImGui::Text("NAMEEEE");
			// 	ImGui::SetCursorPosY(posy*0.5f);
			// 	ImGui::Text("DATEE");
			// 	ImGui::SetCursorPosY(posy);
			// 	ImGui::Text("NUMBER OF UNIT CREATED");
			// //	ImGui::EndColumns();
			// 	ImGui::Dummy({0, 30.f});
			}
			ImGui::EndChild();

			// Buttons
			if (UI::UIButton_2("Back", "Back", ImVec2{ viewport.Size.x * 0.5f - 240.0f, viewport.Size.y * 0.9f }, ImVec2{ 0.f, 8.0f }, FONT_BODY))
				MainMenuUI = UI_SHOW::MAP_UI;

			UI::UIButton_2("Play", "Play", ImVec2{ viewport.Size.x * 0.5f + 240.0f, viewport.Size.y * 0.9f }, ImVec2{ 0.f, 8.0f }, FONT_BODY);
		}
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::CONFLICT_UI:
			ImGui::PushFont(FONT_BTN);
			ImGui::SetCursorPos(buttonPos);
			buttonPos.y += 40.0f;
			if (UI::UISelectable("Edit Existing", false, 0)) {}

			ImGui::SetCursorPos(buttonPos);
			buttonPos.y += 40.0f;
			if (UI::UISelectable("Create New", false, 0))
			{
				MainMenuUI = UI_SHOW::NONE;
				Service<EventManager>::Get().instant_dispatch<OpenConflictResTrigger>();
			}
		

			ImGui::SetCursorPos(buttonPos);
			if (UI::UISelectable("Back", false))
				MainMenuUI = UI_SHOW::INITIAL;

			ImGui::PopFont();
			break;
		case Tempest::MainMenuOverlay::UI_SHOW::SETTINGS:
			break;
		default:
			break;
		}
	}
}