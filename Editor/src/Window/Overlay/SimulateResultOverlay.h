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

        void show(Instance&) override
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

                    //auto tex = tex_map["Assets/SimulationBackdrop.png"];
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);


                    ImGui::BeginChild("##DragDropSectionSimulate", ImVec2(contentSize, ImGui::GetContentRegionAvail().y * 0.8f), true);

                    


                    //ImVec2 point = ImGui::GetCursorPos();
                    //{

                    //    ImVec2 Min{ point.x, point.y };
                    //    ImVec2 Max{ Min.x + viewport->Size.x * 0.91f, Min.y + viewport->Size.y * 0.73f };
                    //    
                    //}


                    ImGui::Text("Test");
                    ImGui::Text("Attacker: %u", atk);
                    ImGui::Text("Defender: %u", def);
                    ImGui::Text("Conflict: %u", conflict);
                    const uint32_t one = 1;
                    ImGui::InputScalar("Frequency", ImGuiDataType_U32, &freq, &one);

                    if (win + lose != 0)
                    {
                        ImGui::Text("%.3f", 1.f * win / (win+lose));
                    }
                    else
                    {

                        ImGui::Text("Press Simulate");
                    }

                    ImGui::EndChild();

                    // Cancel button
                    if (UI::UIButton_1("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - 100.0f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 90.f, 0.f }, FONT_PARA))
                    {
                        //OverlayOpen = false;
                        Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("Test");
                    }

                    // Simulate button
                    if (UI::UIButton_1("Simulate", "Simulate", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - 350.0f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 90.f, 0.f }, FONT_PARA))
                    {
                        freq = std::clamp(freq, 1u, 100000u);

                        Service<EventManager>::Get().instant_dispatch<SimulateConflict>(atk, def, conflict, freq, win, lose);

                    }

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