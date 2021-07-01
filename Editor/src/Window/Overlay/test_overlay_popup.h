#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"

namespace Tempest
{
	class test_overlay_popup : public Window
	{
		const char* window_name() override
		{
            return "";
		}
		void init() override
		{
            window_flags = 
                ImGuiWindowFlags_NoDecoration | 
                ImGuiWindowFlags_NoDocking | 
                ImGuiWindowFlags_AlwaysAutoResize | 
                ImGuiWindowFlags_NoSavedSettings | 
                ImGuiWindowFlags_NoFocusOnAppearing | 
                ImGuiWindowFlags_NoNav;

            Service<EventManager>::Get().register_listener<OverlayTrigger>(&test_overlay_popup::open_popup, this);
		}

		void open_popup(const Event& e)
		{
			auto a = event_cast<OverlayTrigger>(e);
			data = a.msg;
            state = state::APPEAR;
		}

		void show(Instance& ) override
		{
            const auto start_visiblity = 0.95f;
            const auto visible_time = 2.f;
            const auto fade_time = 2.f;
            const auto padding = ImVec2{ 30.f, 10.f };

            switch (state)
            {
            case state::APPEAR:
            {
                const float PAD = 10.0f;
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
                ImVec2 work_size = viewport->WorkSize;
                ImVec2 window_pos, window_pos_pivot;
                window_pos.x = work_pos.x + work_size.x - PAD;
                window_pos.y = work_pos.y + work_size.y - PAD;
                window_pos_pivot.x = 1.0f;
                window_pos_pivot.y = 1.0f;
                ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
                ImGui::SetNextWindowViewport(viewport->ID);
                window_flags |= ImGuiWindowFlags_NoMove;

                auto col = ImGui::GetStyleColorVec4(ImGuiCol_Text);

                timer = 0;
                state = state::VISIBLE;
            }
                [[fallthrough]];
            case state::VISIBLE:
                ImGui::SetNextWindowBgAlpha(start_visiblity); // Transparent background
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);
                if (ImGui::Begin(data.c_str(), nullptr, window_flags))
                {
                    ImGui::PushFont(FONT_BOLD);
                    ImGui::Text(data.c_str());
                    ImGui::PopFont();
                }
                ImGui::End();
                ImGui::PopStyleVar();
                timer += ImGui::GetIO().DeltaTime;
                if (timer > visible_time)
                {
                    state = state::FADING;
                    timer = 0;
                }
                break;
            case state::FADING:
                ImGui::SetNextWindowBgAlpha(els::lerp(start_visiblity, 0.f, timer / fade_time)); // Transparent background
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);
                if (ImGui::Begin(data.c_str(), nullptr, window_flags))
                {
                    auto tx = ImGui::GetStyleColorVec4(ImGuiCol_Text);
                    auto sp = ImGui::GetStyleColorVec4(ImGuiCol_Separator);
                    tx.w = els::lerp(tx.w, 0.f, timer / fade_time);
                    sp.w = els::lerp(sp.w, 0.f, timer / fade_time);
                    
                    ImGui::PushStyleColor(ImGuiCol_Text, tx);
                    ImGui::PushStyleColor(ImGuiCol_Separator, sp);
                    ImGui::PushFont(FONT_BOLD);
                    ImGui::Text(data.c_str());
                    ImGui::PopFont();
                    ImGui::PopStyleColor(2);
                }
                ImGui::End();
                ImGui::PopStyleVar();
                timer += ImGui::GetIO().DeltaTime;

                if (timer > fade_time)
                {
                    state = state::INVISIBLE;
                    timer = 0;
                }
                
                break;
            case state::INVISIBLE:
                break;
            default:
                break;
            }
		}

        enum struct state
        {
            APPEAR,
            VISIBLE,
            FADING,
            INVISIBLE
        };

		string data;
        float visiblity = 0;
        float timer = 0;
        state state = state::INVISIBLE;

	};
}