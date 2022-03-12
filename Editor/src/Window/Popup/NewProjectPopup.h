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
#include "Triggers/Triggers.h"

namespace Tempest
{

    class NewProjectPopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<NewProjectTrigger>(&NewProjectPopup::open_popup, this);
        }

        void open_popup(const Event&)
        {
            enable_popup = true;
        }

        void show(Instance&) override
        {
            const string popup_1_name = "New Project Name?";
            const string popup_2_name = "Confirm?";
            const string browser_1_name = "Select directory";

            // enable browser
            if (enable_popup)
            {
                ImGui::OpenPopup(popup_1_name.c_str());
                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                enable_popup = false;
            }

            if (ImGui::BeginPopupModal(popup_1_name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::PushItemWidth(250.0f);
                ImGui::InputText("##newfilename", &name, ImGuiInputTextFlags_CharsNoBlank );
                ImGui::PopItemWidth();

                if (ImGui::Button("Create New"))
                {
                    ifd::FileDialog::Instance().Open("DirectoryOpenDialog", browser_1_name, "", false, std::filesystem::current_path().root_path().string());
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }


            if (ifd::FileDialog::Instance().IsDone("DirectoryOpenDialog")) {
                if (ifd::FileDialog::Instance().HasResult()) {
                    ImGui::OpenPopup(popup_2_name.c_str());
                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                    path = ifd::FileDialog::Instance().GetResult();
                }
                ifd::FileDialog::Instance().Close();
            }

            if (ImGui::BeginPopupModal(popup_2_name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("New folder for");
                ImGui::SameLine();
                ImGui::PushFont(FONT_BOLD);
                ImGui::Text("%s", name.c_str());
                ImGui::PopFont();
                ImGui::SameLine();
                ImGui::Text("will be created in:");
                ImGui::Text(path.string().c_str());
                ImGui::Text("\n");
                ImGui::Separator();
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                ImGui::Checkbox("Open project on creation", &load_on_create);
                ImGui::PopStyleVar();

                if (ImGui::Button("Confirm"))
                {
                    try
                    {
                        create_new_project();
                        if (load_on_create)
                        {
                            Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(path / name / (name + ".json"), MemoryStrategy{}, InstanceType::EDIT_TIME);
                        }
                    }
                    catch (const std::exception& a)
                    {
                        Service<EventManager>::Get().instant_dispatch<ErrorTrigger>(a.what());
                    }
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    ifd::FileDialog::Instance().Open("DirectoryOpenDialog", browser_1_name, "");
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }


        class bad_project_create : public std::exception
        {
        public:
            bad_project_create(const string& err_msg = "Project could not be created!") : msg(err_msg) {}
            const char* what() const noexcept override { return msg.c_str(); }
        private:
            string msg;
        };

        void create_new_project()
        {
            if (!std::filesystem::exists(path))
                throw bad_project_create("Filepath does not exist!");

            if (!std::filesystem::create_directory(path / name))
                if(!std::filesystem::is_empty(path / name))
                    throw bad_project_create(name + " project folder already exists!");

            std::ofstream out(path / name / (name + ".json"));
            if(!out.good())
                throw bad_project_create(name + " project files could not be created!");
            out << "";
            out.close();
        }

        bool load_on_create = true;
        bool enable_popup = false;
        tpath path;
        string name = "Untitled";


    };
}