/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
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

    class MainMenuSequencePopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
            Service<EventManager>::Get().register_listener<MainMenuSequencePopupTrigger>(&MainMenuSequencePopup::open_popup, this);

        }

        void open_popup(const Event& e)
        {
            enable_popup = true;
            auto a = event_cast<MainMenuSequencePopupTrigger>(e);
            selected_seqs = &a.selected_seqs;
            img = img = tex_map["Assets/Chain.dds"];
        }

        void show(Instance& instance) override
        {
            if (enable_popup)
            {
                const auto& viewport = ImGui::GetMainViewport();
                ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };
                // what happens if you have multiple popup open, need a way to check
                ImGui::OpenPopup("Sequence Select");
                ImGui::SetNextWindowPos(ImVec2{ viewport->Size.x * 0.8f, viewport->Size.y * 0.55f }, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                ImGui::SetNextWindowSize(ImVec2(500, 550));

                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
                ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
                ImGui::PushStyleColor(ImGuiCol_PopupBg, { 0.06f,0.06f, 0.06f, 0.85f });


                if (ImGui::BeginPopupModal("Sequence Select", NULL, window_flags))
                {
                    // draw the background here
                    ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
                    ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 5.f };
                    ImVec2 winMax = { winMin.x + ImGui::GetWindowWidth() * 0.35f, winMin.y + ImGui::GetWindowHeight() * 0.05f };
                    ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
                    ImVec4 textcol = { 0,0,0,1 };

                    ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
                    ImGui::PushFont(FONT_OPEN);
                    ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), "ADDING SEQUENCE");
                    ImGui::PopFont();

                    ImVec2 imgMin = { winMin.x + ImGui::GetWindowWidth() * 0.15f, winMin.y + ImGui::GetWindowHeight() * 0.1f };
                    ImVec2 imgMax = { imgMin.x + img->GetWidth() * 0.7f, imgMin.y + img->GetHeight() * 0.7f };
                    ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(img->GetID()), imgMin, imgMax);

                    auto halfToneImg = tex_map["Assets/HalftoneWhite.dds"];
                    ImVec2 htMin = { winMin.x, winMin.y + ImGui::GetWindowHeight() * 0.55f };
                    ImVec2 htMax = { htMin.x + halfToneImg->GetWidth(), htMin.y + halfToneImg->GetHeight() };
                    ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(halfToneImg->GetID()), htMin, htMax);

                    ImGui::Dummy(ImVec2{ 0, 22.0f });
                    ImVec2 cursor{ ImGui::GetCursorPosX() + 120.0f, ImGui::GetCursorPosY() + 60.0f };

                    unsigned i = 0;
                    unsigned j = 0;
                    int counter = 0;
                    for (auto id : instance.ecs.view<tc::ConflictGraph>())
                    {
                        auto g = instance.ecs.get_if<tc::Graph>(id);

                        bool selected = std::find(selected_seqs->begin(), selected_seqs->end(), (int)id) != selected_seqs->end();
                        std::string seq_name = g->g.get_name();
                        if (UI::UIButton_2(seq_name.c_str(), seq_name.c_str(), { cursor.x + i++ * 230, cursor.y + j * 100 }, { 5, 20 }, FONT_PARA, selected))
                        {
                            if (selected)
                                selected_seqs->erase(std::remove(selected_seqs->begin(), selected_seqs->end(), counter), selected_seqs->end());
    		                else
                                selected_seqs->emplace_back(id);

                            enable_popup = false;
                            ++counter;
                        }

                        // display in rows of 2
                        if (i / 2)
                        {
                            i = 0;
                            j++;
                        }
                    }
                }

                ImGui::EndPopup();

                ImGui::PopStyleVar(3);
                ImGui::PopStyleColor(2);
            }
            
        }

        bool enable_popup = false;
        tsptr<Texture> img;
        std::vector<int>* selected_seqs{ nullptr };
    };
}