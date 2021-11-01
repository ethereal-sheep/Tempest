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
    class WeaponSheetOverlay : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

            Service<EventManager>::Get().register_listener<OpenWeaponSheetTrigger>(&WeaponSheetOverlay::open_popup, this);

        }
        void open_popup(const Event& e);

        void show(Instance&) override;

        tsptr<Texture> UseInfo;
        tsptr<Texture> WeaponInfo;
        bool OverlayOpen = false;
        tc::Weapon NewWeapon;
        Entity SelectedID = INVALID;
    };
}