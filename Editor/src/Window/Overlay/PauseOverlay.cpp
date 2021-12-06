/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"
#include "PauseOverlay.h"

namespace Tempest
{
	void PauseOverlay::open_popup(const Event&)
	{
		OverlayOpen = true;
	}

	void PauseOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		if (OverlayOpen)
		{
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.8f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

			if (ImGui::Begin("PauseMenu", nullptr, window_flags))
			{
				// render title
				ImGui::SetCursorPos(ImVec2{ 0,0 });
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
				UI::SubHeader("Paused");
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });

				if (UI::UIButton_2("Save", "Save", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.3f }, { 50,20}, FONT_BTN))
				{

				}
				if (UI::UIButton_2("Load", "Load", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.4f }, { 50,20 }, FONT_BTN))
				{

				}
				if (UI::UIButton_2("Edit Conflict Resolution", "Edit Conflict Resolution", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.5f }, { 50,20 }, FONT_BTN))
				{

				}
				if (UI::UIButton_2("Return", "Return", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.6f }, { 50,20 }, FONT_BTN))
				{
					OverlayOpen = false;
				}

				// where does this go to?
				if (UI::UIButton_2("Quit", "Quit", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.7f }, { 50,20 }, FONT_BTN))
				{

				}
			}
			ImGui::End();

			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}
	}
}