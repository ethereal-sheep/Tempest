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
		void show(Instance& ) override
		{
			if(ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Project"))
				{
					if (ImGui::MenuItem(ICON_FA_FILE_MEDICAL " New", "Ctrl+N", false))
					{
						Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Creating new project...");
						Service<EventManager>::Get().instant_dispatch<NewProjectTrigger>();
					}

					if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open", "", false))
					{
						Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Opening...");
						Service<EventManager>::Get().instant_dispatch<OpenProjectTrigger>();
					}
					if (ImGui::BeginMenu(ICON_FA_BOX_OPEN " Open Recent"))
					{

						Service<EventManager>::Get().instant_dispatch<ShowRecentUtil>();

						ImGui::EndMenu();

					}
					if (ImGui::MenuItem(ICON_FA_SAVE " Save", "Ctrl+S", false))
					{
						Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Saving...");
						Service<EventManager>::Get().instant_dispatch<SaveProjectTrigger>();
					}

					/*if (ImGui::MenuItem(ICON_FA_SAVE " Save As...", "Ctrl+Shift+S", false))
					{
						Service<EventManager>::Get().instant_dispatch<OverlayTrigger>("Saving...");
						Service<EventManager>::Get().instant_dispatch<SaveProjectAsTrigger>();
					}*/

					if (ImGui::MenuItem(ICON_FA_FILE_EXCEL " Close", "", false))
					{
						Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Closing...");
						Service<EventManager>::Get().instant_dispatch<CloseProjectTrigger>();
					}

					UI::PaddedSeparator(1.f);
					if (ImGui::MenuItem(ICON_FA_FILE_EXPORT " Export", "Ctrl+Shift+S", false)) 
					{
						Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Exporting...");
					}
					UI::PaddedSeparator(1.f);

					if (ImGui::MenuItem(ICON_FA_ARROW_LEFT " Back to Main Menu", "", false))
					{
						Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Going back to main menu in 10s...");
					}
					if (ImGui::MenuItem(ICON_FA_DOOR_OPEN " Exit", "", false))
					{
						Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Application Exiting in 10s...");
					}


					ImGui::EndMenu();
				}


				ImGui::EndMainMenuBar();
			}
		}

	};
}