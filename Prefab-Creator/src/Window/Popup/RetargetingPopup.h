/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
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
    class RetargetingPopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }

        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<RetargetingTrigger>(&RetargetingPopup::open_popup, this);
        }

        void open_popup(const Event& e)
        {
            auto a = event_cast<RetargetingTrigger>(e);
            enable_popup = true;
        }

        void show(Instance&) override
        {
            const auto wrap_width = 200.f;

            if (enable_popup)
            {
                ImGui::OpenPopup("No resource folder targeted!");
                enable_popup = false;
            }

            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            if (ImGui::BeginPopupModal("No resource folder targeted!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text(ICON_FA_EXCLAMATION_CIRCLE);
                ImGui::SameLine();

                ImVec2 pos = ImGui::GetCursorScreenPos();
                ImGui::Text("Please target the resource directory");
                ImGui::Text("\n");

                if (ImGui::Button("Browse...")) {

                    ImGui::CloseCurrentPopup();
                    Service<EventManager>::Get().instant_dispatch<TargetingTrigger>();
                    
                }
                ImGui::EndPopup();
            }


        }

        string data;
        bool enable_popup = false;
    };
}