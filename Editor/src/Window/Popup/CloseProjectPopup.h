#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"

namespace Tempest
{

    class CloseProjectPopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init() override
        {
            Service<EventManager>::Get().register_listener<CloseProjectTrigger>(&CloseProjectPopup::open_popup, this);
        }

        void open_popup(const Event&)
        {
            enable_popup = true;
        }

        void show(Instance&) override
        {
            if (enable_popup)
            {
                Service<EventManager>::Get().instant_dispatch<SaveCurrentBeforeOpenTrigger>("");
                enable_popup = false;
            }
        }

        bool enable_popup = false;


    };
}