#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"

namespace Tempest
{
    class FileBrowser : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init() override
        {
            //Service<EventManager>::Get().register_listener<OverlayTrigger>(&test_overlay_popup::open_popup, this);
        }

        void open_popup(const Event& e)
        {
            //auto a = event_cast<OverlayTrigger>(e);
            //data = a.msg;
            //state = state::APPEAR;
        }

        void show(Instance&) override
        {
            
        }

        string data;
        float visiblity = 0;
        float timer = 0;

    };
}