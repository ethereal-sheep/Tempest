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
#include "Events/EventManager.h"
#include "Font.h"
#include "assimp/Exporter.hpp"

namespace Tempest
{
    class Home : public Window
    {
        const char* window_name() override
        {
            return "Home";
        }
        void init(Instance&) override
        {
            window_flags |=
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoDocking |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_MenuBar;
        }

        void show(Instance& instance) override
        {
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImVec2 size = ImGui::GetMainViewport()->Size;
			ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
			ImGui::SetNextWindowSize(ImVec2(size.x / 1.5f, size.y / 1.5f), ImGuiCond_Always);
			if (ImGui::Begin(window_name(), nullptr, window_flags))
			{
				// menu bar
				{
					if (ImGui::BeginMenuBar())
					{
						if (ImGui::BeginMenu("Project"))
						{
							if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open", "", false))
							{
								Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Opening...");
								Service<EventManager>::Get().instant_dispatch<OpenFileTrigger>();
							}
							ImGui::Dummy({ 0.f, 1.f });
							ImGui::Separator();
							ImGui::Dummy({ 0.f, 1.f });

							if (ImGui::MenuItem(ICON_FA_COMPRESS_ARROWS_ALT " Compile Test", "", false)) 
							{
								test_compile();
							}
							if (ImGui::MenuItem(ICON_FA_FILE_IMPORT " Import Test", "", false))
							{
								test_load_res();
							}


							ImGui::Dummy({ 0.f, 1.f });
							ImGui::Separator();
							ImGui::Dummy({ 0.f, 1.f });

							if (ImGui::MenuItem(ICON_FA_FILE_EXPORT " Export", "Ctrl+Shift+S", false, false)) {}

							ImGui::Dummy({ 0.f, 1.f });
							ImGui::Separator();
							ImGui::Dummy({ 0.f, 1.f });

							if (ImGui::MenuItem(ICON_FA_DOOR_OPEN " Exit", "", false))
							{
								Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Application Exiting in 10s...");
							}

							ImGui::EndMenu();
						}
						ImGui::EndMenuBar();
					}
				}

				// debugging


				for (auto i : instance.ecs.view<tc::Model>())
				{
					auto& m = instance.ecs.get<tc::Model>(i);
					

					ImGui::Text(m.path.c_str());

				}



			}
			ImGui::End();
        }

		void test_compile()
		{
			// test if we can import then export

			Assimp::Importer importer;
			const char* in_path = R"(S:\Development\Tempest\Resource\Models\Sword.fbx)";
			const char* out_path = R"(S:\Development\temp\testing_export\Sword.glb)";

			auto scene = importer.ReadFile(in_path,
				aiProcess_Triangulate |
				aiProcess_GenSmoothNormals |
				aiProcess_JoinIdenticalVertices);

			if (!scene)
			{
				Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("Failed Import!");
				return;
			}


			Assimp::Exporter exporter;
			auto result = exporter.Export(scene, "glb2", out_path);

			if (!(result == aiReturn_SUCCESS))
			{
				Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("Failed Compilation!");
				return;
			}
		}
		void test_load_res()
		{
			Assimp::Importer importer;
			const char* in_path = R"(S:\Development\temp\testing_export\Sword.glb)";

			auto scene = importer.ReadFile(in_path,
				aiProcess_Triangulate |
				aiProcess_GenSmoothNormals |
				aiProcess_JoinIdenticalVertices);

			

			if (!scene)
			{
				Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("Failed import of Resource!");
				return;
			}
		}

    };
}