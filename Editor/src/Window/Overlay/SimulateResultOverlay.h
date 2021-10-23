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
#include <Editor/src/Triggers/Triggers.h>
#include <Editor/src/Triggers/SimulationTriggers.h>

namespace Tempest
{
    class SimulateResultOverlay : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar;

            Service<EventManager>::Get().register_listener<OpenSimulateResultTrigger>(&SimulateResultOverlay::open_popup, this);
        }
        void open_popup(const Event& e)
        {
            auto& a = event_cast<OpenSimulateResultTrigger>(e);
            OverlayOpen = true;

            atk = a.atk;
            def = a.def;
            conflict = a.conflict;

            win = 0;
            lose = 0;

            padding = ImGui::GetMainViewport()->Size.y * 0.02f;
        }

        void show(Instance& instance) override
        {

            const ImGuiViewport* viewport = ImGui::GetMainViewport();

            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);

            if (OverlayOpen)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

                if (ImGui::Begin("SimResult", nullptr, window_flags))
                {
                    // title
                    const float contentSize = ImGui::GetContentRegionAvailWidth() * 0.9f;
                    string text = "Simulate";

                    ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
                    UI::SubHeader("Simulate");
                    ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });

                    // simulate backdrop
                    const float offsetX = (ImGui::GetContentRegionAvailWidth() - contentSize) * 0.5f;

                    auto tex = tex_map["Assets/SimulationBackdrop.png"];
                    ImVec2 point = ImGui::GetCursorPos();
                    {

                        ImVec2 Min{ point.x, point.y };
                        ImVec2 Max{ Min.x + viewport->Size.x, Min.y + viewport->Size.y * 0.73f };
                        ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
                    }

                    // display chosen sequence
                    auto& conf = instance.ecs.get<tc::Graph>(conflict);
                    if (UI::UIButton_2(conf.g.name.c_str(), conf.g.name.c_str(), { viewport->Size.x * 0.5f, viewport->Size.x * 0.15f }, { 0,0 }, FONT_PARA))
                    {

                    }

                    // success title
                    const std::string success_title{ "Chance of Success" };
                    ImGui::PushFont(FONT_HEAD);
                    ImGui::SetCursorPos({ viewport->Size.x * 0.5f - (ImGui::CalcTextSize(success_title.c_str()).x + ImGui::GetFontSize() * 0.5f) * 0.5f, viewport->Size.y * 0.4f });
                    ImGui::Text(success_title.c_str());
                    ImGui::PopFont();

                    // display attacker/defender
                    DisplayUnitSection({ viewport->Size.x * 0.18f,viewport->Size.y * 0.5f }, atk);
                    DisplayUnitSection({ viewport->Size.x * 0.82f,viewport->Size.y * 0.5f }, def);

                    float result = 0.0f;

                    // display number percentage
                    if (win && lose)
                    {
                        result = 100.f * win / (win + lose);
                        ImGui::SetCursorPos({viewport->Size.x * 0.465f, viewport->Size.y * 0.55f});
                        ImGui::PushFont(FONT_HEAD);
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(232, 137, 64, 255));
                        ImGui::Text("%.1f %%", result);
                        ImGui::PopStyleColor();
                        ImGui::PopFont();
                    }

                    // render progress bar
                    ImGui::SetCursorPos({ viewport->Size.x * 0.5f - 350.0f * 0.5f, viewport->Size.y * 0.45f });
                    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(232, 137, 64, 255));
                    ImGui::ProgressBar(result * 0.01f, { 350.0f, 50.0f });
                    ImGui::PopStyleColor();

                    const uint32_t one = 1;
                    ImGui::SetCursorPos({ viewport->Size.x * 0.45f, viewport->Size.y * 0.65f});
                    ImGui::PushFont(FONT_PARA);
                    ImGui::Text("Frequency ");
                    ImGui::PopFont();
                    ImGui::SameLine();
                    ImGui::PushItemWidth(100.f);
                    ImGui::InputScalar("", ImGuiDataType_U32, &freq, 0);
                    freq = std::clamp(freq, 1u, 100u);
                    ImGui::PopItemWidth();

                    if (UI::UIButton_2("Simulate", "Simulate", { viewport->Size.x * 0.5f, viewport->Size.y * 0.72f }, { 10.f, 10.f }, FONT_PARA))
                    {
                        freq = std::clamp(freq, 1u, 100000u);

                        Service<EventManager>::Get().instant_dispatch<SimulateConflict>(atk, def, conflict, freq, win, lose);
                    }
                
                    if (UI::UIButton_2("Back", "Back", { viewport->Size.x * 0.1f, viewport->Size.y * 0.9f }, { 0,0 }, FONT_PARA))
                    {
                        OverlayOpen = false;
                    }

                    // TODO: this button should be greyed out
                    UI::UIButton_2("Next", "Next", { viewport->Size.x * 0.9f, viewport->Size.y * 0.9f }, { 0,0 }, FONT_PARA, false);

                    {
                        ////float center_x = ImGui::GetContentRegionAvailWidth() * 0.5f;
                        //const float contentSize = ImGui::GetContentRegionAvailWidth() * 0.9f;
                        //string text = "Simulate";
                        ////float text_center = center_x - (ImGui::CalcTextSize(text.c_str()).x * 0.5f);

                        //ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
                        //UI::SubHeader("Simulate");
                        //ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });




                        //const float offsetX = (ImGui::GetContentRegionAvailWidth() - contentSize) * 0.5f;

                        //auto tex = tex_map["Assets/SimulationBackdrop.png"];
                        //ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
                        //ImVec2 point = ImGui::GetCursorPos();


                        //ImVec2 Min{ point.x, point.y };
                        //ImVec2 Max{ Min.x + viewport->Size.x * 0.91f, Min.y + viewport->Size.y * 0.73f };
                        //ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);

                        //ImGui::BeginChild("##ContentSectionSimulate", ImVec2(Max.x - Min.x, Max.y - Min.y), false, ImGuiWindowFlags_NoScrollWithMouse);

                        //ImGui::Columns(3, "##simRes", false);

                        //ImGui::SetCursorPos({ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.05f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.2f });
                        //ImGui::Image((void*)static_cast<size_t>(tex_map["Assets/Attacker.png"]->GetID()), { tex_map["Assets/Attacker.png"]->GetWidth() * 0.9f, tex_map["Assets/Attacker.png"]->GetHeight() * 0.9f });
                        //auto& atker = instance.ecs.get<tc::Character>(atk);
                        //UI::UIButton_2(atker.name.c_str(), atker.name.c_str(), { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f,  ImGui::GetCursorPosY() - ImGui::GetContentRegionAvail().y * 0.05f }, { 0, 10.f }, FONT_BODY);

                        //ImGui::SetCursorPos({ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.05f,  ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.3f });
                        //ImGui::Image((void*)static_cast<size_t>(tex_map["Assets/Defender.png"]->GetID()), { tex_map["Assets/Defender.png"]->GetWidth() * 0.9f, tex_map["Assets/Defender.png"]->GetHeight() * 0.9f });
                        //auto& defender = instance.ecs.get<tc::Character>(def);
                        //UI::UIButton_2(defender.name.c_str(), defender.name.c_str(), { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f,  ImGui::GetCursorPosY() - ImGui::GetContentRegionAvail().y * 0.05f }, { 0, 10.f }, FONT_BODY);


                        //ImGui::NextColumn();
                        //auto& conf = instance.ecs.get<tc::Graph>(conflict);
                        ////ImGui::SetCursorPos({ 0,0 });
                        //UI::UIButton_2(conf.g.name.c_str(), conf.g.name.c_str(), { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f,  (Max.y - Min.y) * 0.5f }, { 0, 10.f }, FONT_BODY);


                        //ImGui::NextColumn();
                        //const uint32_t one = 1;
                        //ImGui::SetCursorPos({ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.2f , (Max.y - Min.y) * 0.12f });
                        //ImGui::Text("Frequency: ");
                        //ImGui::SameLine();
                        //ImGui::PushItemWidth(150.f);
                        //ImGui::SetCursorPos({ ImGui::GetCursorPosX(), (Max.y - Min.y) * 0.11f });
                        //ImGui::InputScalar("", ImGuiDataType_U32, &freq, 0);
                        //freq = std::clamp(freq, 1u, 50000u);
                        //ImGui::PopItemWidth();
                        //ImGui::PushFont(FONT_HEAD);
                        //if (win + lose != 0)
                        //{
                        //    //ImGui::SetCursorPos({ 0,0 }); //reset curson pos
                        //    ImGui::SetCursorPos({ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.4f , (Max.y - Min.y) * 0.4f });

                        //    ImGui::Text("%.1f %%", 100.f * win / (win + lose));
                        //}
                        //else
                        //{
                        //    auto size = ImGui::CalcTextSize("Press Simulate");
                        //    ImGui::SetCursorPos({ ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvailWidth() - size.x) * 0.5f , (Max.y - Min.y) * 0.4f });
                        //    ImGui::Text("Press Simulate");

                        //}
                        //ImGui::PopFont();
                        //ImGui::EndChild();

                        //if (UI::UIButton_2("Simulate", "Simulate", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.6f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 10.f, 10.f }, FONT_PARA))
                        //{
                        //    freq = std::clamp(freq, 1u, 100000u);

                        //    Service<EventManager>::Get().instant_dispatch<SimulateConflict>(atk, def, conflict, freq, win, lose);
                        //}
                        //if (UI::UIButton_2("Back", "Back", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.75f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 10.f, 10.f }, FONT_PARA))
                        //{
                        //    Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>();
                        //    OverlayOpen = false;
                        //}
                        //if (UI::UIButton_2("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.895f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 10.f, 10.f }, FONT_PARA))
                        //{
                        //    Service<EventManager>::Get().instant_dispatch<OpenConflictResTrigger>();
                        //    OverlayOpen = false;
                        //}
                    }
                  
                }

                ImGui::PopStyleVar();
                ImGui::End();
            }
        }

        void DisplayUnitSection(const ImVec2 start_pos, Entity& unit_type)
        {
            (void)unit_type;
            ImGui::SetCursorPos(start_pos);
            if (UI::UIButton_2("CHARA", "CHARA", ImGui::GetCursorPos(), { 0,0 }, FONT_PARA))
            {
                // open popup here
            }

            // character mame
            const std::string char_name{ "Character Name" };
            ImGui::SetCursorPos({ start_pos.x - (ImGui::CalcTextSize(char_name.c_str()).x + ImGui::GetFontSize()) * 0.5f, start_pos.y + padding });
            ImGui::Text(char_name.c_str());

            ImGui::SetCursorPos({ start_pos.x, start_pos.y + padding * 4.0f });
            if (UI::UIButton_2("SELECT WEAPON", "SELECT WEAPON", ImGui::GetCursorPos(), { 0,0 }, FONT_PARA))
            {

            }
            ImGui::SetCursorPos({ start_pos.x, start_pos.y + padding * 7.0f });
            if (UI::UIButton_2("SELECT ACTION", "SELECT ACTION", ImGui::GetCursorPos(), { 0,0 }, FONT_PARA))
            {

            }
        }

        Entity atk;
        Entity def;
        Entity conflict;
        uint32_t freq = 100000u;

        uint32_t win;
        uint32_t lose;
        float padding{ 0.0f };

        bool OverlayOpen = false;
    };
}