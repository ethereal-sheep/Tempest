/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
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
#include "Triggers/Triggers.h"
#include "Util/interpolater.h"

namespace Tempest
{
    class Delay : public Window
    {

        enum struct State
        {
            WAITING,
            DONE,
        };

        float wait_time = 0, timer = 0;
        std::function<void(void)> to_do;
        State state = State::DONE;

        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoDocking |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing |
                ImGuiWindowFlags_NoNav;

            Service<EventManager>::Get().register_listener<DelayTrigger>(&Delay::wait, this);
        }

        void wait(const Event& e)
        {
            if (state == State::DONE)
            {
                auto& a = event_cast<DelayTrigger>(e);
                timer = 0;
                wait_time = a.wait_time;
                to_do = a.do_fn;
                state = State::WAITING;
            }
        }


        void show(Instance&) override
        {
            timer += ImGui::GetIO().DeltaTime;
            switch (state)
            {
            case Tempest::Delay::State::WAITING:
                if (timer >= wait_time)
                {
                    std::invoke(to_do);
                    state = State::DONE;
                }
                break;
            case Tempest::Delay::State::DONE:
                break;
            default:
                break;
            }

        }



    };
}