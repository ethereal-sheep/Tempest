/**********************************************************************************
* \author		Huang Xurong(h.xurong@digipen.edu),
                Lim Ziyi Jean(ziyijean.lim@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
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
    class PauseOverlay : public Window
    {
        const char* window_name() override
        {
            return "PuaseOverlay";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar;

            Service<EventManager>::Get().register_listener<PauseOverlayTrigger>(&PauseOverlay::open_popup, this);
        }
        void open_popup(const Event& e);

        void show(Instance&) override;

        bool OverlayOpen = false;
        bool EscDuringPause = false;
        bool CanOpenGraph = false;
        bool FromCombatMode = false;
        bool FromPlaceUnit = false;
        ImGuiID HoveredID{ 0 };
    };
}