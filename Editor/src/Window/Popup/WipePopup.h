/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
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
#include "Util/interpolater.h"

namespace Tempest
{
    class WipePopup : public Window
    {

        enum struct state
        {
            APPEAR,
            FADING_IN,
            VISIBLE,
            FADING_OUT,
            INVISIBLE
        };

        float fade_in_time = 0, fade_out_time = 0, visible_time = 0;
        std::function<void(void)> to_do;
        interpolater<float> inter;
        state state = state::INVISIBLE;

        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoDocking |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing |
                ImGuiWindowFlags_NoNav;

            Service<EventManager>::Get().register_listener<WipeTrigger>(&WipePopup::open_popup, this);
        }

        void open_popup(const Event& e)
        {
            if (state == state::INVISIBLE)
            {
                auto& a = event_cast<WipeTrigger>(e);

                fade_in_time = a.fade_in_time;
                fade_out_time = a.fade_out_time;
                visible_time = a.visible_time;
                to_do = a.do_on_fade;

                state = state::APPEAR;
            }
        }


        void show(Instance&) override
        {
            const auto end = 1.f;
            inter.update(ImGui::GetIO().DeltaTime);
            switch (state)
            {
            case state::APPEAR:
            {
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImVec2 work_pos = viewport->Pos; // Use work area to avoid menu-bar/task-bar, if any!
                ImVec2 work_size = viewport->Size;
                ImVec2 window_pos, window_pos_pivot;
                window_pos.x = work_pos.x + work_size.x;
                window_pos.y = work_pos.y + work_size.y;
                window_pos_pivot.x = 1.0f;
                window_pos_pivot.y = 1.0f;
                ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
                ImGui::SetNextWindowSize(viewport->Size);
                ImGui::SetNextWindowViewport(viewport->ID);
                window_flags |= ImGuiWindowFlags_NoMove;

                inter.start(0, end, fade_in_time, 0, [](float x) { return glm::sineEaseOut(x); });
                state = state::FADING_IN;
            }
            [[fallthrough]];
            case state::FADING_IN:
                ImGui::SetNextWindowFocus();
                ImGui::SetNextWindowBgAlpha(inter.get()); // Transparent background
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, inter.get()));
                if (ImGui::Begin("WIPE##WIPE", nullptr, window_flags))
                {
                }
                ImGui::End();
                ImGui::PopStyleColor(1);
                if (inter.is_finished())
                {
                    state = state::VISIBLE;
                    std::invoke(to_do);
                    inter.start(0, end, visible_time);
                }
                break;
            case state::VISIBLE:
            {
                ImGui::SetNextWindowBgAlpha(end); // Transparent background
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, inter.get()));
                if (ImGui::Begin("WIPE##WIPE", nullptr, window_flags))
                {
                }
                ImGui::End();
                ImGui::PopStyleColor(1);
                if (inter.is_finished())
                {
                    state = state::FADING_OUT;
                    inter.start(end, 0, fade_out_time, 0, [](float x) { return glm::sineEaseIn(x); });
                }
                break;
            }
            case state::FADING_OUT:
                ImGui::SetNextWindowBgAlpha(inter.get()); // Transparent background
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, inter.get()));
                if (ImGui::Begin("WIPE##WIPE", nullptr, window_flags))
                {
                }
                ImGui::End();
                ImGui::PopStyleColor(1);
                if (inter.is_finished())
                {
                    state = state::INVISIBLE;
                }
                break;
            case state::INVISIBLE:
            default:
                break;
            }

        }



    };
}