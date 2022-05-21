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
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"

namespace Tempest
{
    class ErrorMsgPopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }

        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<ErrorTrigger>(&ErrorMsgPopup::open_popup, this);
        }

        void open_popup(const Event& e)
        {
            auto a = event_cast<ErrorTrigger>(e);
            data = a.msg;
            enable_popup = true;
        }

        void show(Instance&) override
        {
            const auto wrap_width = 200.f;

            if (enable_popup)
            {
                ImGui::OpenPopup("Error has occured!");
                enable_popup = false;
            }

            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(0.f, 0.0f), ImGuiCond_Appearing);
            if (ImGui::BeginPopupModal("Error has occured!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text(ICON_FA_EXCLAMATION_CIRCLE);
                ImGui::SameLine();

                ImVec2 pos = ImGui::GetCursorScreenPos();
                ImVec2 marker_min = ImVec2(pos.x + wrap_width, pos.y);
                ImVec2 marker_max = ImVec2(pos.x + wrap_width + 10, pos.y + ImGui::GetTextLineHeight());
                ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
                ImGui::Text(data.c_str(), wrap_width);
                ImGui::PopTextWrapPos();
                ImGui::Text("\n");

                if (ImGui::Button("OK", ImVec2(200, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }
        }

        string data;
        bool enable_popup = false;
    };
}