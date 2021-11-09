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
                    
                    ImGui::BeginChild("##ContentSectionSimulate", ImVec2(Max.x - Min.x, Max.y - Min.y), false, ImGuiWindowFlags_NoScrollWithMouse);

                    ImGui::Columns(3, "##simRes", false);

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
                    freq = std::clamp(freq, 1u, 500000u);
                    ImGui::PopItemWidth();
                    ImGui::PushFont(FONT_HEAD);
                    if (win+lose != 0)
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
                        Service<EventManager>::Get().instant_dispatch<SimulateConflict>(atk, def, atk_act, def_act, conflict, freq, win, lose, attack, defend, finish);
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
                }
                ImGui::End();

                if (ImGui::Begin("Test"))
                {
                    ImGui::InputScalar("Conflict", ImGuiDataType_U32, &conflict);

                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SelectConflict"))
                        {
                            IM_ASSERT(payload->DataSize == sizeof(Entity));
                            conflict = *static_cast<Entity*>(payload->Data);
                        }
                    }

                    UI::PaddedSeparator(0.5f);


                    ImGui::InputScalar("Attacking", ImGuiDataType_U32, &atk);

                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SelectUnit"))
                        {
                            IM_ASSERT(payload->DataSize == sizeof(Entity));
                            atk = *static_cast<Entity*>(payload->Data);
                        }
                    }
                    ImGui::InputScalar("Attack Action", ImGuiDataType_U32, &atk_act);

                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SelectAction"))
                        {
                            IM_ASSERT(payload->DataSize == sizeof(Entity));
                            atk_act = *static_cast<Entity*>(payload->Data);
                        }
                    }

                    UI::PaddedSeparator(0.5f);

                    ImGui::InputScalar("Defending", ImGuiDataType_U32, &def);
                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SelectUnit"))
                        {
                            IM_ASSERT(payload->DataSize == sizeof(Entity));
                            def = *static_cast<Entity*>(payload->Data);
                        }
                    }

                    ImGui::InputScalar("Defend Action", ImGuiDataType_U32, &def_act);
                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SelectAction"))
                        {
                            IM_ASSERT(payload->DataSize == sizeof(Entity));
                            def_act = *static_cast<Entity*>(payload->Data);
                        }
                    }

                    UI::PaddedSeparator(1.f);


                    if (ImGui::TreeNodeEx("Conflict Graphs"))
                    {
                        for (auto id : instance.ecs.view<tc::ConflictGraph>())
                        {
                            std::string text = std::to_string(id);
                            ImGui::Indent(10.f);
                            if (ImGui::Selectable(text.c_str(), conflict == id))
                            {
                                conflict = id;
                            }
                            if (ImGui::BeginDragDropSource())
                            {

                                static Entity payload = 0;
                                payload = id;
                                ImGui::SetDragDropPayload("SelectConflict", &payload, sizeof(Entity));

                                ImGui::Text("%u", id);
                                ImGui::EndDragDropSource();
                            }
                            ImGui::Unindent(10.f);
                        }
                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNodeEx("Action Graphs"))
                    {
                        for (auto id : instance.ecs.view<tc::ActionGraph>())
                        {
                            std::string text = std::to_string(id);
                            ImGui::Indent(10.f);
                            ImGui::Selectable(text.c_str());
                            if (ImGui::BeginDragDropSource())
                            {
                                static Entity payload = 0;
                                payload = id;
                                ImGui::SetDragDropPayload("SelectAction", &payload, sizeof(Entity));

                                ImGui::Text("%u", id);
                                ImGui::EndDragDropSource();
                            }
                            ImGui::Unindent(10.f);
                        }
                        ImGui::TreePop();
                    }

                    UI::PaddedSeparator(0.5f);

                    if (ImGui::TreeNodeEx("Units"))
                    {
                        for (auto id : instance.ecs.view<tc::Character>())
                        {
                            std::string text = std::to_string(id);
                            ImGui::Indent(10.f);
                            ImGui::Selectable(text.c_str());
                            if (ImGui::BeginDragDropSource())
                            {
                                static Entity payload = 0;
                                payload = id;
                                ImGui::SetDragDropPayload("SelectUnit", &payload, sizeof(Entity));

                                ImGui::Text("%u", id);
                                ImGui::EndDragDropSource();
                            }
                            ImGui::Unindent(10.f);
                        }
                        ImGui::TreePop();
                    }

                }
                ImGui::End();
            }
        }

        Entity atk;
        Entity def;
        Entity atk_act;
        Entity def_act;
        Entity conflict;
        uint32_t freq = 100000u;

        std::atomic_uint32_t win;
        std::atomic_uint32_t lose;
        std::atomic_int32_t attack;
        std::atomic_int32_t defend;
        std::atomic_bool finish;


        bool OverlayOpen = false;
    };
}