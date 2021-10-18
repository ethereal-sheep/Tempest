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
#include "Triggers/Triggers.h"

namespace Tempest
{

    class ImportAssetPopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }

        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<ImportAssetTrigger>(&ImportAssetPopup::open_popup, this);
        }

        void open_popup(const Event&)
        {
            enable_popup = true;
        }

        void show(Instance& instance) override
        {
            if (enable_popup)
            {
                ifd::FileDialog::Instance().Open("TextureOpenDialog", "Open a texture", "Image file (*.png;*.jpg;*.jpeg;*.bmp;*.tga){.png,.jpg,.jpeg,.bmp,.tga},.*");
                enable_popup = false;
            }

            if (ifd::FileDialog::Instance().IsDone("TextureOpenDialog")) {
                if (ifd::FileDialog::Instance().HasResult()) {
                    ImGui::OpenPopup("Confirm Import?");
                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                    path = ifd::FileDialog::Instance().GetResult();
                }
                ifd::FileDialog::Instance().Close();
            }

            if (ImGui::BeginPopupModal("Confirm Import?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                std::size_t found = path.string().find_last_of("/\\");
                std::string asset = path.string().substr(found + 1);

                ImGui::PushFont(FONT_BOLD);
                ImGui::Text("%s", asset.c_str());
                ImGui::PopFont();
                ImGui::SameLine();
                ImGui::Text("will be imported into %s project \n", instance.get_name().c_str());

                if (ImGui::Button("Ok"))
                {
                    /* Call importer here */
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
        tpath path;
    };
}