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
#include "Font.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"
#include "Graphics/Basics/RenderSystem.h"

namespace Tempest
{
    class OpenFile : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<OpenFileTrigger>(&OpenFile::open_popup, this);
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
                ifd::FileDialog::Instance().Open(
                    "DirectoryOpenDialog", 
                    "Open a directory", 
                    "Model file (*.fbx;*.3ds;){.fbx,.3ds},.*");
                enable_popup = false;
            }


            if (ifd::FileDialog::Instance().IsDone("DirectoryOpenDialog")) {
                if (ifd::FileDialog::Instance().HasResult()) {
                    ImGui::OpenPopup("Confirm Open?");
                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                    path = ifd::FileDialog::Instance().GetResult();
                }
                ifd::FileDialog::Instance().Close();
            }

            if (ImGui::BeginPopupModal("Confirm Open?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("%s", path.string().c_str());

                if (ImGui::Button("Ok"))
                {
                    ImGui::CloseCurrentPopup();
                    
                    auto entity = instance.ecs.create();
                    auto m = instance.ecs.emplace<tc::Model>(entity);
                    m->path = path.string();
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