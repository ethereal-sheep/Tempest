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
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include <Editor/src/Triggers/Triggers.h>
#include "Util/interpolater.h"

namespace Tempest
{
    class TurnOrderOverlay : public Window
    {
        enum class TURN_ORDER_STATE
        {
            ORDER_ADD_UNITS,
            ORDER_TURN_MAIN,
            ORDER_TURN_SUB
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

            change_state(TURN_ORDER_STATE::ORDER_ADD_UNITS);
            character_icon = tex_map["Assets/CharacterIcon.dds"];
            unit_black = tex_map["Assets/Unit_Black.dds"];
            current_stat = "";
        }

        void open_popup(const Event& e);

        void change_state(TURN_ORDER_STATE state);

        void show(Instance&) override;

        bool OverlayOpen = true;

        bool new_instance{ true };
        int tutorial_index{ 0 };
        std::string overlay_title{ "" };
        std::string current_stat{ "" };
        std::string next_button_name{ "" };
        TURN_ORDER_STATE turn_order_state{ TURN_ORDER_STATE::ORDER_ADD_UNITS };
        tsptr<Texture> unit_black;
        tsptr<Texture> character_icon;
        tvector<Entity> added_entities; // temp

        std::vector<interpolater<float>> inter_nest = std::vector< interpolater<float>>(10);
    };
}