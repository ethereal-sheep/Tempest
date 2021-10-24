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
            position = is_attacker ? ImVec2{ viewport->Size.x * 0.2f, viewport->Size.y * 0.5f } :
                                     ImVec2{ viewport->Size.x * 0.8f, viewport->Size.y * 0.5f };
        }

        void show(Instance& instance) override
        {
            if (enable_popup)
            {
                // what happens if you have multiple popup open, need a way to check
                ImGui::OpenPopup("Simulate Select");
                ImGui::SetNextWindowPos(position, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                ImGui::SetNextWindowSize(ImVec2(500, 500));

                if (ImGui::BeginPopupModal("Simulate Select", NULL, flags))
                {
                    unsigned i = 0;
                    unsigned j = 0;

                    switch (type)
                    {
                    case SIMULATE_POPUP_TYPE::UNIT:
                    {
                        auto view = instance.ecs.view<Components::Character>(exclude_t<tc::Destroyed>());

                       
                        // TODO: store selected item
                        const ImVec2 cursor{ ImGui::GetCursorPosX() + 100, ImGui::GetCursorPosY() + 60 };
                        for (auto id : view)
                        {
                            auto& charac = instance.ecs.get<tc::Character>(id);
                            if (UI::UIButton_1(charac.name.c_str(), charac.name.c_str(), { cursor.x + i++ * 120, cursor.y + j * 100 }, { 50,50 }, FONT_PARA))
                            {
                                data = id;
                            }

                            // display in rows of 3
                            if (i / 3)
                                j++;
                        }

                        // create units here
                        if (UI::UIButton_1("+", "+", { cursor.x + i++ * 120, cursor.y + j * 100 }, { 55,30 }, FONT_HEAD))
                        {
                            // TODO: take out id/data
                            enable_popup = false;
                            Service<EventManager>::Get().instant_dispatch<OpenUnitSheetTrigger>(false, data);
                        }
                    }
                        break;
                    case SIMULATE_POPUP_TYPE::WEAPON:
                    {
                        // for testing here
                      /*  auto view = instance.ecs.view<Components::Weapon>(exclude_t<tc::Destroyed>());

                        for (auto id : view)
                        {
                            if (data == UNDEFINED)
                            {
                                data = id;
                                break;
                            }
                        }*/
                    }
                        break;
                    case SIMULATE_POPUP_TYPE::ACTION:
                        break;
                    default:
                        break;
                    }

                    if (UI::UIButton_2("Confirm", "Confirm", { ImGui::GetContentRegionAvailWidth() * 0.8f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.9f }, { 0.f, 0.f }, FONT_PARA))
                    {
                        ImGui::CloseCurrentPopup();
                        enable_popup = false;

                        Service<EventManager>::Get().instant_dispatch<SimulateSelectionConfirm>(type, is_attacker, data);
                    }
                }

                ImGui::EndPopup();
            }
        }

        Entity data;
        bool enable_popup = false;
        bool is_attacker = false;
        ImVec2 position{ 0,0 };
        SIMULATE_POPUP_TYPE type{ SIMULATE_POPUP_TYPE::UNIT};
        ImGuiWindowFlags flags{ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                               ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove };
    };
}