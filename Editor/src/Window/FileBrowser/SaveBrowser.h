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
        void init() override
        {
            Service<EventManager>::Get().register_listener<TEvent<string>>(&SaveBrowser::open_popup, this);
        }

        void open_popup(const Event&)
        {
            //auto a = event_cast<OverlayTrigger>(e);
            //data = a.msg;
            enable_popup = true;
        }

        void show(Instance& ) override
        {
            if (enable_popup)
            {
                ImGui::OpenPopup("Open");
               //browser.current_path = R"(S:\Development\Projects\)";
                enable_popup = false;
            }
            if (browser.showFileDialog("Open", imgui_addons::ImGuiFileBrowser::DialogMode::SELECT, ImVec2(700, 310), ".json"))
            {
                // what do we need to save
                /*
                * Project
                * Graphs 
                */
            }
        }

        bool enable_popup = false;
        imgui_addons::ImGuiFileBrowser browser;


    };
}