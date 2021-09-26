#pragma once
#include "Instance/Instance.h"
#include "Events/EventManager.h"
#include "Font.h"

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

        void show(Instance&) override
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

			}
			ImGui::End();
        }

    };
}