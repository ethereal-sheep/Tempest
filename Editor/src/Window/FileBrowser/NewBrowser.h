#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"

namespace Tempest
{
    class NewBrowser : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init() override
        {
            Service<EventManager>::Get().register_listener<NewProjectTrigger>(&NewBrowser::open_popup, this);
        }

        void open_popup(const Event&)
        {
            enable_popup = true;
        }

        void show(Instance& ) override
        {
            // enable browser
            if (enable_popup)
            {
                ImGui::OpenPopup("New...");
                //browser.current_path = R"(S:\Development\Projects\)";
                enable_popup = false;
            }
            if (browser.showFileDialog("New...", imgui_addons::ImGuiFileBrowser::DialogMode::SELECT, ImVec2(700, 310), ".json"))
            {
                ImGui::OpenPopup("Confirm?");
                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                path = browser.selected_path;
            }

            if (ImGui::BeginPopupModal("Confirm?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("New folder for * will be created at:");
                ImGui::Text(browser.selected_path.c_str());

                if (ImGui::Button("Cancel", { ImGui::GetContentRegionAvailWidth() / 2.f, 0 }))
                {
                    enable_popup = true;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Okay", { -1, 0 }))
                {
                    ImGui::CloseCurrentPopup();
                    Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(
                        tpath(browser.selected_path), MemoryStrategy(DebugFlag::DEBUG_NON_VERBOSE), InstanceType::EDIT_TIME);
                }
                ImGui::EndPopup();
            }
        }

        bool enable_popup = false;
        string path;
        imgui_addons::ImGuiFileBrowser browser;


    };
}