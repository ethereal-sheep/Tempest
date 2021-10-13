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
#include "Instance/EditTimeInstance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"

namespace Tempest
{

    class SaveCurrentBeforeOpenPopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<SaveCurrentBeforeOpenTrigger>(&SaveCurrentBeforeOpenPopup::open_popup, this);
        }

        void open_popup(const Event& e)
        {
            auto a = event_cast<SaveCurrentBeforeOpenTrigger>(e);
            path = a.open_path;
            enable_popup = true;
        }

        void show(Instance& instance) override
        {
            auto* edit = dynamic_cast<EditTimeInstance*>(&instance);
            const string popup_name = "Save Current?";

            // enable browser
            if (enable_popup)
            {
                if (edit)
                {
                    ImGui::OpenPopup(popup_name.c_str());
                }
                else
                {
                    Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(path, MemoryStrategy{ DebugFlag::DEBUG_STRICT }, InstanceType::EDIT_TIME);
                }

                enable_popup = false;
            }

            if (ImGui::BeginPopupModal(popup_name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Would you like to save");
                ImGui::SameLine();
                ImGui::PushFont(FONT_BOLD);
                ImGui::Text("%s?", instance.get_name().c_str());
                ImGui::PopFont();
                ImGui::Text("\n");

                if (ImGui::Button("Save"))
                {
                    edit->save();
                    Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(path, MemoryStrategy{DebugFlag::DEBUG_STRICT}, InstanceType::EDIT_TIME);
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SameLine();
                if (ImGui::Button("Discard Changes"))
                {
                    Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(path, MemoryStrategy{ DebugFlag::DEBUG_STRICT }, InstanceType::EDIT_TIME);
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        bool enable_popup = false;
        tpath path;


    };
}