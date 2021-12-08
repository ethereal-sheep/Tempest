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

    class QuickMenuPopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<QuickMenuPopupTrigger>(&QuickMenuPopup::open_popup, this);
            Tabs[QUICKMENU_POPUP_TYPE::SIMULATE] = tex_map["Assets/SimulateMainUnlit.dds"];
            Tabs[QUICKMENU_POPUP_TYPE::UNITS] = tex_map["Assets/UnitsMainUnlit.dds"];
            Tabs[QUICKMENU_POPUP_TYPE::ACTIONS] = tex_map["Assets/ActionsMainUnlit.dds"];
            Tabs[QUICKMENU_POPUP_TYPE::SEQUENCES] = tex_map["Assets/SequenceMainUnlit.dds"];
            Tabs[QUICKMENU_POPUP_TYPE::WEAPONS] = tex_map["Assets/WeaponsMainUnlit.dds"];
            Tabs[QUICKMENU_POPUP_TYPE::ITEMS] = tex_map["Assets/ItemsMainUnlit.dds"];
            button_size = ImVec2{ 1.f, 1.f * Tabs[QUICKMENU_POPUP_TYPE::SIMULATE]->GetHeight() / Tabs[QUICKMENU_POPUP_TYPE::SIMULATE]->GetWidth() };
        }

        void open_popup(const Event& e)
        {
            enable_popup = true;
            auto a = event_cast<QuickMenuPopupTrigger>(e);
            previous = a.current;
            current = a.current;
        }

        void show(Instance& instance) override
        {
            if (enable_popup)
            {
                const auto viewport = ImGui::GetMainViewport();

                ImGui::OpenPopup("Quick Menu");
                ImGui::SetNextWindowPos(ImVec2{ viewport->Size.x * 0.5f, 0.f}, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y * 0.45f));


                button_size = ImVec2{ 1.f, 1.f * Tabs[QUICKMENU_POPUP_TYPE::SIMULATE]->GetHeight() / Tabs[QUICKMENU_POPUP_TYPE::SIMULATE]->GetWidth() };

                button_size.x *= viewport->Size.x / 6 * 0.95f;
                button_size.y *= viewport->Size.x / 6 * 0.95f;

                if (ImGui::BeginPopupModal("Quick Menu", NULL, flags))
                {
                    auto tex = tex_map["Assets/QuickMenuBG.dds"];

                    ImVec2 point{ 0,0 };
                    {
                        ImVec2 Min{ point.x, point.y };
                        ImVec2 Max{ Min.x + viewport->Size.x, Min.y + viewport->Size.y * 0.4f};
                        ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
                    }

                    ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f,viewport->Size.y * 0.25f });

                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });

                    tex = tex_map["Assets/BackMenuBtn.dds"];

                    if (ImGui::ImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }))
                    {
                        // do what here?
                    }

                    ImGui::SameLine();
                    ImGui::Dummy(ImVec2{ 20.0f, 0.0f });
                    ImGui::SameLine();

                    tex = tex_map["Assets/QuickMenuBtn.dds"];

                    if (ImGui::ImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }))
                    {
                        enable_popup = false;
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

                    // render the buttons yo
                    ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f,viewport->Size.y * 0.33f});

                    if (ImGui::ImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::SIMULATE]->GetID()), button_size))
                    {
                        enable_popup = false;
                        AudioEngine ae;
                        ae.Play("Sounds2D/ButtonClick.wav", "sfx_bus");
                        ImGui::CloseCurrentPopup();
                        Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(current);
                        Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>(instance);
                    }


                    if (ImGui::IsItemHovered() || current == QUICKMENU_POPUP_TYPE::SIMULATE)
                        Tabs[QUICKMENU_POPUP_TYPE::SIMULATE] = tex_map["Assets/SimulateMainLit.dds"];
                    else
                        Tabs[QUICKMENU_POPUP_TYPE::SIMULATE] = tex_map["Assets/SimulateMainUnlit.dds"];

                    ImGui::SameLine();

                    if (ImGui::ImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::UNITS]->GetID()), button_size))
                    {
                        enable_popup = false;
                        AudioEngine ae;
                        ae.Play("Sounds2D/ButtonClick.wav", "sfx_bus");
                        ImGui::CloseCurrentPopup();
                        Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(current);
                        Service<EventManager>::Get().instant_dispatch<OpenUnitSheetTrigger>(false, instance, UNDEFINED);
                    }

                    if (ImGui::IsItemHovered() || current == QUICKMENU_POPUP_TYPE::UNITS)
                        Tabs[QUICKMENU_POPUP_TYPE::UNITS] = tex_map["Assets/UnitsMainLit.dds"];
                    else
                        Tabs[QUICKMENU_POPUP_TYPE::UNITS] = tex_map["Assets/UnitsMainUnlit.dds"];

                    ImGui::SameLine();

                    if (ImGui::ImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::ACTIONS]->GetID()), button_size))
                    {
                        enable_popup = false;
                        AudioEngine ae;
                        ae.Play("Sounds2D/ButtonClick.wav", "sfx_bus");
                        ImGui::CloseCurrentPopup();
                        Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(current);
                        Service<EventManager>::Get().instant_dispatch<OpenGraphTrigger>(UNDEFINED, instance, OPEN_GRAPH_TYPE::GRAPH_ACTION);
                    }

                    if (ImGui::IsItemHovered() || current == QUICKMENU_POPUP_TYPE::ACTIONS)
                        Tabs[QUICKMENU_POPUP_TYPE::ACTIONS] = tex_map["Assets/ActionsMainLit.dds"];
                    else
                        Tabs[QUICKMENU_POPUP_TYPE::ACTIONS] = tex_map["Assets/ActionsMainUnlit.dds"];

                    ImGui::SameLine();

                    if (ImGui::ImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::SEQUENCES]->GetID()), button_size))
                    {
                        enable_popup = false;
                        AudioEngine ae;
                        ae.Play("Sounds2D/ButtonClick.wav", "sfx_bus");
                        ImGui::CloseCurrentPopup();
                        Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(current);
                        Service<EventManager>::Get().instant_dispatch<OpenGraphTrigger>(UNDEFINED, instance, OPEN_GRAPH_TYPE::GRAPH_SEQUENCE);
                    }

                    if (ImGui::IsItemHovered() || current == QUICKMENU_POPUP_TYPE::SEQUENCES)
                        Tabs[QUICKMENU_POPUP_TYPE::SEQUENCES] = tex_map["Assets/SequenceMainLit.dds"];
                    else
                        Tabs[QUICKMENU_POPUP_TYPE::SEQUENCES] = tex_map["Assets/SequenceMainUnlit.dds"];

                    ImGui::SameLine();

                    if (ImGui::ImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::WEAPONS]->GetID()), button_size))
                    {
                        enable_popup = false;
                        AudioEngine ae;
                        ae.Play("Sounds2D/ButtonClick.wav", "sfx_bus");
                        ImGui::CloseCurrentPopup();
                        Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(current);
                        Service<EventManager>::Get().instant_dispatch<OpenWeaponSheetTrigger>(false, instance);
                    }

                    if (ImGui::IsItemHovered() || current == QUICKMENU_POPUP_TYPE::WEAPONS)
                        Tabs[QUICKMENU_POPUP_TYPE::WEAPONS] = tex_map["Assets/WeaponsMainLit.dds"];
                    else
                        Tabs[QUICKMENU_POPUP_TYPE::WEAPONS] = tex_map["Assets/WeaponsMainUnlit.dds"];

                    ImGui::SameLine();

                    if (ImGui::ImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::ITEMS]->GetID()), button_size))
                    {
                        AudioEngine ae;
                        ae.Play("Sounds2D/ButtonClick.wav", "sfx_bus");
                    }

                    if (ImGui::IsItemHovered() || current == QUICKMENU_POPUP_TYPE::ITEMS)
                        Tabs[QUICKMENU_POPUP_TYPE::ITEMS] = tex_map["Assets/ItemsMainLit.dds"];
                    else
                        Tabs[QUICKMENU_POPUP_TYPE::ITEMS] = tex_map["Assets/ItemsMainUnlit.dds"];

                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor(3);

                }

                ImGui::EndPopup();
            }
        }

        bool enable_popup = false;
        ImVec2 button_size{ 0.f,0.f };
        std::array<tsptr<Texture>, 6> Tabs;
        QUICKMENU_POPUP_TYPE current{ QUICKMENU_POPUP_TYPE::SIMULATE};
        QUICKMENU_POPUP_TYPE previous{ QUICKMENU_POPUP_TYPE::SIMULATE };
        ImGuiWindowFlags flags{ ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove };
    };
}