#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"

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

					}

					if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open", "", false))
					{

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

					}

					if (ImGui::MenuItem(ICON_FA_SAVE " Save As...", "Ctrl+Shift+S", false))
					{

					}

					if (ImGui::MenuItem(ICON_FA_FILE_EXCEL " Close", "", false))
					{

					}

					UI::PaddedSeparator(1.f);

					if (ImGui::MenuItem(ICON_FA_ARROW_LEFT " Back to Main Menu", "", false))
					{

					}
					if (ImGui::MenuItem(ICON_FA_DOOR_OPEN " Exit", "", false))
					{
						Service<EventManager>::Get().instant_dispatch<TEvent<string>>("From Menu");
					}


					ImGui::EndMenu();
				}


				ImGui::EndMainMenuBar();
			}
		}

	};
}