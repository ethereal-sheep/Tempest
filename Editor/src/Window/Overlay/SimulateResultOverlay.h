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
        }

        void show(Instance& instance) override
        {

            const ImGuiViewport* viewport = ImGui::GetMainViewport();

            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);

            if (OverlayOpen)
            {
                if (ImGui::Begin("SimResult", nullptr, window_flags))
                {

                    //float center_x = ImGui::GetContentRegionAvailWidth() * 0.5f;
                    const float contentSize = ImGui::GetContentRegionAvailWidth() * 0.9f;
                    string text = "Simulate";
                    //float text_center = center_x - (ImGui::CalcTextSize(text.c_str()).x * 0.5f);

                    ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
                    UI::SubHeader("Simulate");
                    ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });



                    
                    const float offsetX = (ImGui::GetContentRegionAvailWidth() - contentSize) * 0.5f;

                    auto tex = tex_map["Assets/SimulationBackdrop.png"];
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
                    ImVec2 point = ImGui::GetCursorPos();
                    

                    ImVec2 Min{ point.x, point.y };
                    ImVec2 Max{ Min.x + viewport->Size.x * 0.91f, Min.y + viewport->Size.y * 0.73f };
                    ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
                    
                    ImGui::BeginChild("##ContentSectionSimulate", ImVec2(Max.x - Min.x, Max.y - Min.y), true, ImGuiWindowFlags_NoScrollWithMouse);

                    ImGui::Columns(3, "##simRes");

                    ImGui::SetCursorPos({ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.05f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.2f });
                    ImGui::Image((void*)static_cast<size_t>(tex_map["Assets/Attacker.png"]->GetID()), { tex_map["Assets/Attacker.png"]->GetWidth() * 0.9f, tex_map["Assets/Attacker.png"]->GetHeight() * 0.9f });
                    auto& atker = instance.ecs.get<tc::Character>(atk);
                    UI::UIButton_2(atker.name.c_str(), atker.name.c_str(), {ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f,  ImGui::GetCursorPosY() - ImGui::GetContentRegionAvail().y * 0.05f }, { 0, 10.f }, FONT_BODY);

                    ImGui::SetCursorPos({ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.05f,  ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.3f });
                    ImGui::Image((void*)static_cast<size_t>(tex_map["Assets/Defender.png"]->GetID()), { tex_map["Assets/Defender.png"]->GetWidth() * 0.9f, tex_map["Assets/Defender.png"]->GetHeight() * 0.9f });
                    auto& defender = instance.ecs.get<tc::Character>(def);
                    UI::UIButton_2(defender.name.c_str(), defender.name.c_str(), { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f,  ImGui::GetCursorPosY() - ImGui::GetContentRegionAvail().y * 0.05f }, { 0, 10.f }, FONT_BODY);
                    
                    
                    ImGui::NextColumn();
                    auto& conf = instance.ecs.get<tc::Graph>(conflict);
                    //ImGui::SetCursorPos({ 0,0 });
                    UI::UIButton_2(conf.g.name.c_str(), conf.g.name.c_str(), { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f,  (Max.y - Min.y)*0.5f }, { 0, 10.f }, FONT_BODY);
                    
                    
                    ImGui::NextColumn();
                    const uint32_t one = 1;
                    ImGui::SetCursorPos({ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.2f , (Max.y - Min.y) * 0.12f });
                    ImGui::Text("Frequency: ");
                    ImGui::SameLine();
                    ImGui::PushItemWidth(150.f);
                    ImGui::SetCursorPos({ ImGui::GetCursorPosX(), (Max.y - Min.y) * 0.11f });
                    ImGui::InputScalar("", ImGuiDataType_U32, &freq, 0);
                    freq = std::clamp(freq, 1u, 50000u);
                    ImGui::PopItemWidth();
                    ImGui::PushFont(FONT_HEAD);
                    if (win + lose != 0)
                    {
                        //ImGui::SetCursorPos({ 0,0 }); //reset curson pos
                        ImGui::SetCursorPos({ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.4f , (Max.y - Min.y) * 0.4f });
                        
                        ImGui::Text("%.1f %%", 100.f * win / (win+lose));
                        
                        
                    }
                    else
                    {
                        auto size = ImGui::CalcTextSize("Press Simulate");
                        ImGui::SetCursorPos({ ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvailWidth() - size.x) * 0.5f , (Max.y - Min.y) * 0.4f });
                        ImGui::Text("Press Simulate");
                        
                    }
                    ImGui::PopFont();
                    ImGui::EndChild();

                    if (UI::UIButton_2("Simulate", "Simulate", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.6f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 10.f, 10.f }, FONT_PARA))
                    {
                        freq = std::clamp(freq, 1u, 100000u);

                        Service<EventManager>::Get().instant_dispatch<SimulateConflict>(atk, def, conflict, freq, win, lose);
                    }
                    if (UI::UIButton_2("Back", "Back", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.75f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 10.f, 10.f }, FONT_PARA))
                    {
                        Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>();
                        OverlayOpen = false;
                    }
                    if (UI::UIButton_2("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.895f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 10.f, 10.f }, FONT_PARA))
                    {
                        Service<EventManager>::Get().instant_dispatch<OpenConflictResTrigger>();
                        OverlayOpen = false;
                    }
                    //ImGui::Text("Test");
                    //ImGui::Text("Attacker: %u", atk);
                    //ImGui::Text("Defender: %u", def);
                    //ImGui::Text("Conflict: %u", conflict);
                    //const uint32_t one = 1;
                    //ImGui::InputScalar("Frequency", ImGuiDataType_U32, &freq, &one);

                    //if (win + lose != 0)
                    //{
                    //    ImGui::Text("%.3f", 1.f * win / (win+lose));
                    //}
                    //else
                    //{

                    //    ImGui::Text("Press Simulate");
                    //}

                    //// Cancel button
                    //if (UI::UIButton_1("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - 100.0f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 90.f, 0.f }, FONT_PARA))
                    //{
                    //    OverlayOpen = false;
                    //    //Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("Test");
                    //}

                    //// Simulate button
                    //if (UI::UIButton_1("Simulate", "Simulate", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - 350.0f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 90.f, 0.f }, FONT_PARA))
                    //{
                    //    freq = std::clamp(freq, 1u, 100000u);

                    //    Service<EventManager>::Get().instant_dispatch<SimulateConflict>(atk, def, conflict, freq, win, lose);

                    //}

                }
                ImGui::End();
            }
        }

        Entity atk;
        Entity def;
        Entity conflict;
        uint32_t freq = 100000u;

        uint32_t win;
        uint32_t lose;


        bool OverlayOpen = false;
    };
}