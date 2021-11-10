/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "PlaceUnitsOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"
#include "Triggers/Triggers.h"

namespace Tempest
{
	void PlaceUnitsOverlay::open_popup(const Event& e)
	{
		OverlayOpen = true;
		auto a = event_cast<OpenPlaceUnitsOverlay>(e);
		Entities = a.entities;

		window_flags |= ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground;
	}

	void PlaceUnitsOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		if (OverlayOpen)
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

			if (ImGui::Begin("Place Units Overlay", nullptr, window_flags))
			{
				auto window = ImGui::GetWindowDrawList();
				if (ImGui::Button("X"))
					OverlayOpen = false;

				auto sideBg = tex_map["Assets/FadingSideBG.png"];
				auto imgWidth = tex_map["Assets/SmallTurnSelectedBtn.png"]->GetWidth();
				ImVec2 sideBarSize = { imgWidth + 20.f, ImGui::GetWindowHeight() * 0.6f };
				ImVec2 sideBgMin = ImGui::GetCurrentWindow()->Pos;
				ImVec2 sideBgMax = { sideBgMin.x + (float)sideBg->GetWidth(), sideBgMin.x + (float)ImGui::GetWindowHeight() };
				ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
				ImGui::PushFont(FONT_TURN);
				string str = "PLACING UNIT";
				auto strSize = ImGui::CalcTextSize(str.c_str());
				ImGui::PopFont();

				//ImGui::GetForegroundDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
				window->AddImage((void*)static_cast<size_t>(sideBg->GetID()), sideBgMin, sideBgMax);
				window->AddRectFilled({ sideBgMin.x, sideBgMin.y + ImGui::GetWindowHeight() * 0.1f }, { sideBgMin.x + imgWidth - 20.f,  sideBgMin.y + ImGui::GetWindowHeight() * 0.1f + strSize.y  }, ImGui::GetColorU32(col));
				
				ImGui::PushFont(FONT_TURN);
				window->AddText({ sideBgMin.x + 5.f, sideBgMin.y + ImGui::GetWindowHeight() * 0.1f }, ImGui::GetColorU32({ 0,0,0,1 }), str.c_str());
				ImGui::PopFont();

				ImGui::PushFont(FONT_BODY);
				string str2 = "Place your selected units into the level";
				window->AddText({ sideBgMin.x + 5.f, sideBgMin.y + ImGui::GetWindowHeight() * 0.15f }, ImGui::GetColorU32({ 1,1,1,1 }), str2.c_str());
				ImGui::PopFont();

				ImGui::SetCursorPosY(sideBgMin.y + ImGui::GetWindowHeight() * 0.2f);
				DrawSideBar(instance, sideBarSize);
				ImVec2 buttonPos = { sideBgMin.x + 100.f, sideBgMin.y + ImGui::GetWindowHeight() * 0.87f };
				if (UI::UIButton_2("Turn Order", "Turn Order", buttonPos, { -40.f,20.f }, FONT_BODY))
				{

				}
				if (UI::UIButton_2("Save", "Save", { buttonPos.x + 200.f, buttonPos.y }, { -40.f,20.f }, FONT_BODY))
				{

				}
			}
			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}
	}
	void PlaceUnitsOverlay::DrawSideBar(Instance& instance, ImVec2 size)
	{
		if (ImGui::BeginChild("##PlaceUnitSideBar", size))
		{
			for (auto unit : Entities)
			{
				auto character = instance.ecs.get_if<tc::Character>(unit);
				if (!character)
					continue;

				auto unitIcon = tex_map["Assets/Unit_Black.png"];
				UI::UICharTurnButton((void*)static_cast<size_t>(unitIcon->GetID()), { (float)unitIcon->GetWidth(), (float)unitIcon->GetHeight() }, character->name.c_str(), "##PlaceUnit" + std::to_string(unit), false);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
			}
		}

		ImGui::EndChild();
		

	}
}