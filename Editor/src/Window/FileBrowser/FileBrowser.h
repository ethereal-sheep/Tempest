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

namespace Tempest
{
    class FileBrowser : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
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