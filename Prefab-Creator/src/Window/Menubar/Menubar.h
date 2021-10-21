/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Instance/Instance.h"
#include "../Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"

namespace Tempest
{
	class Menubar : public Window
	{
		const char* window_name() override
		{
			return "Menubar";
		}
		void show(Instance& instance) override
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem(ICON_FA_COGS " Settings", " ", false))
					{
						//Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Creating new project...");
						Service<EventManager>::Get().instant_dispatch<SettingTrigger>();
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Edit", instance.selected != INVALID))
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