#pragma once
#include "Instance/EditTimeInstance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"

namespace Tempest
{

    class SaveProjectPopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<SaveProjectTrigger>(&SaveProjectPopup::open_popup, this);
        }

        void open_popup(const Event&)
        {
            enable = true;
        }

        void show(Instance& instance) override
        {
            auto& edit = dynamic_cast<EditTimeInstance&>(instance);

            // enable browser
            if (enable)
            {
                edit.save();
                enable = false;
            }

        }
        bool save_or_save_as = false;
        bool enable = false;
        tpath path;


    };
}