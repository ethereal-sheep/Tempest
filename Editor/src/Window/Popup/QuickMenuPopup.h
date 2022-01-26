/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
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
#include "Util/interpolater.h"

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
            if (state == State::CLOSED)
            {
                auto a = event_cast<QuickMenuPopupTrigger>(e);
                previous = a.current;
                current = a.current;

                state = State::START;
            }
        }

        bool draw_menu(Instance& instance, float y, bool still_working = true)
        {
            const auto viewport = ImGui::GetMainViewport();
            button_size = ImVec2{ 1.f, 1.f * Tabs[QUICKMENU_POPUP_TYPE::SIMULATE]->GetHeight() / Tabs[QUICKMENU_POPUP_TYPE::SIMULATE]->GetWidth() };

            button_size.x *= viewport->Size.x / 6 * 0.95f;
            button_size.y *= viewport->Size.x / 6 * 0.95f;

            bool return_v = false;

            ImGui::SetNextWindowPos(ImVec2{ viewport->Size.x * 0.5f, y }, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowFocus();
            if (ImGui::BeginPopupModal("Quick Menu", NULL, flags))
            {
                auto tex = tex_map["Assets/QuickMenuBG.dds"];

                ImVec2 point{ 0,0 };
                {
                    ImVec2 Min{ point.x, y };
                    ImVec2 Max{ Min.x + viewport->Size.x, Min.y + viewport->Size.y * 0.4f + y };
                    ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
                }

                ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f,viewport->Size.y * 0.25f + y });

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });

                ImGui::Dummy(ImVec2{ 150.0f, 0.0f });
                ImGui::SameLine();

                tex = tex_map["Assets/QuickMenuBtn_Toggled.dds"];

                if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }) && still_working)
                {
                    return_v = true;
                }

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

                // render the buttons yo
                ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f,viewport->Size.y * 0.33f + y });

                if (UI::UIImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::SIMULATE]->GetID()), button_size) && still_working)
                {
                    if (current != QUICKMENU_POPUP_TYPE::SIMULATE)
                    {
                        AudioEngine ae;
                        ae.Play("Sounds2D/ButtonClick.wav", "SFX");
                        Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(current);
                        Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>(instance);
                    }
                    return_v = true;
                }


                if (ImGui::IsItemHovered() || current == QUICKMENU_POPUP_TYPE::SIMULATE)
                    Tabs[QUICKMENU_POPUP_TYPE::SIMULATE] = tex_map["Assets/SimulateMainLit.dds"];
                else
                    Tabs[QUICKMENU_POPUP_TYPE::SIMULATE] = tex_map["Assets/SimulateMainUnlit.dds"];

                ImGui::SameLine();

                if (UI::UIImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::UNITS]->GetID()), button_size) && still_working)
                {
                    if (current != QUICKMENU_POPUP_TYPE::UNITS)
                    {
                        AudioEngine ae;
                        ae.Play("Sounds2D/ButtonClick.wav", "SFX");
                        Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(current);
                        Service<EventManager>::Get().instant_dispatch<OpenUnitSheetTrigger>(false, instance, UNDEFINED);
                    }
                    return_v = true;
                }

                if (ImGui::IsItemHovered() || current == QUICKMENU_POPUP_TYPE::UNITS)
                    Tabs[QUICKMENU_POPUP_TYPE::UNITS] = tex_map["Assets/UnitsMainLit.dds"];
                else
                    Tabs[QUICKMENU_POPUP_TYPE::UNITS] = tex_map["Assets/UnitsMainUnlit.dds"];

                ImGui::SameLine();

                if (UI::UIImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::ACTIONS]->GetID()), button_size) && still_working)
                {
                    if (current != QUICKMENU_POPUP_TYPE::ACTIONS)
                    {
                        AudioEngine ae;
                        ae.Play("Sounds2D/ButtonClick.wav", "SFX");
                        Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(current);
                        Service<EventManager>::Get().instant_dispatch<OpenGraphTrigger>(UNDEFINED, instance, OPEN_GRAPH_TYPE::GRAPH_ACTION);
                    }
                    return_v = true;
                }

                if (ImGui::IsItemHovered() || current == QUICKMENU_POPUP_TYPE::ACTIONS)
                    Tabs[QUICKMENU_POPUP_TYPE::ACTIONS] = tex_map["Assets/ActionsMainLit.dds"];
                else
                    Tabs[QUICKMENU_POPUP_TYPE::ACTIONS] = tex_map["Assets/ActionsMainUnlit.dds"];

                ImGui::SameLine();

                if (UI::UIImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::SEQUENCES]->GetID()), button_size) && still_working)
                {
                    if (current != QUICKMENU_POPUP_TYPE::SEQUENCES)
                    {
                        AudioEngine ae;
                        ae.Play("Sounds2D/ButtonClick.wav", "SFX");
                        Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(current);
                        Service<EventManager>::Get().instant_dispatch<OpenGraphTrigger>(UNDEFINED, instance, OPEN_GRAPH_TYPE::GRAPH_SEQUENCE);
                    }
                    return_v = true;
                }

                if (ImGui::IsItemHovered() || current == QUICKMENU_POPUP_TYPE::SEQUENCES)
                    Tabs[QUICKMENU_POPUP_TYPE::SEQUENCES] = tex_map["Assets/SequenceMainLit.dds"];
                else
                    Tabs[QUICKMENU_POPUP_TYPE::SEQUENCES] = tex_map["Assets/SequenceMainUnlit.dds"];

                ImGui::SameLine();

                if (UI::UIImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::WEAPONS]->GetID()), button_size) && still_working)
                {
                    if (current != QUICKMENU_POPUP_TYPE::WEAPONS)
                    {
                        AudioEngine ae;
                        ae.Play("Sounds2D/ButtonClick.wav", "SFX");
                        Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(current);
                        Service<EventManager>::Get().instant_dispatch<OpenWeaponSheetTrigger>(false, instance);
                    }
                    return_v = true;
                }

                if (ImGui::IsItemHovered() || current == QUICKMENU_POPUP_TYPE::WEAPONS)
                    Tabs[QUICKMENU_POPUP_TYPE::WEAPONS] = tex_map["Assets/WeaponsMainLit.dds"];
                else
                    Tabs[QUICKMENU_POPUP_TYPE::WEAPONS] = tex_map["Assets/WeaponsMainUnlit.dds"];

                ImGui::SameLine();

                if (UI::UIImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::ITEMS]->GetID()), button_size) && still_working)
                {
                    /*AudioEngine ae;
                    ae.Play("Sounds2D/ButtonClick.wav", "SFX");*/
                }

                /*if (ImGui::IsItemHovered() || current == QUICKMENU_POPUP_TYPE::ITEMS)
                    Tabs[QUICKMENU_POPUP_TYPE::ITEMS] = tex_map["Assets/ItemsMainLit.dds"];
                else*/
                    Tabs[QUICKMENU_POPUP_TYPE::ITEMS] = tex_map["Assets/ItemsMainUnlit.dds"];

                ImGui::PopStyleVar();
                ImGui::PopStyleColor(3);

                ImGui::EndPopup();
            }

            return return_v;
        }


        void show(Instance& instance) override
        {
            const auto viewport = ImGui::GetMainViewport();
            switch (state)
            {
            case Tempest::QuickMenuPopup::State::CLOSED:
                break;
            case Tempest::QuickMenuPopup::State::START:
            {
                ImGui::OpenPopup("Quick Menu");
                ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y * 0.45f));
                inter.start(-(viewport->Size.y * .22f), 0, .2f, 0.f, [](float x) { return glm::sineEaseOut(x); });

                state = State::OPENING;
            }
                [[fallthrough]];
            case Tempest::QuickMenuPopup::State::OPENING:
                inter.update(ImGui::GetIO().DeltaTime);
                draw_menu(instance, inter.get());
                if (inter.is_finished())
                    state = State::OPEN;
                break;
            case Tempest::QuickMenuPopup::State::OPEN:
                if (draw_menu(instance, 0))
                {
                    inter.start(0, -(viewport->Size.y * .22f), .2f, 0.f, [](float x) { return glm::sineEaseIn(x); });
                    state = State::CLOSING;
                }
                break;
            case Tempest::QuickMenuPopup::State::CLOSING:
                inter.update(ImGui::GetIO().DeltaTime);
                ImGui::SetNextWindowBgAlpha(0);
                draw_menu(instance, inter.get(), false);

                if (inter.is_finished())
                {
                    state = State::CLOSED;
                    ImGui::CloseCurrentPopup();
                }
                break;
            default:
                break;
            }
        }

        ImVec2 button_size{ 0.f,0.f };
        std::array<tsptr<Texture>, 6> Tabs;
        QUICKMENU_POPUP_TYPE current{ QUICKMENU_POPUP_TYPE::SIMULATE};
        QUICKMENU_POPUP_TYPE previous{ QUICKMENU_POPUP_TYPE::SIMULATE };
        ImGuiWindowFlags flags{ ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove };
        ImVec4 btnTintHover = { 0.922f,0.922f,0.922f,1.f };
        ImVec4 btnTintPressed = { 0.768f, 0.768f, 0.768f, 1.f };

        enum struct State 
        {
            CLOSED,
            START,
            OPENING,
            OPEN,
            CLOSING
        };

        State state = State::CLOSED;

        interpolater<float> inter;
    };
}