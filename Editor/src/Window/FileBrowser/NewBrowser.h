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
        void init(Instance&) override
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
                ifd::FileDialog::Instance().Open("DirectoryOpenDialog", "Open a directory", "");
                enable_popup = false;
            }


            if (ifd::FileDialog::Instance().IsDone("DirectoryOpenDialog")) {
                if (ifd::FileDialog::Instance().HasResult()) {
                    ImGui::OpenPopup("Confirm?");
                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                    path = ifd::FileDialog::Instance().GetResult();
                }
                ifd::FileDialog::Instance().Close();
            }

            if (ImGui::BeginPopupModal("Confirm?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("New folder for * will be created at:");
                ImGui::Text(path.string().c_str());

                if (ImGui::Button("Ok"))
                {
                    ImGui::CloseCurrentPopup();
                    Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(
                        tpath(path), MemoryStrategy(DebugFlag::DEBUG_NON_VERBOSE), InstanceType::EDIT_TIME);
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    enable_popup = true;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        bool enable_popup = false;
        tpath path;


    };
}