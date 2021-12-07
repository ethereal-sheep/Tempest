/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Instance/EditTimeInstance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"

namespace Tempest
{

    class OpenProjectPopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<OpenProjectTrigger>(&OpenProjectPopup::open_popup, this);
        }

        void open_popup(const Event&)
        {
            enable_popup = true;
        }

        void show(Instance& instance) override
        {
            const string browser_name = "Select directory";

            // enable browser
            if (enable_popup)
            {
                ifd::FileDialog::Instance().Open("ProjectOpenDialog", browser_name, "Project file (*.json){.json}");
                enable_popup = false;
            }

            std::vector<tpath> recents;
            Service<EventManager>::Get().instant_dispatch<GetRecentUtil>(recents);

            if (ifd::FileDialog::Instance().IsDone("ProjectOpenDialog", &recents)) {
                if (ifd::FileDialog::Instance().HasResult()) 
                {
                    path = ifd::FileDialog::Instance().GetResult();

                    if (instance.get_path() == path.parent_path()) // if path are the same
                    {
                        Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("Project is already open!");
                    }
                    else
                    {
                        Service<EventManager>::Get().instant_dispatch<SaveCurrentBeforeOpenTrigger>(path);
                    }
                }
                ifd::FileDialog::Instance().Close();
            }
        }

        bool enable_popup = false;
        tpath path;


    };
}