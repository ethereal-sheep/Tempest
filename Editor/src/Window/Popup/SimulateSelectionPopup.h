#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"
namespace Tempest
{

    class SimulateSelectionPopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<SimulatePopupTrigger>(&SimulateSelectionPopup::open_popup, this);
        }

        void open_popup(const Event& e)
        {
            const auto& viewport = ImGui::GetMainViewport();
            enable_popup = true;
            auto a = event_cast<SimulatePopupTrigger>(e);
            type = a.type;
            is_attacker = a.is_attacker;
            data = a.data;
            for_unitpage = a.for_unitpage;
            if (type == SIMULATE_POPUP_TYPE::SEQUENCE || for_unitpage)
            {
                position = ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.5f };
            }

            else
            {
                position = is_attacker ? ImVec2{ viewport->Size.x * 0.2f, viewport->Size.y * 0.5f } :
                                         ImVec2{ viewport->Size.x * 0.8f, viewport->Size.y * 0.5f };
            }

            switch (type)
            {
            case Tempest::UNIT:
                popup_title = "ADDING UNITS";
                img = tex_map["Assets/Charac.png"];
                break;
            case Tempest::WEAPON:
                popup_title = "ADDING WEAPONS";
                img = tex_map["Assets/Sword.png"];
                break;
            case Tempest::ACTION:
                popup_title = "ADDING ACTIONS";
                img = tex_map["Assets/Actions.png"];
                break;
            case Tempest::SEQUENCE:
                popup_title = "ADDING SEQUENCE";
                img = tex_map["Assets/Chain.png"];
                break;
            default:
                break;
            }
           
        }

        void show(Instance& instance) override
        {
            if (enable_popup)
            {
                ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };
                // what happens if you have multiple popup open, need a way to check
                ImGui::OpenPopup("Simulate Select");
                ImGui::SetNextWindowPos(position, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                ImGui::SetNextWindowSize(ImVec2(500, 550));

                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
                ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
                ImGui::PushStyleColor(ImGuiCol_PopupBg, { 0.06f,0.06f, 0.06f, 0.85f });

         
                if (ImGui::BeginPopupModal("Simulate Select", NULL, flags))
                {
                    // draw the background here
                    ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
                    ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 5.f };
                    ImVec2 winMax = { winMin.x + ImGui::GetWindowWidth() * 0.35f, winMin.y + ImGui::GetWindowHeight() * 0.05f };
                    ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
                    ImVec4 textcol = { 0,0,0,1 };
                    if (ImGui::IsWindowFocused() == false)
                    {
                        col = { 0.980f, 0.768f, 0.509f, 0.7f };
                        textcol = { 0,0,0,0.7 };
                    }

                    ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
                    ImGui::PushFont(FONT_OPEN);
                    ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), popup_title.c_str());
                    ImGui::PopFont();

                    auto halfToneImg = tex_map["Assets/HalftoneWhite.png"];
                    ImVec2 imgMin = { winMin.x + ImGui::GetWindowWidth() * 0.15f, winMin.y + ImGui::GetWindowHeight() * 0.1f };
                    ImVec2 imgMax = { imgMin.x + img->GetWidth() * 0.7f, imgMin.y + img->GetHeight() * 0.7f };
                    ImVec2 htMin = { winMin.x, winMin.y + ImGui::GetWindowHeight() * 0.55f };
                    ImVec2 htMax = { htMin.x + halfToneImg->GetWidth(), htMin.y + halfToneImg->GetHeight() };
                    ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(img->GetID()), imgMin, imgMax);
                    ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(halfToneImg->GetID()), htMin, htMax);

                    unsigned i = 0;
                    unsigned j = 0;
                    ImGui::Dummy(ImVec2{ 0, 22.0f });
                    ImVec2 cursor{ ImGui::GetCursorPosX() + 120.0f, ImGui::GetCursorPosY() + 20.0f};

                    ImGui::BeginChild("##SimualteStuff", ImVec2{ 500,480 });
                    switch (type)
                    {
                    case SIMULATE_POPUP_TYPE::UNIT:
                    {
                        auto view = instance.ecs.view<Components::Character>(exclude_t<tc::Destroyed>());
                        cursor.x -= 30.0f;
                        cursor.y += 20.0f;

                        for (auto id : view)
                        {
                            auto& charac = instance.ecs.get<tc::Character>(id);
                            auto charc_icon = tex_map["Assets/CharacterIcon.png"];
                            ImGui::SetCursorPos(ImVec2{ cursor.x + i++ * 120, cursor.y + j * 140 });
                            if (UI::UICharButton_NoDelete((void*)static_cast<size_t>(charc_icon->GetID()), { 90,90 }, charac.name.c_str(), "##" + std::to_string(id), data == id))
                            {
                                data = id;
                            }

                            // display in rows of 3
                            if (i / 3)
                            {
                                i = 0;
                                j++;
                            }
                        }
                        
                        // create units here
                        if (UI::UIButton_1("+", "+", ImVec2{ ImGui::GetCursorPosX() + (i + 1) * 120.0f, cursor.y + (!j ? 60.0f : j * 190.0f) }, { 55,30 }, FONT_HEAD))
                        {
                            // TODO: take out id/data
                            enable_popup = false;
                            Service<EventManager>::Get().instant_dispatch<OpenUnitSheetTrigger>(false, instance, UNDEFINED);
                        }
                    }
                        break;
                    case SIMULATE_POPUP_TYPE::WEAPON:
                    {
                        // for testing here
                        auto view = instance.ecs.view<Components::Weapon>(exclude_t<tc::Destroyed>());

                        for (auto id : view)
                        {
                            auto& weapon = instance.ecs.get<tc::Weapon>(id);
                            if (UI::UIButton_1(weapon.name.c_str(), weapon.name.c_str(), { cursor.x + i++ * 200, cursor.y + j * 100 }, { 100, 10 }, FONT_PARA, data == id))
                            {
                                data = id;
                            }

                            // display in rows of 2
                            if (i / 2)
                            {
                                i = 0;
                                j++;
                            }
                        }

                        if (UI::UIButton_1("+", "+", { cursor.x + i++ * 200, cursor.y + j * 100 }, { 140,-10 }, FONT_HEAD))
                        {
                            enable_popup = false;
                            Service<EventManager>::Get().instant_dispatch<OpenWeaponSheetTrigger>(false, instance);
                        }
                    }
                        break;
                    case SIMULATE_POPUP_TYPE::ACTION:
                    {
                        for (auto id : instance.ecs.view<tc::ActionGraph>())
                        {
                            auto& action = instance.ecs.get<tc::Graph>(id);

                            if (UI::UIButton_1(action.g.name + ": " + std::to_string(i), action.g.name + ": " + std::to_string(i), { cursor.x + i++ * 230, cursor.y + j * 100 }, { 120, 20 }, FONT_PARA, data == id))
                            {
                                data = id;
                            }

                            // display in rows of 2
                            if (i / 2)
                            {
                                i = 0;
                                j++;
                            }
                              
                        }

                        if (UI::UIButton_1("+", "+", { cursor.x + i++ * 230, cursor.y + j * 100 }, { 140,-10 }, FONT_HEAD))
                        {
                            enable_popup = false;
                            // goto actions page and create additional actions
                            //   Service<EventManager>::Get().instant_dispatch<OpenUnitSheetTrigger>(true, instance, INVALID);
                        }
                    }
                        break;
                    case SIMULATE_POPUP_TYPE::SEQUENCE:
                    {
                        for (auto id : instance.ecs.view<tc::ConflictGraph>())
                        {
                            auto& action = instance.ecs.get<tc::Graph>(id);

                            if (UI::UIButton_1(action.g.name + ": " + std::to_string(i), action.g.name + ": " + std::to_string(i), { cursor.x + i++ * 230, cursor.y + j * 100 }, { 120, 20 }, FONT_PARA, data == id))
                            {
                                data = id;
                            }

                            // display in rows of 2
                            if (i / 2)
                            {
                                i = 0;
                                j++;
                            }

                        }

                        if (UI::UIButton_1("+", "+", { cursor.x + i++ * 230, cursor.y + j * 100 }, { 140,-10 }, FONT_HEAD))
                        {
                            enable_popup = false;
                            // goto sequence page and create additional sequence
                            //   Service<EventManager>::Get().instant_dispatch<OpenUnitSheetTrigger>(true, instance, INVALID);
                        }
                    }
                        break;
                    default:
                        break;
                    }

                    ImGui::EndChild();

                    if (UI::UIButton_2("Confirm", "Confirm", { ImGui::GetContentRegionAvailWidth() * 0.75f, ImGui::GetCursorPosY() }, { 0.f, 0.f }, FONT_PARA))
                    {
                        ImGui::CloseCurrentPopup();
                        enable_popup = false;

                        Service<EventManager>::Get().instant_dispatch<SimulateSelectionConfirm>(type, is_attacker, data, for_unitpage);
                    }
                   
                }

                ImGui::EndPopup();

                ImGui::PopStyleVar(3);
                ImGui::PopStyleColor(2);
            }
        }

        Entity data;
        bool enable_popup = false;
        bool is_attacker = false;
        bool for_unitpage = false;
        std::string popup_title{ "" };
        tsptr<Texture> img;
        ImVec2 position{ 0,0 };
        SIMULATE_POPUP_TYPE type{ SIMULATE_POPUP_TYPE::UNIT};
        ImGuiWindowFlags flags{ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                               ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove };
    };
}