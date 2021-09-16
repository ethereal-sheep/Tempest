#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"
namespace Tempest
{

    class DefineStatsPopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<DefineStatsTrigger>(&DefineStatsPopup::open_popup, this);
        }

        void open_popup(const Event&)
        {
            enable_popup = true;
        }

        void show(Instance&) override
        {
            if (enable_popup)
            {
				ImGui::OpenPopup("Add Stat");
				ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
				ImGui::SetNextWindowSize(ImVec2(500, 300));
				ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoScrollbar;
				if (ImGui::BeginPopupModal("Add Stat", NULL, flags))
				{
					

					if (UI::UIButton_1("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.8f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 80.f, 0.f }, FONT_PARA))
					{
						ImGui::CloseCurrentPopup();
                        enable_popup = false;
					}

				}

				ImGui::EndPopup();
            }
        }

        bool enable_popup = false;


    };
}