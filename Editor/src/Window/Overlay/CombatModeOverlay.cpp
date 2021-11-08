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

namespace Tempest
{
	void CombatModeOverlay::open_popup(const Event& e)
	{
		auto a = event_cast<OpenCombatModeTrigger>(e);

		OverlayOpen = true;
		window_flags |= ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground;

		for (auto id : a.instance.ecs.view<Components::Character>(exclude_t<tc::Destroyed>()))
		{
			curr_entity = id;
			break;
		}
	}

	void CombatModeOverlay::show(Instance& instance)
	{
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		if (OverlayOpen)
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.f});

			if (ImGui::Begin("Combat Mode Screen", nullptr, window_flags))
			{

				if (ImGui::Button("X"))
					OverlayOpen = false;

				// top bar

				if (curr_entity != UNDEFINED)
				{
					ImGui::Dummy(ImVec2{ 5.0f,0.f });

					// TODO: get from the added list
					bool first = true;
					float padding = 0.0f;
					for (auto id : instance.ecs.view<Components::Character>(exclude_t<tc::Destroyed>()))
					{
						// check if turn is over

						UI::CharacterTurn(instance, id, { 0.f, ImGui::GetCursorPosY() + padding }, first);
						padding += 100.0f;
						first = false;
					}

					// render the entity stat

					if (!display_curr_stat)
					{
						auto& tex = tex_map["Assets/Placeholder_Character.png"];
						UI::CharacterTurnData(instance, curr_entity, { 0.f, viewport->Size.y - tex->GetHeight() });
						if (UI::UIButton_2("More Information >", "More Information >", ImVec2{ viewport->Size.x * 0.3f, viewport->Size.y * 0.72f }, { 0.f, 8.0f }, FONT_BODY))
						{
							display_curr_stat = true;
						}
					}
					else
					{
						if (UI::UIButton_2("< Hide", "< Hide", ImVec2{ viewport->Size.x * 0.3f, viewport->Size.y * 0.15f }, { 0.f, 8.0f }, FONT_BODY))
						{
							display_curr_stat = false;
						}

						const ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };
						ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.f);
						ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);
						ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
						ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
						ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.06f,0.06f, 0.06f, 0.85f });

						ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.3f, viewport->Size.y * 2.0f});
						render_more_info(instance, *viewport, INFO_TYPE::CHAR);
						// render actions
						//render weapons
						ImGui::PopStyleColor(2);
						ImGui::PopStyleVar(3);
						
					}
					
				}
				
				if (UI::UIButton_2("Add Units", "Add Units", ImVec2{ viewport->Size.x * 0.9f, viewport->Size.y * 0.06f }, { 10.f,10.f }, FONT_PARA))
				{
				}
			}

            ImGui::End();

			ImGui::PopStyleVar();
            ImGui::PopStyleColor();
            
		}
	}

	void CombatModeOverlay::render_more_info(Instance& instance, const ImGuiViewport& viewport, INFO_TYPE type)
	{
		if (ImGui::BeginChild("Char more info", ImVec2{ viewport.Size.x * 0.3f, viewport.Size.y * 0.8f / 3.0f }, true))
		{
			auto& charac = instance.ecs.get<tc::Character>(curr_entity);

			ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
			ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 5.f };
			ImVec2 winMax = { winMin.x + ImGui::GetWindowWidth() * 0.5f, winMin.y + ImGui::GetWindowHeight() * 0.05f };
			ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
			ImVec4 textcol = { 0,0,0,1 };
			if (!ImGui::IsWindowFocused())
			{
				col = { 0.980f, 0.768f, 0.509f, 0.7f };
				textcol = { 0,0,0,0.7 };
			}

			ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
			ImGui::PushFont(FONT_OPEN);
			ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), charac.name.c_str());
			ImGui::PopFont();
		}
		ImGui::EndChild();
	}
}