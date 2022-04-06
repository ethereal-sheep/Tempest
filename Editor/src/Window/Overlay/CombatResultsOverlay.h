/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu)
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
    class CombatResultsOverlay : public Window
    {
        const char* window_name() override
        {
            return "PuaseOverlay";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar;

            Service<EventManager>::Get().register_listener<CombatResultsTrigger>(&CombatResultsOverlay::open_popup, this);
            Service<EventManager>::Get().register_listener<SendCombatResults>(&CombatResultsOverlay::get_results, this);

            character_icon = tex_map["Assets/CharacterIcon.dds"];
        }
        void open_popup(const Event& e);
        void get_results(const Event& e);

        void show(Instance&) override;

        bool OverlayOpen = false;
        ImGuiID HoveredID{ 0 };
        std::map<Entity, UnitResult> units_results;
        tsptr<Texture> character_icon;
    };
}