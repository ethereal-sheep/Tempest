#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"

namespace Tempest
{
	class EditTimeMenuBar : public Window
	{
		const char* window_name() override
		{
			return "EditTimeMenuBar";
		}
		void show(Instance& instance) override
		{
			if(ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Project"))
				{
					if (ImGui::MenuItem(ICON_FA_FILE_MEDICAL " New", "Ctrl+N", false))
					{
						Service<EventManager>::Get().instant_dispatch<OverlayTrigger>("Creating new project...");
					}

					if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open", "", false))
					{
						Service<EventManager>::Get().instant_dispatch<OverlayTrigger>("Opening...");
					}
					if (ImGui::BeginMenu(ICON_FA_BOX_OPEN " Open Recent"))
					{
						for (auto i = 0; i < 3; ++i)
						{
							if (ImGui::MenuItem((string("File") + std::to_string(i)).c_str(), "", false, false))
							{

							}
						}


						ImGui::EndMenu();

					}
					if (ImGui::MenuItem(ICON_FA_SAVE " Save", "Ctrl+S", false))
					{
						Service<EventManager>::Get().instant_dispatch<OverlayTrigger>("Saving...");
					}

					if (ImGui::MenuItem(ICON_FA_SAVE " Save As...", "Ctrl+Shift+S", false))
					{
						Service<EventManager>::Get().instant_dispatch<OverlayTrigger>("Saving...");
					}

					if (ImGui::MenuItem(ICON_FA_FILE_EXCEL " Close", "", false))
					{

					}

					UI::PaddedSeparator(1.f);

					if (ImGui::MenuItem(ICON_FA_ARROW_LEFT " Back to Main Menu", "", false))
					{
						Service<EventManager>::Get().instant_dispatch<OverlayTrigger>("Going back to main menu in 10s...");
					}
					if (ImGui::MenuItem(ICON_FA_DOOR_OPEN " Exit", "", false))
					{
						Service<EventManager>::Get().instant_dispatch<OverlayTrigger>("Application Exiting in 10s...");
					}


					ImGui::EndMenu();
				}


				ImGui::EndMainMenuBar();
			}
		}

	};
}