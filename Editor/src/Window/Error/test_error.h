#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"

namespace Tempest
{
	class test_error : public Window
	{
        const char* window_name() override
        {
            return "test_error";
        }

        void init() override
        {
            Service<EventManager>::Get().register_listener<ErrorTrigger>(&test_error::open_popup, this);
        }

        void open_popup(const Event& e)
        {
            auto a = event_cast<ErrorTrigger>(e);
            data = a.msg;
            enable_popup = true;
        }

		void show(Instance&) override
		{
            if (enable_popup)
            {
                ImGui::OpenPopup("Error has occured!");
                enable_popup = false;
            }

            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            if (ImGui::BeginPopupModal("Error has occured!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text(ICON_FA_EXCLAMATION_CIRCLE);
                ImGui::SameLine();
                const auto wrap_width = 200.f;

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