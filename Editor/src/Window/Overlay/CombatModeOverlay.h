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
        void render_more_info(Instance& instance, const ImGuiViewport& viewport, INFO_TYPE type);
        bool OverlayOpen = false;

        Entity curr_entity = UNDEFINED;
        bool display_curr_stat{ false };
        bool display_other_stat{ false };
        INFO_TYPE info_type{};
    };
}