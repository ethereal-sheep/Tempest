/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "CombatModeOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"
#include "ConflictResOverlay.h"

namespace Tempest
{
	void ConflictResOverlay::open_popup(const Event&)
	{
		OverlayOpen = true;
		LeftBg = tex_map["Assets/ConflictBG_Left.png"];
		ConflictBg = tex_map["Assets/ConflictBG.png"];
	}
	void ConflictResOverlay::show([[maybe_unused]]Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		if (OverlayOpen)
		{
			window_flags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

			//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
			if (ImGui::Begin("Conflict Resolution", &visible, window_flags))
			{
				auto drawlist = ImGui::GetWindowDrawList();
				auto pos = ImGui::GetCursorPos();
				ImVec2 leftBgMax = { pos.x + LeftBg->GetWidth(), pos.y + LeftBg->GetHeight() };
				ImVec2 bgMax = { pos.x + ConflictBg->GetWidth(), pos.y + ConflictBg->GetHeight() };
				drawlist->AddImage((void*)static_cast<size_t>(ConflictBg->GetID()), pos, bgMax);
				drawlist->AddImage((void*)static_cast<size_t>(LeftBg->GetID()), pos, leftBgMax);

				static float x = 10.f;
				static float y = 10.f;
				ImGui::DragFloat("##12", &x);
				ImGui::DragFloat("##132", &y);
				ImGui::SetCursorPos({ pos.x + x,pos.y + y });
				UI::UIConflictSelectable("CONFLICT RES", false, 1);
				UI::UIConflictSelectable("CONFLICT RES", false, 0);

				if (ImGui::Button("Close"))
				{
					OverlayOpen = false;
				}
			}
			ImGui::PopStyleVar();
			ImGui::End();
			
			//ImGui::PopStyleColor();
		}
	}
}