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
#include "Instance/CreatorInstance.h"
#include "Font.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"


namespace Tempest
{
    class TargetingBrowser : public Window
    {
        const char* window_name() override
        {
            return "ImportBrowser";
        }
        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<TargetingTrigger>(&TargetingBrowser::open_popup, this);
        }

        void open_popup(const Event&)
        {
            enable_popup = true;
        }

        void show(Instance& instance) override
        {
            // enable browser
            if (enable_popup)
            {
                ifd::FileDialog::Instance().Open("DirectoryOpenDialog", "Target a directory", "");
                enable_popup = false;
            }

            if (ifd::FileDialog::Instance().IsDone("DirectoryOpenDialog")) {
                if (ifd::FileDialog::Instance().HasResult()) {

                    out_path = ifd::FileDialog::Instance().GetResult();

                    ImGui::OpenPopup("Confirm?##TargetingBrowser");
                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                }
                ifd::FileDialog::Instance().Close();
            }

            if (ImGui::BeginPopupModal("Confirm?##TargetingBrowser", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {

                // we can do some sanity check here also and warn
                if (out_path.filename() != "Resource")
                {
                    ImGui::Text("Target directory is not a Resource folder.");
                    ImGui::Text("Are you sure you want to target %s?", out_path.string().c_str());
                }
                else
                {
                    ImGui::Text("Target %s? ", out_path.string().c_str());
                }


                if (ImGui::Button("Ok"))
                {
                    LOG_ASSERT(fs::exists(out_path));

                    auto& creator = dynamic_cast<CreatorInstance&>(instance);
                    creator.retarget(out_path);
                    ImGui::CloseCurrentPopup();
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
        tpath out_path;


    };
}