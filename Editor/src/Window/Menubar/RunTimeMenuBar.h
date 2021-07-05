#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"

namespace Tempest
{
	class RunTimeMenuBar : public Window
	{
		const char* window_name() override
		{
			return "RunTimeMenuBar";
		}
		void show(Instance& instance) override
		{
            auto& runtime = dynamic_cast<RuntimeInstance&>(instance);

			if(ImGui::BeginMainMenuBar())
			{
				if (ImGui::Button(ICON_FA_STOP, {25.f, 25.f}))
				{
                    Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(
						runtime.get_full_path(), 
						MemoryStrategy{}, 
						InstanceType::EDIT_TIME);
				}

				if (ImGui::BeginMenu("Project"))
				{
					if (ImGui::MenuItem(ICON_FA_FILE_MEDICAL " New", "Ctrl+N", false)){}

					if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open", "", false)){}
					if (ImGui::BeginMenu(ICON_FA_BOX_OPEN " Open Recent")){}
					if (ImGui::MenuItem(ICON_FA_SAVE " Save", "Ctrl+S", false)){}

					if (ImGui::MenuItem(ICON_FA_FILE_EXCEL " Close", "", false)){}

					UI::PaddedSeparator(1.f);
					if (ImGui::MenuItem(ICON_FA_FILE_EXPORT " Export", "Ctrl+Shift+S", false)){}
					UI::PaddedSeparator(1.f);

					if (ImGui::MenuItem(ICON_FA_FILE_IMPORT " Import Asset", "", false)){}

					UI::PaddedSeparator(1.f);

					if (ImGui::MenuItem(ICON_FA_ARROW_LEFT " Back to Main Menu", "", false)){}
					if (ImGui::MenuItem(ICON_FA_DOOR_OPEN " Exit", "", false)){}

					
					ImGui::EndMenu();
				}



				ImGui::EndMainMenuBar();
			}
		}

	};
}