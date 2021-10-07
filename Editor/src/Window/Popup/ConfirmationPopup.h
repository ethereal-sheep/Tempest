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
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"
namespace Tempest
{

    class ConfirmationPopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<ConfirmationTrigger>(&ConfirmationPopup::open_popup, this);
        }

        void open_popup(const Event&)
        {
            enable_popup = true;
            /* auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
             Entity StateLineId = UNDEFINED;
             for (auto id : StatsView)
                 StateLineId = id;
             auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);*/

        }

        void show(Instance& instance) override
        {
            if (enable_popup)
            {
                ImGui::OpenPopup("Add Stat");
                ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                ImGui::SetNextWindowSize(ImVec2(700, 600));
                ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove;

                if (ImGui::BeginPopupModal("Add Stat", NULL, flags))
                {
                  
                }

                ImGui::EndPopup();
            }
        }

        bool enable_popup = false;
    };
}