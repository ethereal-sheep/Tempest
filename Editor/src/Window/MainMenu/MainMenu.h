#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"
#include "InstanceManager/InstanceConfig.h"

namespace Tempest
{
	class MainMenuWindow : public Window
	{

		const char* window_name() override
		{
			return ICON_FA_DUNGEON " Some Cool Editor";
		}

		void init() override
		{
			window_flags |= 
				ImGuiWindowFlags_NoResize | 
				ImGuiWindowFlags_NoDocking | 
				ImGuiWindowFlags_NoCollapse | 
				ImGuiWindowFlags_NoMove | 
				ImGuiWindowFlags_MenuBar;
		}

		void show(Instance& ) override
		{
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImVec2 size = ImGui::GetMainViewport()->Size;
			ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
			ImGui::SetNextWindowSize(ImVec2(size.x / 1.5f, size.y / 1.5f), ImGuiCond_Always);
			if(ImGui::Begin(window_name(), nullptr, window_flags))
			{
				// menu bar
				{
					if (ImGui::BeginMenuBar())
					{
						if (ImGui::BeginMenu("Project"))
						{
							if (ImGui::MenuItem(ICON_FA_FILE_MEDICAL " New", "Ctrl+N", false))
							{
								Service<EventManager>::Get().instant_dispatch<OverlayTrigger>("Creating new project...");
								Service<EventManager>::Get().instant_dispatch<NewProjectTrigger>();

							}

							if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open", "", false))
							{
								Service<EventManager>::Get().instant_dispatch<OverlayTrigger>("Opening...");
								Service<EventManager>::Get().instant_dispatch<OpenProjectTrigger>();
							}
							if (ImGui::BeginMenu(ICON_FA_BOX_OPEN " Open Recent"))
							{
								Service<EventManager>::Get().instant_dispatch<ShowRecentUtil>();
								ImGui::EndMenu();

							}
							if (ImGui::MenuItem(ICON_FA_SAVE " Save", "Ctrl+S", false, false)) {}
							//if (ImGui::MenuItem(ICON_FA_SAVE " Save As...", "Ctrl+Shift+S", false, false)) {}
							if (ImGui::MenuItem(ICON_FA_FILE_EXCEL " Close", "", false, false)) {}

							UI::PaddedSeparator(1.f);

							if (ImGui::MenuItem(ICON_FA_FILE_EXPORT " Export", "Ctrl+Shift+S", false, false)) {}

							UI::PaddedSeparator(1.f);

							if (ImGui::MenuItem(ICON_FA_ARROW_LEFT " Back to Main Menu", "", false, false))
							{
							}
							if (ImGui::MenuItem(ICON_FA_DOOR_OPEN " Exit", "", false))
							{
								Service<EventManager>::Get().instant_dispatch<OverlayTrigger>("Application Exiting in 10s...");
							}

							ImGui::EndMenu();
						}


						ImGui::EndMenuBar();
					}
				}

			}
			ImGui::End();
		}


	};
}