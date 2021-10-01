#pragma once
#include "Instance/Instance.h"
#include "Font.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"
#include "Graphics/Basics/RenderSystem.h"
#include "assimp/Exporter.hpp"


namespace Tempest
{
    class ImportBrowser : public Window
    {
        const char* window_name() override
        {
            return "ImportBrowser";
        }
        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<OpenFileTrigger>(&ImportBrowser::open_popup, this);
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
                    "ImportDialog",
                    "Select file to compile",
                    "Model file (*.fbx;*.3ds;){.fbx,.3ds},.*");
                enable_popup = false;
            }



            if (ifd::FileDialog::Instance().IsDone("ImportDialog")) {
                if (ifd::FileDialog::Instance().HasResult()) {

                    open_second = true;
                    in_path = ifd::FileDialog::Instance().GetResult();
                }
                ifd::FileDialog::Instance().Close();
            }

            if (open_second)
            {
                ifd::FileDialog::Instance().Open(
                    "ExportDialog",
                    "Select export directory",
                    "");
                open_second = false;
            }

            if (ifd::FileDialog::Instance().IsDone("ExportDialog")) {
                if (ifd::FileDialog::Instance().HasResult()) {

                    out_path = ifd::FileDialog::Instance().GetResult();

                    ImGui::OpenPopup("Confirm?");
                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                }
                ifd::FileDialog::Instance().Close();
            }

            if (ImGui::BeginPopupModal("Confirm?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {

                auto filename = in_path.stem();
                filename.replace_extension(".glb");
                auto new_out_path = out_path / filename;


                ImGui::Text("Compiling:");
                ImGui::Text("%s", in_path.string().c_str());
                ImGui::Text("To:");
                ImGui::Text("%s", new_out_path.string().c_str());

                if (ImGui::Button("Ok"))
                {
                    ImGui::CloseCurrentPopup();

                    Assimp::Importer importer;

                    auto scene = importer.ReadFile(in_path.string(),
                        aiProcess_Triangulate |
                        aiProcess_GenSmoothNormals |
                        aiProcess_JoinIdenticalVertices);

                    if (!scene)
                    {
                        Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("Failed Import!");
                        return;
                    }

                    Assimp::Exporter exporter;
                    auto result = exporter.Export(scene, "glb2", new_out_path.string());

                    if (!(result == aiReturn_SUCCESS))
                    {
                        Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("Failed Compilation!");
                        return;
                    }
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
        bool open_second = false;
        tpath in_path;
        tpath out_path;


    };
}