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
            Tabs[QUICKMENU_POPUP_TYPE::SIMULATE] = tex_map["Assets/SimulateMainUnlit.png"];
            Tabs[QUICKMENU_POPUP_TYPE::UNITS] = tex_map["Assets/UnitsMainUnlit.png"];
            Tabs[QUICKMENU_POPUP_TYPE::ACTIONS] = tex_map["Assets/ActionsMainUnlit.png"];
            Tabs[QUICKMENU_POPUP_TYPE::SEQUENCES] = tex_map["Assets/SequenceMainUnlit.png"];
            Tabs[QUICKMENU_POPUP_TYPE::WEAPONS] = tex_map["Assets/WeaponsMainUnlit.png"];
            Tabs[QUICKMENU_POPUP_TYPE::ITEMS] = tex_map["Assets/ItemsMainUnlit.png"];
            button_size = ImVec2{ Tabs[QUICKMENU_POPUP_TYPE::SIMULATE]->GetWidth() * 0.75f, Tabs[QUICKMENU_POPUP_TYPE::SIMULATE]->GetHeight() * 0.75f};
        }

        void open_popup(const Event& e)
        {
            enable_popup = true;
            auto a = event_cast<QuickMenuPopupTrigger>(e);
            current = a.current;
        }

        void show(Instance&) override
        {
            if (enable_popup)
            {
                const auto viewport = ImGui::GetMainViewport();

                ImGui::OpenPopup("Quick Menu");
                ImGui::SetNextWindowPos(ImVec2{ viewport->Size.x * 0.5f, 0.f}, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y * 0.45f));

                if (ImGui::BeginPopupModal("Quick Menu", NULL, flags))
                {
                    auto tex = tex_map["Assets/QuickMenuBG.png"];

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

                    tex = tex_map["Assets/BackMenuBtn.png"];

                    if (ImGui::ImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }))
                    {
                        // do what here?
                    }

                    ImGui::SameLine();
                    ImGui::Dummy(ImVec2{ 20.0f, 0.0f });
                    ImGui::SameLine();

                    tex = tex_map["Assets/QuickMenuBtn.png"];

                    if (ImGui::ImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }))
                    {
                        enable_popup = false;
                        ImGui::CloseCurrentPopup();
                    }

                    // render the buttons yo
                    ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f,viewport->Size.y * 0.33f});

                    if (ImGui::ImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::SIMULATE]->GetID()), button_size))
                    {
                        current = QUICKMENU_POPUP_TYPE::SIMULATE;
                    }

                    if (ImGui::IsItemHovered())
                        Tabs[QUICKMENU_POPUP_TYPE::SIMULATE] = tex_map["Assets/SimulateMainLit.png"];
                    else
                        Tabs[QUICKMENU_POPUP_TYPE::SIMULATE] = tex_map["Assets/SimulateMainUnlit.png"];

                    ImGui::SameLine();

                    if (ImGui::ImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::UNITS]->GetID()), button_size))
                    {
                        current = QUICKMENU_POPUP_TYPE::UNITS;
                    }

                    if (ImGui::IsItemHovered())
                        Tabs[QUICKMENU_POPUP_TYPE::UNITS] = tex_map["Assets/UnitsMainLit.png"];
                    else
                        Tabs[QUICKMENU_POPUP_TYPE::UNITS] = tex_map["Assets/UnitsMainUnlit.png"];

                    ImGui::SameLine();

                    if (ImGui::ImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::ACTIONS]->GetID()), button_size))
                    {
                        current = QUICKMENU_POPUP_TYPE::ACTIONS;
                    }

                    if (ImGui::IsItemHovered())
                        Tabs[QUICKMENU_POPUP_TYPE::ACTIONS] = tex_map["Assets/ActionsMainLit.png"];
                    else
                        Tabs[QUICKMENU_POPUP_TYPE::ACTIONS] = tex_map["Assets/ActionsMainUnlit.png"];

                    ImGui::SameLine();

                    if (ImGui::ImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::SEQUENCES]->GetID()), button_size))
                    {
                        current = QUICKMENU_POPUP_TYPE::SEQUENCES;
                    }

                    if (ImGui::IsItemHovered())
                        Tabs[QUICKMENU_POPUP_TYPE::SEQUENCES] = tex_map["Assets/SequenceMainLit.png"];
                    else
                        Tabs[QUICKMENU_POPUP_TYPE::SEQUENCES] = tex_map["Assets/SequenceMainUnlit.png"];

                    ImGui::SameLine();

                    if (ImGui::ImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::WEAPONS]->GetID()), button_size))
                    {
                        current = QUICKMENU_POPUP_TYPE::WEAPONS;
                    }

                    if (ImGui::IsItemHovered())
                        Tabs[QUICKMENU_POPUP_TYPE::WEAPONS] = tex_map["Assets/WeaponsMainLit.png"];
                    else
                        Tabs[QUICKMENU_POPUP_TYPE::WEAPONS] = tex_map["Assets/WeaponsMainUnlit.png"];

                    ImGui::SameLine();

                    if (ImGui::ImageButton((void*)static_cast<size_t>(Tabs[QUICKMENU_POPUP_TYPE::ITEMS]->GetID()), button_size))
                    {
                        current = QUICKMENU_POPUP_TYPE::ITEMS;
                    }

                    if (ImGui::IsItemHovered())
                        Tabs[QUICKMENU_POPUP_TYPE::ITEMS] = tex_map["Assets/ItemsMainLit.png"];
                    else
                        Tabs[QUICKMENU_POPUP_TYPE::ITEMS] = tex_map["Assets/ItemsMainUnlit.png"];

                    ImGui::PopStyleColor(3);

                }

                ImGui::EndPopup();
            }
        }

        bool enable_popup = false;
        ImVec2 button_size{ 0.f,0.f };
        std::array<tsptr<Texture>, 6> Tabs;
        QUICKMENU_POPUP_TYPE current;
        ImGuiWindowFlags flags{ ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove };
    };
}