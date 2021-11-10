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

		// hardget the entities
		for (auto id : a.instance.ecs.view<Components::Character>(exclude_t<tc::Destroyed>()))
		{
			if (curr_entity == UNDEFINED)
				curr_entity = id;
			else if (other_entity == UNDEFINED)
				other_entity = id;
			else
				break;
		}

		placeholder_height = tex_map["Assets/Placeholder_Character.png"]->GetHeight();
		action_background_size = ImVec2{ tex_map["Assets/ActionBackdrop.png"]->GetWidth() * 1.0f, tex_map["Assets/ActionBackdrop.png"]->GetHeight() * 1.0f};
		action_button_diff = tex_map["Assets/SkillSelected.png"]->GetWidth() - tex_map["Assets/SkillUnselected.png"]->GetWidth();
		battle_state = BATTLE_STATE::CURR_TURN; 
	}

	void CombatModeOverlay::visibility(const Event& e)
	{
		OverlayOpen = event_cast<CombatModeVisibility>(e).isVisible;
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
						if (battle_state == BATTLE_STATE::CURR_TURN || battle_state == BATTLE_STATE::SELECT_ACTION || battle_state == BATTLE_STATE::SELECT_WEAPON)
						{
							bool first = true;
							float padding = 0.0f;
							for (auto id : instance.ecs.view<Components::Character>(exclude_t<tc::Destroyed>()))
							{
								// check if turn is over

								UI::CharacterTurn(instance, id, { 0.f, ImGui::GetCursorPosY() + padding }, first);
								padding += 100.0f;
								first = false;
							}
						}
					
						if (battle_state != BATTLE_STATE::SELECT_OTHER && battle_state != BATTLE_STATE::COMMENCE_BATTLE)
						{
							UI::CharacterTurnData(instance, curr_entity, { 0.f, viewport->Size.y - placeholder_height });

							if (UI::UIButton_2("More Information >", "More Information >", ImVec2{ action_background_size.x * 0.72f, viewport->Size.y - action_background_size.y * 1.2f }, { 0.f, 8.0f }, FONT_BODY))
							{
								display_curr_stat = true;
							}
						}
					}
					else
					{
						if (UI::UIButton_2("< Hide", "< Hide", ImVec2{ viewport->Size.x * 0.3f, viewport->Size.y * 0.15f }, { 0.f, 8.0f }, FONT_BODY))
						{
							display_curr_stat = false;
						}

						push_style_color();

						const float infoSize = viewport->Size.y * 0.8f / 3.0f;
						ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y * 0.2f});
						render_more_info(instance, *viewport, INFO_TYPE::CHAR, curr_entity);
						ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y * 0.2f + infoSize }); 
						render_more_info(instance, *viewport, INFO_TYPE::ACTIONS, curr_entity);
						ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y * 0.2f + infoSize * 2.0f});
						render_more_info(instance, *viewport, INFO_TYPE::WEAPONS, curr_entity);

						pop_style_color();
					}

					if (battle_state == BATTLE_STATE::CURR_TURN || battle_state == BATTLE_STATE::SELECT_ACTION || battle_state == BATTLE_STATE::SELECT_WEAPON)
					{
						UI::ActionUI(ImVec2{ viewport->Size.x, viewport->Size.y - action_background_size.y }, battle_state == BATTLE_STATE::SELECT_WEAPON ? "SELECT A WEAPON" : "SELECT AN ACTION");
						ImGui::SetCursorPos(ImVec2{ viewport->Size.x - action_background_size.x * 0.85f , viewport->Size.y - action_background_size.y * 0.7f });
						if (ImGui::BeginChild("Action content", ImVec2{ action_background_size.x * 0.85f, action_background_size.y * 0.7f }, true, ImGuiWindowFlags_NoScrollbar))
						{
							switch (battle_state)
							{
							case Tempest::CombatModeOverlay::BATTLE_STATE::CURR_TURN:
							{
								// action
								if (UI::UIButton_2("ACTION", "ACTION", ImVec2{ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.3f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.5f }, { -150,80 }, FONT_SHEAD))
								{
									battle_state = BATTLE_STATE::SELECT_ACTION;
								}

								ImGui::SameLine();
								const ImVec2 cursor{ ImGui::GetCursorPos() };
								const ImVec2 avail_region{ ImGui::GetContentRegionAvail() };
								if (UI::UIButton_2("ITEMS", "ITEMS", ImVec2{ cursor.x + avail_region.x * 0.5f, cursor.y - avail_region.y * 0.25f }, { -200,10 }, FONT_SHEAD))
								{

								}

								if (UI::UIButton_2("MOVE", "MOVE", ImVec2{ cursor.x + avail_region.x * 0.5f, cursor.y + avail_region.y * 0.6f }, { -200,10 }, FONT_SHEAD))
								{

								}
							}
							break;
							case Tempest::CombatModeOverlay::BATTLE_STATE::SELECT_ACTION: // TODO: account for no actions
							{
								auto& charac = instance.ecs.get<tc::Character>(curr_entity);
								unsigned i = 0;
								float xpos = ImGui::GetCursorPosX() + 60.0f;
								for (auto id : charac.actions)
								{
									auto& action = instance.ecs.get<tc::Graph>(id);

									ImGui::SetCursorPos(ImVec2{ selected_action == id ? xpos - action_button_diff : xpos, ImGui::GetCursorPosY() });
									if (UI::UIActionButton(action.g.name.c_str(), "##ACTIONSTUFF" + i++, selected_action == id))
									{
										selected_action = id;
										battle_state = BATTLE_STATE::SELECT_WEAPON;
									}
								}
							}
							break;
							case Tempest::CombatModeOverlay::BATTLE_STATE::SELECT_WEAPON: // TODO: account for no weapons
							{
								auto& charac = instance.ecs.get<tc::Character>(curr_entity);
								unsigned i = 0;
								float xpos = ImGui::GetCursorPosX() + 60.0f;
								for (auto id : charac.weapons)
								{
									auto& action = instance.ecs.get<tc::Weapon>(id);

									ImGui::SetCursorPos(ImVec2{ selected_weapon == id ? xpos - action_button_diff : xpos, ImGui::GetCursorPosY() });
									if (UI::UIActionButton(action.name.c_str(), "##WEAPONSTUFF" + i++, selected_weapon == id))
									{
										selected_weapon = id;
										battle_state = BATTLE_STATE::SELECT_OTHER;
									}
								}
							}
							break;
							default:
								break;
							}

						}
						ImGui::EndChild();
					}

					// display the back button at the top right
					if (battle_state == BATTLE_STATE::SELECT_ACTION || battle_state == BATTLE_STATE::SELECT_WEAPON)
					{
						if (UI::UIButton_2("Back", "Back", ImVec2{ viewport->Size.x * 0.92f, viewport->Size.y - action_background_size.y * 1.2f }, { 0,0 }, FONT_BODY))
						{
							battle_state = battle_state == BATTLE_STATE::SELECT_ACTION ? BATTLE_STATE::CURR_TURN : BATTLE_STATE::SELECT_ACTION;
						}
					}

					// display the end turn button
					else if (battle_state == BATTLE_STATE::CURR_TURN)
					{
					}

					switch (battle_state)
					{
					case Tempest::CombatModeOverlay::BATTLE_STATE::SELECT_OTHER:
					{
						// go to select the thing
						battle_state = BATTLE_STATE::BATTLE_GLIMPSE;
					}
					break;
					case Tempest::CombatModeOverlay::BATTLE_STATE::BATTLE_GLIMPSE:
					{
						// display other character
						if (!display_other_stat)
						{
							UI::CharacterTurnData(instance, other_entity, { viewport->Size.x, viewport->Size.y - placeholder_height }, true);
							if (UI::UIButton_2("< More Information", "< More Information", ImVec2{ viewport->Size.x - action_background_size.x * 0.6f, viewport->Size.y - action_background_size.y * 1.2f }, { 0.f, 8.0f }, FONT_BODY))
							{
								display_other_stat = true;
							}
						}

						else
						{
							if (UI::UIButton_2("Hide >", "Hide >", ImVec2{ viewport->Size.x * 0.7f, viewport->Size.y * 0.15f }, { 0.f, 8.0f }, FONT_BODY))
							{
								display_other_stat = false;
							}

							push_style_color();

							const float infoSize = viewport->Size.y * 0.8f / 3.0f;
							ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.7f, viewport->Size.y * 0.2f });
							render_more_info(instance, *viewport, INFO_TYPE::CHAR, other_entity);
							ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.7f, viewport->Size.y * 0.2f + infoSize });
							render_more_info(instance, *viewport, INFO_TYPE::ACTIONS, other_entity);
							ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.7f, viewport->Size.y * 0.2f + infoSize * 2.0f });
							render_more_info(instance, *viewport, INFO_TYPE::WEAPONS, other_entity);

							pop_style_color();
						}

						// display chance of success
						UI::AttackSuccessUI(instance.ecs.get<tc::Graph>(selected_action).g.name.c_str(), ImVec2{viewport->Size.x * 0.5f, viewport->Size.y * 0.72f },100);

						if (UI::UIButton_2("Confirm", "Confirm", ImVec2{ viewport->Size.x * 0.45f, viewport->Size.y * 0.8f }, { -60,0 }, FONT_BODY))
						{
							battle_state = BATTLE_STATE::COMMENCE_BATTLE;
							display_other_stat = false;
							display_curr_stat = false;
						}

						if (UI::UIButton_2("Cancel", "Cancel", ImVec2{ viewport->Size.x * 0.55f, viewport->Size.y * 0.8f }, { -60,0 }, FONT_BODY))
						{
							//battle_state = BATTLE_STATE::SELECT_OTHER;
							battle_state = BATTLE_STATE::CURR_TURN; // temp testing
						}
					}
					break;
					case Tempest::CombatModeOverlay::BATTLE_STATE::COMMENCE_BATTLE:
					{
						if (UI::CharacterTurnData(instance, curr_entity, { 0.f, viewport->Size.y - placeholder_height }, false, true))
						{
							// attacker roll
						}

						if (UI::CharacterTurnData(instance, other_entity, { viewport->Size.x, viewport->Size.y - placeholder_height }, true, true))
						{
							// defender roll
						}
						
						// draw the combat roll
						auto tex = tex_map["Assets/CombatRollBG.png"];
						ImVec2 point{ 0.0f,viewport->Size.y * 0.2f };
						{
							ImVec2 Min{ point.x, point.y };
							ImVec2 Max{ Min.x + viewport->Size.x , Min.y + viewport->Size.y * 0.3f};
							ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
						}

						auto& attacker = instance.ecs.get<tc::Character>(curr_entity);
						auto& defender = instance.ecs.get<tc::Character>(curr_entity);

						// display the character name and rolls
						ImGui::PushFont(FONT_HEAD);

						std::string roll = "34";
						ImGui::SetCursorPos(ImVec2{viewport->Size.x * 0.4f - ImGui::CalcTextSize(attacker.name.c_str()).x, viewport->Size.y * 0.27f });
						ImGui::Text(attacker.name.c_str());
						ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.4f - ImGui::CalcTextSize(roll.c_str()).x, viewport->Size.y * 0.35f });
						ImGui::Text(roll.c_str());

						roll = "9";
						ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.6f - ImGui::CalcTextSize(defender.name.c_str()).x, viewport->Size.y * 0.27f });
						ImGui::Text(defender.name.c_str());
						ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.6f - ImGui::CalcTextSize(roll.c_str()).x, viewport->Size.y * 0.35f });
						ImGui::Text(roll.c_str());
						ImGui::PopFont();

						// only trigger this if no more rolls
						if (UI::UIButton_2("Confirm", "Confirm", ImVec2{viewport->Size.x * 0.5f, viewport->Size.y * 0.55f}, { 0,0 }, FONT_BODY))
						{
							// TODO: affect the entities

							battle_state = BATTLE_STATE::CURR_TURN;

							//  TODO: change to the next entity turn
							selected_action = UNDEFINED;
							selected_weapon = UNDEFINED;
						}

					}
					break;
					default:
						break;
					}
				}
				

				if (UI::UIButton_2("Turn Order", "Turn Order", ImVec2{ viewport->Size.x * 0.8f, viewport->Size.y * 0.06f }, { 10.f,10.f }, FONT_PARA))
				{
					Service<EventManager>::Get().instant_dispatch<OpenTurnOrderOverlay>();
					Service<EventManager>::Get().instant_dispatch<CombatModeVisibility>(false);
				}

				if (UI::UIButton_2("Place Units", "Place Units", ImVec2{ viewport->Size.x * 0.9f, viewport->Size.y * 0.06f }, { 10.f,10.f }, FONT_PARA))
				{
				}
			}

            ImGui::End();

			ImGui::PopStyleVar();
            ImGui::PopStyleColor();
            
		}
	}

	void CombatModeOverlay::render_more_info(Instance& instance, const ImGuiViewport& viewport, INFO_TYPE type, Entity entity)
	{
		const int identifier = static_cast<int>(type) + static_cast<int>(entity * 2.5f); // this is a scam
		if (ImGui::BeginChild(std::string("Char more info" + std::to_string(identifier)).c_str(), ImVec2{ viewport.Size.x * 0.3f, viewport.Size.y * 0.8f / 3.0f }, true))
		{
			auto& charac = instance.ecs.get<tc::Character>(entity);

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

			if (ImGui::BeginChild(std::string("Internal content" + std::to_string(identifier)).c_str(), ImVec2{ ImGui::GetContentRegionAvailWidth() * 0.95f, ImGui::GetContentRegionAvail().y * 0.8f }, true))
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

	void CombatModeOverlay::push_style_color() const
	{
		const ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.f);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
		ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.06f,0.06f, 0.06f, 0.85f });
	}

	void  CombatModeOverlay::pop_style_color() const
	{
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(3);
	}
}