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
    class UnitSheetOverlay : public Window
    {
        enum TABS_TYPE
        {
            UNIT,
            WEAPON,
            ITEM,
            ACTION,
            TOTAL
        };

        struct TabImageData
        {
            enum STATE
            {
                UNHOVER,
                HOVER
            };

            std::array< void*, 2> image_id;
            STATE current_state{ UNHOVER };
            ImVec2 size{ 0,0 };
            bool is_active{ false };
        };

        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

            Service<EventManager>::Get().register_listener<OpenUnitSheetTrigger>(&UnitSheetOverlay::open_popup, this);

            initialise_tabs();
        }
        void open_popup(const Event& e);

        void show(Instance&) override;

        // helpers
        void push_button_style() const;
        void pop_button_style() const;
        void initialise_tabs();

        template<typename F>
        void render_tabs(TABS_TYPE type, F&& func);

        bool OverlayOpen = false;
        bool IsUnitCreation = false;
        std::array<TabImageData, TOTAL> Tabs;
        TABS_TYPE CurrentTab{ TABS_TYPE::UNIT };
        tc::Character NewCharacter;
        Entity SelectedID = INVALID;

   /*     bool AddWeaponPopup = false;
        bool EditWeaponPopup = false;
        bool CreateOpen = false;
        std::string Title = "Unit Creation";
        tc::Weapon NewWeap;
        tc::Weapon EditWeap;
        tc::Character NewCharacter;
        Entity CurSelection = UNDEFINED;
        Entity SelectedID = INVALID;
        std::string NewStatName = "Stat";
        int NewStatValue = 0;*/

    };
}