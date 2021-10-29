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
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"
#include "Font.h"

namespace Tempest
{
    class SettingPopup : public Window
    {
        const char* window_name() override
        {
            return "SettingPopup";
        }

        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<SettingTrigger>(&SettingPopup::open_popup, this);
        }

        void open_popup(const Event&)
        {
            enable_popup = true;
        }

        void show(Instance& instance) override
        {
            const auto wrap_width = 200.f;

            if (enable_popup)
            {
                ImGui::OpenPopup("Settings");
                enable_popup = false;
            }

            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            if (ImGui::BeginPopupModal("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text(ICON_FA_COGS);

                if (ImGui::BeginTabBar("SettingsTabBar", ImGuiTabBarFlags_None))
                {
                    if (ImGui::BeginTabItem("Resource Target"))
                    {
                        resource_target(instance);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("ECS Usage"))
                    {
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                ImGui::Text("\n");

                if (ImGui::Button("Close")) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }


        }

        void resource_target(Instance& instance)
        {
            ImGui::Text("Current Target: %s", instance.get_full_path().string().c_str());
            if (ImGui::Button("Retarget"))
            {
                Service<EventManager>::Get().instant_dispatch<TargetingTrigger>();
            }
            ImGui::Text("\n");
        }

        string data;
        bool enable_popup = false;
    };
}