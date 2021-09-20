#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include <Editor/src/Triggers/Triggers.h>

namespace Tempest
{
    class UnitSheetOverlay : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

            Service<EventManager>::Get().register_listener<OpenUnitSheetTrigger>(&UnitSheetOverlay::open_popup, this);
        }
        void open_popup(const Event& e);

        void show(Instance&) override;

        bool OverlayOpen = false;
        bool AddWeaponPopup = false;
        bool AddStatPopup = false;
        bool EditWeaponPopup = false;
        bool CreateOpen = false;
        bool IsUnitCreation = false;
        std::string Title = "Unit Creation";
        tc::Weapon NewWeap;
        tc::Weapon EditWeap;
        tc::Character NewCharacter;
        Entity CurSelection = UNDEFINED;
        Entity SelectedID = INVALID;
        std::string NewStatName = "Stat";
        int NewStatValue = 0;


    };
}