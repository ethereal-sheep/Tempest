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
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"

namespace Tempest
{
    class SaveBrowser : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<TEvent<string>>(&SaveBrowser::open_popup, this);
        }

        void open_popup(const Event&)
        {
            enable_popup = true; 
        }

        void show(Instance& ) override
        {
            if (enable_popup)
            {
                ifd::FileDialog::Instance().Open(
                    "TextureOpenDialog",
                    "Open a texture",
                    "Image file (*.png;*.jpg;*.jpeg;*.bmp;*.tga){.png,.jpg,.jpeg,.bmp,.tga},.*");
                enable_popup = false;
            }
            if (ifd::FileDialog::Instance().IsDone("TextureOpenDialog")) {
                if (ifd::FileDialog::Instance().HasResult()) {
                    std::string res = ifd::FileDialog::Instance().GetResult().u8string();
                    LOG("OPEN[{}]\n", res.c_str());
                }
                ifd::FileDialog::Instance().Close();
            }
        }

        bool enable_popup = false;


    };
}