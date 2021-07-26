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
            auto& edit = dynamic_cast<EditTimeInstance&>(instance);

			if(ImGui::BeginMainMenuBar())
			{
				
				if (ImGui::Button(ICON_FA_PLAY, {25.f, 25.f}))
				{
					edit.save();
                    Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(
						edit.get_full_path(), 
						MemoryStrategy{}, 
						InstanceType::RUN_TIME);
				}

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


					if (ImGui::MenuItem(ICON_FA_FILE_IMPORT " Import Asset", "", false))
					{
						Service<EventManager>::Get().instant_dispatch<ImportAssetTrigger>();
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

				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem(ICON_FA_UNDO " Undo", "Ctrl+Z", nullptr, instance.action_history.HasUndo()))
					{
						instance.action_history.Undo(instance);
					}
					if (ImGui::MenuItem(ICON_FA_REDO " Redo", "Ctrl+Y", nullptr, instance.action_history.HasRedo()))
					{
						instance.action_history.Redo(instance);
					}

					UI::PaddedSeparator(1.f);

					if (ImGui::MenuItem(ICON_FA_COPY " Copy", "   Ctrl+C", nullptr, false))
					{
					}
					if (ImGui::MenuItem(ICON_FA_CLONE " Duplicate", "", nullptr, false))
					{
					}
					if (ImGui::MenuItem(ICON_FA_TRASH " Delete", "   Del", nullptr, false))
					{
					}


					ImGui::EndMenu();
				}


				ImGui::EndMainMenuBar();
			}
		}

	};
}