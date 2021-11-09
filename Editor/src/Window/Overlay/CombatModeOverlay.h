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

namespace Tempest
{
    class CombatModeOverlay : public Window
    {
        enum class INFO_TYPE
        {
            CHAR,
            ACTIONS,
            WEAPONS
        };

        enum class BATTLE_STATE
        {
            CURR_TURN,       // display character information and action options
            SELECT_ACTION,   // select performable actions of character
            SELECT_WEAPON,   // select weapons from inventory 
            SELECT_OTHER,    // select enemy to perform action on
            BATTLE_GLIMPSE,  // display chance of success of selected action
            COMMENCE_BATTLE  // simulate battle
        };

        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;

            Service<EventManager>::Get().register_listener<OpenCombatModeTrigger>(&CombatModeOverlay::open_popup, this);
        }
        void open_popup(const Event& e);

        void show(Instance&) override;
        void push_style_color() const;
        void pop_style_color() const;
        void render_more_info(Instance& instance, const ImGuiViewport& viewport, INFO_TYPE type, Entity entity);
        bool OverlayOpen = false;

        Entity curr_entity = UNDEFINED;
        Entity other_entity = UNDEFINED;
        Entity selected_action = UNDEFINED;
        Entity selected_weapon = UNDEFINED;
        float action_button_diff = 0.0f;
        float placeholder_height = 0.0f;
        ImVec2 action_background_size{0.f,0.f};
        bool display_curr_stat{ false };
        bool display_other_stat{ false };
        INFO_TYPE info_type{ INFO_TYPE::CHAR};
        BATTLE_STATE battle_state{ BATTLE_STATE::CURR_TURN };
    };
}