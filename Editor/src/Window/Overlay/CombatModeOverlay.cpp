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

					// render the entity stat

					if (!display_curr_stat)
					{
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

						const float infoSize = viewport->Size.y * 0.8f / 3.0f;
						ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y * 0.2f});
						render_more_info(instance, *viewport, INFO_TYPE::CHAR);
						ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y * 0.2f + infoSize }); 
						render_more_info(instance, *viewport, INFO_TYPE::ACTIONS);
						ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y * 0.2f + infoSize * 2.0f});
						render_more_info(instance, *viewport, INFO_TYPE::WEAPONS);

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
		if (ImGui::BeginChild("Char more info" + static_cast<int>(type), ImVec2{ viewport.Size.x * 0.3f, viewport.Size.y * 0.8f / 3.0f }, true))
		{
			auto& charac = instance.ecs.get<tc::Character>(curr_entity);

			ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
			ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 5.f };
			ImVec2 winMax = { winMin.x + ImGui::GetWindowWidth() * 0.5f, winMin.y + ImGui::GetWindowHeight() * 0.1f };
			ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
			ImVec4 textcol = { 0,0,0,1 };
			if (!ImGui::IsWindowFocused())
			{
				col = { 0.980f, 0.768f, 0.509f, 0.7f };
				textcol = { 0,0,0,0.7f };
			}

			ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
			ImGui::PushFont(FONT_OPEN);
			switch (type)
			{
			case Tempest::CombatModeOverlay::INFO_TYPE::CHAR:
				ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), charac.name.c_str());
				break;
			case Tempest::CombatModeOverlay::INFO_TYPE::ACTIONS:
				ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), "ACTIONS");
				break;
			case Tempest::CombatModeOverlay::INFO_TYPE::WEAPONS:
				ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), "WEAPONS");
				break;
			default:
				break;
			}

			ImGui::Dummy(ImVec2{0, 30.0f});

			if (ImGui::BeginChild("Internal content", ImVec2{ ImGui::GetContentRegionAvailWidth() * 0.95f, ImGui::GetContentRegionAvail().y * 0.8f }, true))
			{
				ImVec2 cursor{ ImGui::GetCursorPosX() + 100.0f, ImGui::GetCursorPosY() + 30.0f };
				unsigned i = 0; 
				unsigned j = 0;
				switch (type)
				{
				case Tempest::CombatModeOverlay::INFO_TYPE::CHAR:
				{
					auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
					Entity StateLineId = UNDEFINED;
					for (auto id : StatsView)
						StateLineId = id;
					auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);

					ImGui::Dummy(ImVec2{ 0.0f, 10.0f });
					ImGui::PushFont(FONT_BODY);
					bool NextLine = false;
					ImVec2 PrevPos{ 0.f ,0.f };
					float frontPadding = 5.f;
					for (auto counter = 0; counter < sl->size(); counter++)
					{
						if ((*sl)(counter))
						{
							string stat = sl->operator[](counter) + " " + std::to_string(charac.get_stat(counter));

							if (!NextLine)
							{
								ImGui::Dummy({ frontPadding * 15, 0 });
								ImGui::SameLine();
								PrevPos = ImGui::GetCursorPos();
								ImGui::Text(stat.c_str());
								ImGui::Dummy({ frontPadding * 15, 0 });
								ImGui::SameLine();
							}

							else
							{
								ImGui::SetCursorPos(PrevPos);
								ImGui::Dummy({ 250.f - frontPadding * 15, 0.f });
								ImGui::SameLine();

								ImGui::Text(stat.c_str());
								ImGui::Dummy({ 250 + frontPadding, 0 });
								ImGui::SameLine();

								ImGui::SetCursorPos(PrevPos);
								ImGui::Dummy({ 0, 50.f });
							}

							NextLine = !NextLine;

						}

					}
					ImGui::PopFont();
				}
					// display stas
					break;
				case Tempest::CombatModeOverlay::INFO_TYPE::ACTIONS:
					// display actions
					for (auto id : charac.actions)
					{
						auto& action = instance.ecs.get<tc::Graph>(id);
						if (UI::UIButton_2(action.g.name.c_str(), action.g.name.c_str(), { cursor.x + i++ * 190, cursor.y + j * 50 }, { 0,0 }, FONT_PARA))
						{
							// go edit page
						}

						if (i / 2)
						{
							i = 0;
							++j;
						}
					}
					break;
				case Tempest::CombatModeOverlay::INFO_TYPE::WEAPONS:
					// display weapons
					for (auto id : charac.weapons)
					{
						auto& weapon = instance.ecs.get<tc::Weapon>(id);
						if (UI::UIButton_2(weapon.name.c_str(), weapon.name.c_str(), { cursor.x + i++ * 190, cursor.y + j * 50 }, { 0,0 }, FONT_PARA))
						{
							// go edit page
						}

						if (i / 2)
						{
							i = 0;
							++j;
						}
					}
					break;
				default:
					break;
				}
			}
			ImGui::EndChild();
			
			ImGui::PopFont();

			// display the edit here	
		}
		ImGui::EndChild();
	}
}