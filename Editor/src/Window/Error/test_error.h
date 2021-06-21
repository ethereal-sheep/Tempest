#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"

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
            Service<EventManager>::Get().register_listener<TEvent<string>>(&test_error::open_popup, this);
        }

        void open_popup(const Event& e)
        {
            auto a = event_cast<TEvent<string>>(e);
            data = a.i;
            enable_popup = true;
        }

		void show(Instance&) override
		{
            if (enable_popup)
            {
                ImGui::OpenPopup("test_error?");
                enable_popup = false;
            }

            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            if (ImGui::BeginPopupModal("test_error?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("test_error!\n\n");
                ImGui::Separator();

                UI::RenderText("Test Error:", 70.f);
                ImGui::Text(data.c_str());

                if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }
		}
        
        string data;
        bool enable_popup = false;
	};
}