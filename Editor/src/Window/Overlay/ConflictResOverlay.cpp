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
				ImVec2 max = { pos.x + viewport->Size.x,pos.y + viewport->Size.y };
				
				drawlist->AddImage((void*)static_cast<size_t>(ConflictBg->GetID()), pos, max);
				auto scale = viewport->Size / initViewPortSize ;
				auto scale2 = initViewPortSize / viewport->Size ;
				auto scale3 = scale * scale2 ;

				ImGui::Dummy({ 0, 70.f });
				UI::SubHeader("Conflict Resolution");
				ImGui::SetCursorPos({0.f, pos.y + viewport->Size.y * 0.2f});
				auto curPos = ImGui::GetCursorPos();
				for (int i = 0; i < 3; i++)
				{
					ImGui::SetCursorPos(curPos);
					ImGui::Dummy({ pos.x + viewport->Size.x * 0.3f, 0.f });
					ImGui::SameLine();
					string str = "CONFLICT RES##" + std::to_string(i);
					UI::UIConflictSelectable(str.c_str(), false, 1);
					//ImGui::Dummy({ 0.f, 60.f });
					ImGui::Dummy({ 0.f, viewport->Size.y * 0.07f });
					curPos = ImGui::GetCursorPos();
					
				}
				
				auto tex = tex_map["Assets/BackMenuBtn.png"];
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f,viewport->Size.y * 0.03f });
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
				if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
				{
					OverlayOpen = false;
				}
				ImGui::PopStyleColor(3);
			}
			ImGui::PopStyleVar();
			ImGui::End();
			
			//ImGui::PopStyleColor();
		}
	}
}