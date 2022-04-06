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
    class Startup : public Window
    {
    public:
        /*Startup(bool fade_in = false) : Window()
        {
            if (fade_in)
            {
                LoadTrigger a;
                run_until_true = a.do_until_true_fn;
                to_do_at_end = a.do_at_end_fn;

                state = state::VISIBLE_SKIP;
            }
        }*/

        enum struct state
        {
            APPEAR,
            FADING_IN,
            INVISIBLE_LOGO_1,
            FADING_IN_LOGO_1,
            VISIBLE_LOGO_1,
            FADE_OUT_LOGO_1,

            INVISIBLE_LOGO_2,
            FADING_IN_LOGO_2,
            VISIBLE_LOGO_2,
            FADE_OUT_LOGO_2,

            BLACK,
            FADING_OUT,
            INVISIBLE
        };

        float 
            fade_in_time = .5f, 
            logo_fade_in_time = 1.f, 
            fade_out_time = .5f, 
            visible_time = 1.f,
            invisible_time = 1.f;

        std::function<bool(void)> run_until_true;
        std::function<void(void)> to_do_at_end;
        interpolater<float> inter;
        state state = state::APPEAR;

        string logo1 = "Assets/DigiPen_Logo_RED.png";
        string logo2 = "Assets/Placeholder_Character.dds";

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

                inter.start(end, end, fade_in_time, 0, [](float x) { return glm::sineEaseOut(x); });
                state = state::FADING_IN;
            }
            [[fallthrough]];
            case state::FADING_IN:
                ImGui::SetNextWindowFocus();
                ImGui::SetNextWindowBgAlpha(inter.get()); // Transparent background
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, inter.get()));
                if (ImGui::Begin("LOADING##LOADING", nullptr, window_flags))
                {
                }
                ImGui::End();
                ImGui::PopStyleColor(1);
                if (inter.is_finished())
                {
                    state = state::INVISIBLE_LOGO_1;
                    inter.start(end, end, invisible_time);
                }
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
                {
                    state = state::INVISIBLE_LOGO_1;
                    inter.start(end, end, invisible_time);
                }

                break;
                //[[fallthrough]];
            case state::INVISIBLE_LOGO_1:
                ImGui::SetNextWindowFocus();
                ImGui::SetNextWindowBgAlpha(1); // Transparent background
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, inter.get()));
                if (ImGui::Begin("LOADING##LOADING", nullptr, window_flags))
                {
                }
                ImGui::End();
                ImGui::PopStyleColor(1);
                if (inter.is_finished())
                {
                    state = state::FADING_IN_LOGO_1;
                    inter.start(0, end, invisible_time);
                }
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
                {
                    state = state::FADING_IN_LOGO_1;
                    inter.start(0, end, invisible_time);
                }
                break;
            case state::FADING_IN_LOGO_1:
                ImGui::SetNextWindowFocus();
                ImGui::SetNextWindowBgAlpha(end); // Transparent background
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, inter.get()));
                if (ImGui::Begin("LOADING##LOADING", nullptr, window_flags))
                {
                    auto viewport = ImGui::GetMainViewport();
                    auto title_img = tex_map[logo1];
                    const ImVec2 title_size{ title_img->GetWidth() * 1.0f, title_img->GetHeight() * 1.0f };
                    //button_pos.x = viewport.Size.x * 0.5f - title_size.x * 0.5f;
                    ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.5f - title_size.x * 0.5f, viewport->Size.y * 0.5f - title_size.y * 0.5f });
                    ImGui::Image((void*)static_cast<size_t>(title_img->GetID()), title_size, { 0, 0 }, { 0.999f, 0.999f }, { 1,1,1, inter.get() });
                }
                ImGui::End();
                ImGui::PopStyleColor(1);
                if (inter.is_finished())
                {
                    state = state::VISIBLE_LOGO_1;
                    inter.start(end, end, visible_time);
                }
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
                {
                    state = state::VISIBLE_LOGO_1;
                    inter.start(end, end, visible_time);
                }


                break;
            case state::VISIBLE_LOGO_1:
            {
                ImGui::SetNextWindowFocus();
                ImGui::SetNextWindowBgAlpha(end); // Transparent background
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, inter.get()));
                if (ImGui::Begin("LOADING##LOADING", nullptr, window_flags))
                {
                    auto viewport = ImGui::GetMainViewport();
                    auto title_img = tex_map[logo1];
                    const ImVec2 title_size{ title_img->GetWidth() * 1.0f, title_img->GetHeight() * 1.0f };
                    //button_pos.x = viewport.Size.x * 0.5f - title_size.x * 0.5f;
                    ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.5f - title_size.x * 0.5f, viewport->Size.y * 0.5f - title_size.y * 0.5f });
                    ImGui::Image((void*)static_cast<size_t>(title_img->GetID()), title_size, { 0, 0 }, { 0.999f, 0.999f }, { 1,1,1, 1 });
                }
                ImGui::End();
                ImGui::PopStyleColor(1);
                if (inter.is_finished())
                {
                    state = state::FADE_OUT_LOGO_1;
                    inter.start(end, 0, logo_fade_in_time);
                }
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
                {
                    state = state::INVISIBLE_LOGO_2;
                    inter.start(end, end, invisible_time);
                }
                break;
            }
            case state::FADE_OUT_LOGO_1:
            {
                // skip next frame after 
                ImGui::SetNextWindowFocus();
                ImGui::SetNextWindowBgAlpha(end); // Transparent background
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, inter.get()));
                if (ImGui::Begin("LOADING##LOADING", nullptr, window_flags))
                {
                    auto viewport = ImGui::GetMainViewport();
                    auto title_img = tex_map[logo1];
                    const ImVec2 title_size{ title_img->GetWidth() * 1.0f, title_img->GetHeight() * 1.0f };
                    //button_pos.x = viewport.Size.x * 0.5f - title_size.x * 0.5f;
                    ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.5f - title_size.x * 0.5f, viewport->Size.y * 0.5f - title_size.y * 0.5f });
                    ImGui::Image((void*)static_cast<size_t>(title_img->GetID()), title_size, { 0, 0 }, { 0.999f, 0.999f }, { 1,1,1, inter.get() });
                }
                ImGui::End();
                ImGui::PopStyleColor(1);
                if (inter.is_finished())
                {
                    state = state::INVISIBLE_LOGO_2;
                    inter.start(end, end, invisible_time);
                }
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
                {
                    state = state::INVISIBLE_LOGO_2;
                    inter.start(end, end, invisible_time);
                }
                break;
            }
            case state::INVISIBLE_LOGO_2:
                ImGui::SetNextWindowFocus();
                ImGui::SetNextWindowBgAlpha(end); // Transparent background
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, inter.get()));
                if (ImGui::Begin("LOADING##LOADING", nullptr, window_flags))
                {
                }
                ImGui::End();
                ImGui::PopStyleColor(1);
                if (inter.is_finished())
                {
                    state = state::FADING_IN_LOGO_2;
                    inter.start(0, end, invisible_time);
                }
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
                {
                    state = state::FADING_IN_LOGO_2;
                    inter.start(0, end, invisible_time);
                }
                break;
            case state::FADING_IN_LOGO_2:
                ImGui::SetNextWindowFocus();
                ImGui::SetNextWindowBgAlpha(end); // Transparent background
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, inter.get()));
                if (ImGui::Begin("LOADING##LOADING", nullptr, window_flags))
                {
                    auto viewport = ImGui::GetMainViewport();
                    auto title_img = tex_map[logo2];
                    const ImVec2 title_size{ title_img->GetWidth() * 1.0f, title_img->GetHeight() * 1.0f };
                    //button_pos.x = viewport.Size.x * 0.5f - title_size.x * 0.5f;
                    ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.5f - title_size.x * 0.5f, viewport->Size.y * 0.5f - title_size.y * 0.5f });
                    ImGui::Image((void*)static_cast<size_t>(title_img->GetID()), title_size, { 0, 0 }, { 0.999f, 0.999f }, { 1,1,1, inter.get() });
                }
                ImGui::End();
                ImGui::PopStyleColor(1);
                if (inter.is_finished())
                {
                    state = state::VISIBLE_LOGO_2;
                    inter.start(end, end, visible_time);
                }
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
                {
                    state = state::VISIBLE_LOGO_2;
                    inter.start(end, end, visible_time);
                }
                break;
            case state::VISIBLE_LOGO_2:
            {
                ImGui::SetNextWindowFocus();
                ImGui::SetNextWindowBgAlpha(end); // Transparent background
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, inter.get()));
                if (ImGui::Begin("LOADING##LOADING", nullptr, window_flags))
                {
                    auto viewport = ImGui::GetMainViewport();
                    auto title_img = tex_map[logo2];
                    const ImVec2 title_size{ title_img->GetWidth() * 1.0f, title_img->GetHeight() * 1.0f };
                    //button_pos.x = viewport.Size.x * 0.5f - title_size.x * 0.5f;
                    ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.5f - title_size.x * 0.5f, viewport->Size.y * 0.5f - title_size.y * 0.5f });
                    ImGui::Image((void*)static_cast<size_t>(title_img->GetID()), title_size, { 0, 0 }, { 0.999f, 0.999f }, { 1,1,1, 1 });
                }
                ImGui::End();
                ImGui::PopStyleColor(1);
                if (inter.is_finished())
                {
                    state = state::FADE_OUT_LOGO_2;
                    inter.start(end, 0, logo_fade_in_time);
                }
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
                {
                    state = state::BLACK;
                    inter.start(end, end, invisible_time);
                }
                break;
            }
            case state::FADE_OUT_LOGO_2:
            {
                // skip next frame after 
                ImGui::SetNextWindowFocus();
                ImGui::SetNextWindowBgAlpha(end); // Transparent background
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, inter.get()));
                if (ImGui::Begin("LOADING##LOADING", nullptr, window_flags))
                {
                    auto viewport = ImGui::GetMainViewport();
                    auto title_img = tex_map[logo2];
                    const ImVec2 title_size{ title_img->GetWidth() * 1.0f, title_img->GetHeight() * 1.0f };
                    //button_pos.x = viewport.Size.x * 0.5f - title_size.x * 0.5f;
                    ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.5f - title_size.x * 0.5f, viewport->Size.y * 0.5f - title_size.y * 0.5f });
                    ImGui::Image((void*)static_cast<size_t>(title_img->GetID()), title_size, { 0, 0 }, { 0.999f, 0.999f }, { 1,1,1, inter.get() });
                }
                ImGui::End();
                ImGui::PopStyleColor(1);
                if (inter.is_finished())
                {
                    state = state::BLACK;
                    inter.start(end, end, invisible_time);
                }
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
                {
                    state = state::BLACK;
                    inter.start(end, end, invisible_time);
                }
                break;
            }
            case state::BLACK:
                ImGui::SetNextWindowFocus();
                ImGui::SetNextWindowBgAlpha(end); // Transparent background
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, inter.get()));
                if (ImGui::Begin("LOADING##LOADING", nullptr, window_flags))
                {
                }
                ImGui::End();
                ImGui::PopStyleColor(1);
                if (inter.is_finished())
                {
                    state = state::FADING_OUT;
                    inter.start(end, 0, fade_out_time);
                }
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
                {
                    state = state::FADING_OUT;
                    inter.start(end, 0, fade_out_time);
                }
                break;
            case state::FADING_OUT:
                ImGui::SetNextWindowFocus();
                ImGui::SetNextWindowBgAlpha(inter.get()); // Transparent background
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, inter.get()));
                if (ImGui::Begin("LOADING##LOADING", nullptr, window_flags))
                {
                }
                ImGui::End();
                ImGui::PopStyleColor(1);
                if (inter.is_finished())
                {
                    state = state::INVISIBLE;
                }
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
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