/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include <Editor/src/Triggers/Triggers.h>

namespace Tempest
{
    class TurnOrderOverlay : public Window
    {
        enum class TURN_ORDER_STATE
        {
            ORDER_ADD_UNITS,
            ORDER_TURN_MAIN,
            ORDER_DICE,
            ORDER_STAT,
            ORDER_DICE_STAT,
            ORDER_CUSTOM
        };

        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

            Service<EventManager>::Get().register_listener<OpenTurnOrderOverlay>(&TurnOrderOverlay::open_popup, this);

            OverlayOpen = true;

            turn_order_state = TURN_ORDER_STATE::ORDER_ADD_UNITS;
            character_icon = tex_map["Assets/CharacterIcon.png"];
            unit_black = tex_map["Assets/Unit_Black.png"];
            buttons[0] = tex_map["Assets/DiceRollButtonUnlit.png"];
            buttons[1] = tex_map["Assets/DiceRollStatsButtonUnlit.png"];
            buttons[2] = tex_map["Assets/StatsButtonUnlit.png"];
            buttons[3] = tex_map["Assets/CustomButtonUnlit.png"];
            current_stat = "";
        }

        void open_popup(const Event& e);

        void show(Instance&) override;

        bool OverlayOpen = true;

        std::string overlay_title{ "" };
        std::string current_stat{ "" };
        TURN_ORDER_STATE turn_order_state{ TURN_ORDER_STATE::ORDER_ADD_UNITS };
        tsptr<Texture> unit_black;
        tsptr<Texture> character_icon;
        std::array<tsptr<Texture>, 4> buttons;
        tvector<Entity> added_entities; // temp

    };
}