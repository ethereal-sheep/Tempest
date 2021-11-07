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
            case SIMULATE_POPUP_TYPE::UNIT:
                popup_title = "ADDING UNITS";
                img = tex_map["Assets/Charac.png"];
                break;
            case SIMULATE_POPUP_TYPE::WEAPON:
                popup_title = "ADDING WEAPONS";
                img = tex_map["Assets/Sword.png"];
                break;
            case SIMULATE_POPUP_TYPE::ACTION:
                popup_title = "ADDING ACTIONS";
                img = tex_map["Assets/Actions.png"];
                break;
            case SIMULATE_POPUP_TYPE::SEQUENCE:
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
                    
                    if (type != SIMULATE_POPUP_TYPE::EDIT_UNIT && type != SIMULATE_POPUP_TYPE::EDIT_WEAPON)
                    {
                        ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
                        ImGui::PushFont(FONT_OPEN);
                        ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), popup_title.c_str());
                        ImGui::PopFont();

                        ImVec2 imgMin = { winMin.x + ImGui::GetWindowWidth() * 0.15f, winMin.y + ImGui::GetWindowHeight() * 0.1f };
                        ImVec2 imgMax = { imgMin.x + img->GetWidth() * 0.7f, imgMin.y + img->GetHeight() * 0.7f };
                        ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(img->GetID()), imgMin, imgMax);
                    }
                   

                    auto halfToneImg = tex_map["Assets/HalftoneWhite.png"];
                    ImVec2 htMin = { winMin.x, winMin.y + ImGui::GetWindowHeight() * 0.55f };
                    ImVec2 htMax = { htMin.x + halfToneImg->GetWidth(), htMin.y + halfToneImg->GetHeight() };
                    ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(halfToneImg->GetID()), htMin, htMax);

                    unsigned i = 0;
                    unsigned j = 0;
                    ImGui::Dummy(ImVec2{ 0, 22.0f });
                    ImVec2 cursor{ ImGui::GetCursorPosX() + 120.0f, ImGui::GetCursorPosY() + 20.0f};

                    ImGui::BeginChild("##SimualteStuff", ImVec2{ 500,470 });
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
                               Service<EventManager>::Get().instant_dispatch<OpenActionGraphTrigger>(UNDEFINED, instance);
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
                    case SIMULATE_POPUP_TYPE::EDIT_UNIT:
                    {
                        auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
                        Entity StateLineId = UNDEFINED;
                        for (auto id : StatsView)
                            StateLineId = id;
                        auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);

                        auto& cs = instance.ecs.get<tc::Character>(data);
                        float frontPadding = 5.f;
                        ImGui::PushFont(FONT_BODY);
                        ImGui::Dummy({ frontPadding * 15, 0 });
                        ImGui::SameLine();
                        ImGui::Text("Name");
                        ImGui::Dummy({ frontPadding * 15, 0 });
                        ImGui::SameLine();
                        ImGui::PushItemWidth(320.f);
                        ImGui::InputText("##Name", &cs.name);
                        ImGui::PopItemWidth();
                        bool NameDisabled = cs.name.size() > 15;
                        ImGui::SameLine();

                        if (NameDisabled)
                            ImGui::Text("15 Char only");
                        else
                            ImGui::Dummy({ 100.f, 10.f });

                        ImGui::Dummy({ 0, 10.f });
                        bool NextLine = false;
                        ImVec2 PrevPos{ 0.f ,0.f };
                        for (auto counter = 0; counter < sl->size(); counter++)
                        {
                            if ((*sl)(counter))
                            {
                                string stat = sl->operator[](counter) + " :";
                                string label = "##" + stat;

                                if (!NextLine)
                                {
                                    ImGui::Dummy({ frontPadding * 15, 0 });
                                    ImGui::SameLine();
                                    PrevPos = ImGui::GetCursorPos();
                                    ImGui::Text(stat.c_str());
                                    ImGui::Dummy({ frontPadding * 15, 0 });
                                    ImGui::SameLine();
                                    ImGui::PushItemWidth(100.f);
                                    ImGui::InputInt(label.c_str(), &cs.get_stat(counter), 0);
                                    ImGui::PopItemWidth();
                                }

                                else
                                {
                                    ImGui::SetCursorPos(PrevPos);
                                    ImGui::Dummy({ 250.f - frontPadding * 15, 0.f });
                                    ImGui::SameLine();

                                    ImGui::Text(stat.c_str());
                                    ImGui::Dummy({ 250 + frontPadding, 0 });
                                    ImGui::SameLine();
                                    ImGui::PushItemWidth(100.f);
                                    ImGui::InputInt(label.c_str(), &cs.get_stat(counter), 0);
                                    ImGui::PopItemWidth();

                                    ImGui::SetCursorPos(PrevPos);
                                    ImGui::Dummy({ 0, 70.f });
                                }

                                NextLine = !NextLine;

                            }

                        }
                        ImGui::PopFont();
                    }
                        break;
                    case SIMULATE_POPUP_TYPE::EDIT_WEAPON:
                    {
                        auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
                        Entity StateLineId = UNDEFINED;
                        for (auto id : StatsView)
                            StateLineId = id;
                        auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);

                        auto& weap = instance.ecs.get<tc::Weapon>(data);
                        float frontPadding = 5.f;
                        ImGui::PushFont(FONT_BODY);
                        ImGui::Dummy({ frontPadding * 15, 0 });
                        ImGui::SameLine();
                        ImGui::Text("Name");
                        ImGui::Dummy({ frontPadding * 15, 0 });
                        ImGui::SameLine();
                        ImGui::PushItemWidth(320.f);
                        ImGui::InputText("##Name", &weap.name);
                        ImGui::PopItemWidth();
                        bool NameDisabled = weap.name.size() > 15;
                        ImGui::SameLine();

                        if (NameDisabled)
                            ImGui::Text("15 Char only");
                        else
                            ImGui::Dummy({ 100.f, 10.f });

                        ImGui::Dummy({ 0, 10.f });
                        bool NextLine = false;
                        ImVec2 PrevPos{ 0.f ,0.f };
                        for (auto counter = 0; counter < sl->size(); counter++)
                        {
                            if ((*sl)(counter))
                            {
                                string stat = sl->operator[](counter) + " :";
                                string label = "##" + stat;

                                if (!NextLine)
                                {
                                    ImGui::Dummy({ frontPadding * 15, 0 });
                                    ImGui::SameLine();
                                    PrevPos = ImGui::GetCursorPos();
                                    ImGui::Text(stat.c_str());
                                    ImGui::Dummy({ frontPadding * 15, 0 });
                                    ImGui::SameLine();
                                    ImGui::PushItemWidth(100.f);
                                    ImGui::InputInt(label.c_str(), &weap.get_stat(counter), 0);
                                    ImGui::PopItemWidth();
                                }

                                else
                                {
                                    ImGui::SetCursorPos(PrevPos);
                                    ImGui::Dummy({ 250.f - frontPadding * 15, 0.f });
                                    ImGui::SameLine();

                                    ImGui::Text(stat.c_str());
                                    ImGui::Dummy({ 250 + frontPadding, 0 });
                                    ImGui::SameLine();
                                    ImGui::PushItemWidth(100.f);
                                    ImGui::InputInt(label.c_str(), &weap.get_stat(counter), 0);
                                    ImGui::PopItemWidth();

                                    ImGui::SetCursorPos(PrevPos);
                                    ImGui::Dummy({ 0, 70.f });
                                }

                                NextLine = !NextLine;
                            }
                        }
                        ImGui::PopFont();
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