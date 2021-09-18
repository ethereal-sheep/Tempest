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
           /* auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
            Entity StateLineId = UNDEFINED;
            for (auto id : StatsView)
                StateLineId = id;
            auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);*/
            
        }

        void show(Instance& instance) override
        {
            if (enable_popup)
            {
				ImGui::OpenPopup("Add Stat");
				ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
				ImGui::SetNextWindowSize(ImVec2(700, 600));
				ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove;
                auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
                Entity StateLineId = UNDEFINED;
                for (auto id : StatsView)
                    StateLineId = id;
                auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);
                
                if(PopUpClose)
                    tempStat = *sl;
                PopUpClose = false;
                
				if (ImGui::BeginPopupModal("Add Stat", NULL, flags))
				{
                    ImGui::Dummy({ 0.f, 10.f });
                    UI::SubHeader("Define Stats");
                    ImGui::Dummy({ 0.f, ImGui::GetWindowHeight() * 0.075f - 10.f });
                    ImGui::PushFont(FONT_PARA);
                    float wrap_width = ImGui::GetWindowWidth() * 0.85f;
                    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
                    string str = "Rename and select the stats that you want to use here.Checking a stat makes it available for use in Unit profiles and Actions.";
                    ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.15f);
                    ImGui::Text(str.c_str(), wrap_width);
                    ImGui::PopTextWrapPos();
                    ImGui::PopFont();
                    ImU32 lineCol = ImGui::GetColorU32({ 0.48f, 0.48f, 0.48f, 0.5f });
                    ImVec2 lineMin{ ImGui::GetMainViewport()->GetCenter().x - ImGui::GetWindowWidth() * 0.4f, ImGui::GetMainViewport()->GetCenter().y - ImGui::GetWindowHeight()* 0.27f };
                    ImVec2 lineMax{ ImGui::GetMainViewport()->GetCenter().x + ImGui::GetWindowWidth() * 0.4f, ImGui::GetMainViewport()->GetCenter().y - ImGui::GetWindowHeight() * 0.27f };

                    ImGui::GetWindowDrawList()->AddLine(
                        lineMin, lineMax, lineCol, 2.0f);
                    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);
                    ImGui::SetCursorPos({ ImGui::GetWindowWidth() * 0.1f, ImGui::GetWindowHeight() * 0.3f });
                    ImGui::PushFont(FONT_BODY);
                    for (int i = 3; i < tempStat.size(); i++)
                    {
                        if ((tempStat)(i))
                        {
                            ImVec4 col = {0.47f,0.47f,0.47f,0.6f };
                            ImVec4 textCol = { 0.9,0.9,0.9,1.f };
                            
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, col);
                            ImGui::PushStyleColor(ImGuiCol_Text, textCol);
                        }
                        else
                        {
                            ImVec4 col = { 0.28f,0.28f,0.28f,0.35f };
                            ImVec4 textCol = { 0.6f,0.6f,0.6f,1.f };
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, col);
                            ImGui::PushStyleColor(ImGuiCol_Text, textCol);
                            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                        }
                        ImGui::InputText(("##stats" + std::to_string(i)).c_str(), &(tempStat)[i]);
                        if(!(tempStat)(i))
                            ImGui::PopItemFlag();
                        ImGui::SameLine();
                        ImGui::Dummy({ 5.f,0.f });
                        ImGui::SameLine();
                        UI::UICheckBox_1(("##bool" + (tempStat)[i] + std::to_string(i)).c_str(), &(tempStat)(i));
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(7);

                        if ((i-1) % 2 == 0)
                        {
                            ImGui::SameLine();
                            ImGui::Dummy({ 10.f, 0 });
                            ImGui::SameLine();
                        }
                        else
                        {
                            ImGui::SetCursorPos({ ImGui::GetWindowWidth() * 0.1f, ImGui::GetWindowHeight() * 0.3f + ((i-2) * 30.f )});
                        }

                        ImGui::PopStyleColor(2);
                    }
                    ImGui::PopFont();
                    ImGui::PopItemWidth();

                    if (UI::UIButton_1("Save", "Save", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.3f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 80.f, 0.f }, FONT_PARA))
                    {
                        *sl = tempStat;
                        ImGui::CloseCurrentPopup();
                        enable_popup = false;
                        PopUpClose = true;
                    }

					if (UI::UIButton_1("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.6f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 80.f, 0.f }, FONT_PARA))
					{
						ImGui::CloseCurrentPopup();
                        enable_popup = false;
                        PopUpClose = true;
					}

				}

				ImGui::EndPopup();
            }
        }

        bool enable_popup = false;
        bool PopUpClose = true;
        tc::Statline tempStat;

    };
}