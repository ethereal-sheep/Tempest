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
                ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };
                auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
                Entity StateLineId = UNDEFINED;
                for (auto id : StatsView)
                    StateLineId = id;
                auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);
                
                if(PopUpClose)
                    tempStat = *sl;
                PopUpClose = false;
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding , 0.f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
                ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
				if (ImGui::BeginPopupModal("Add Stat", NULL, flags))
				{
                    vec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y};
                    vec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 5.f};
                    vec2 winMax = { winMin.x + ImGui::GetWindowWidth()* 0.2f, winMin.y + ImGui::GetWindowHeight() * 0.05f};
                    ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
                    ImVec4 textcol = { 0,0,0,1 };
                    if (ImGui::IsWindowFocused() == false)
                    {
                        col = { 0.980f, 0.768f, 0.509f, 0.7f };
                        textcol = { 0,0,0,0.7f };
                    }
                        
                    string te = "DEFINE STATS";
                    //ImGui::GetForegroundDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
                    ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
                    ImGui::PushFont(FONT_OPEN);
                    ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), te.c_str());
                    ImGui::PopFont();
                    
                    //drawlist->AddImage((void*)static_cast<size_t>(test->GetID()), { winMin.x, winMin.y }, { winMax.x, winMax.y });

                    

                    ImGui::Dummy({ 0.f, ImGui::GetWindowHeight() * 0.15f - 10.f });
                    ImGui::PushFont(FONT_PARA);
                    float wrap_width = ImGui::GetWindowWidth() * 0.85f;
                    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
                    string str = "Rename and select the stats that you want to use here.Checking a stat makes it available for use in Unit profiles and Actions.";
                    ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.15f);
                    ImGui::Text(str.c_str(), wrap_width);
                    ImGui::PopTextWrapPos();
                    ImGui::PopFont();
                   ImU32 lineCol = ImGui::GetColorU32({ 0.48f, 0.48f, 0.48f, 0.5f });
                    ImVec2 lineMin{ ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvailWidth() * 0.1f, ImGui::GetCursorScreenPos().y };
                    ImVec2 lineMax{ ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvailWidth() * 0.9f, ImGui::GetCursorScreenPos().y };

                    ImGui::GetWindowDrawList()->AddLine(
                        lineMin, lineMax, lineCol, 2.0f);

                    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);
                    ImGui::SetCursorPos({ ImGui::GetWindowWidth() * 0.1f, ImGui::GetWindowHeight() * 0.3f });
                    ImGui::PushFont(FONT_BODY);
                    for (int i = 3; i < tc::STAT_TOTAL; i++)
                    {
                        if ((tempStat)(i))
                        {
                            ImVec4 color = {0.47f,0.47f,0.47f,0.6f };
                            ImVec4 textCol = { 0.9f,0.9f,0.9f,1.f };
                            
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, color);
                            ImGui::PushStyleColor(ImGuiCol_Text, textCol);
                        }
                        else
                        {
                            ImVec4 color = { 0.28f,0.28f,0.28f,0.35f };
                            ImVec4 textCol = { 0.6f,0.6f,0.6f,1.f };
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, color);
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
                    ImGui::SetCursorPosX(0);
                    if (UI::UIButton_1("Save", "Save", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.4f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 90.f, 0.f }, FONT_PARA))
                    {
                        *sl = tempStat;
                        ImGui::CloseCurrentPopup();
                        enable_popup = false;
                        PopUpClose = true;
                    }
                    ImGui::EndPopup();
				}

				
                ImGui::PopStyleVar(3);
                ImGui::PopStyleColor();
            }
        }

        bool enable_popup = false;
        bool PopUpClose = true;
        tc::Statline tempStat;

    };
}