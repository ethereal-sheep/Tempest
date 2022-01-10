/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "TurnOrderOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Triggers/Triggers.h"
#include <Tempest/src/Instance/EditTimeInstance.h>
#include <Editor/src/InstanceManager/InstanceConfig.h>
namespace Tempest
{
	void TurnOrderOverlay::open_popup(const Event& e)
	{
		auto a = event_cast<OpenTurnOrderOverlay>(e);
		new_instance = a.newInstance;
		OverlayOpen = true;
		turn_order_state = TURN_ORDER_STATE::ORDER_ADD_UNITS;
		if (!new_instance)
			added_entities = a.entities;
	}

	void TurnOrderOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		if (OverlayOpen)
		{
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 0.0f,0.0f,0.0f,0.8f});
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

			if (ImGui::Begin("Turn Order Overlay", nullptr, window_flags))
			{
				

				// title
				ImGui::SetCursorPos(ImVec2{ 0,0 });
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
				UI::SubHeader(turn_order_state == TURN_ORDER_STATE::ORDER_ADD_UNITS ? "Add Units" : "Turn Order");
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });

				// draw the divider
				ImVec2 winMin = { viewport->Size.x * 0.43f, viewport->Size.y * 0.25f };
				ImVec2 winMax = { viewport->Size.x * 0.4305f,viewport->Size.y * 0.85f };
				ImVec4 col = { 0.8f, 0.8f, 0.8f, 1.f };

				ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));

				switch (turn_order_state)
				{
				case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_ADD_UNITS:
				{
					// text displays
					std::string text = "Select and add the units from the list that you want you in the game.";
					ImGui::PushFont(FONT_BODY);
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.05f, viewport->Size.y * 0.25f });
					ImGui::Text(text.c_str());

					text = "Currently selected units";
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.695f - ImGui::CalcTextSize(text.c_str()).x * 0.5f, viewport->Size.y * 0.25f });
					ImGui::Text(text.c_str());
					ImGui::PopFont();

					// character selection section
					ImGui::SetCursorPos(ImVec2{ 0, viewport->Size.y * 0.3f });
					if (ImGui::BeginChild("Character adding", ImVec2{ viewport->Size.x * 0.35f, viewport->Size.y * 0.5f }))
					{
						unsigned i = 0;
						for (auto id : instance.ecs.view<Components::Character>())
						{
							auto& charac = instance.ecs.get<tc::Character>(id);

							if (charac.isInCombat)
								continue;
							if (UI::UICharTurnButton((void*)static_cast<size_t>(unit_black->GetID()), ImVec2{ unit_black->GetWidth() * 1.0f, unit_black->GetHeight() * 1.0f },
								charac.name.c_str(), "##turnordercharc" + std::to_string(i++), false, true, ImVec2{ 0,0 }, ImVec2{ 1,1 }, -1, ImVec4{ 0,0,0,0 }, ImVec4{ charac.color.x, charac.color.y,charac.color.z,1 }))
							{
								added_entities.emplace_back(id);
							}

							ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
						}
					}
					ImGui::EndChild();
				}
					break;
				case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_TURN_MAIN:
				{
					auto tex = tex_map["Assets/TurnOrderLogo.dds"];
					ImGui::SetCursorPos(ImVec2{viewport->Size.x * 0.2f - tex->GetWidth() * 0.5f, viewport->Size.y * 0.25f - tex->GetHeight() * 0.5f });
					ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 1.0f,tex->GetHeight() * 1.0f });

					ImGui::SetCursorPos(ImVec2{ 0, viewport->Size.y * 0.4f });
					ImGui::Dummy(ImVec2{ 20.f, 0.f });
					ImGui::SameLine();
					if (ImGui::BeginChild("TurnOrderText", ImVec2{ viewport->Size.x * 0.4f, viewport->Size.y * 0.5f }))
					{
						std::string text = "Turn order decides the order in which each entity is able to act in the game. This order can be decided in several ways depending on the mechanics of the tabletop RPG.\n\nEntities with the fastest turn order may have a significant advantage over others.";
						ImGui::TextWrapped(text.c_str());

						ImGui::Dummy(ImVec2{ 0.f, 100.0f });
						if (ImGui::ImageButton((void*)static_cast<size_t>(buttons[0]->GetID()), ImVec2{ buttons[0]->GetWidth() * 1.0f,buttons[0]->GetHeight() * 1.0f }))
						{
							turn_order_state = TURN_ORDER_STATE::ORDER_DICE;
						}
						if (ImGui::IsItemHovered())
							buttons[0] = tex_map["Assets/DiceRollButtonLit.dds"];
						else
							buttons[0] = tex_map["Assets/DiceRollButtonUnlit.dds"];

						ImVec2 cursor = ImGui::GetCursorPos();

						ImGui::SameLine();
						ImGui::Dummy(ImVec2{ 20.f, 0.f });
						ImGui::SameLine();

						if (ImGui::ImageButton((void*)static_cast<size_t>(buttons[2]->GetID()), ImVec2{ buttons[2]->GetWidth() * 1.0f,buttons[2]->GetHeight() * 1.0f }))
						{
							turn_order_state = TURN_ORDER_STATE::ORDER_STAT;
						}
						if (ImGui::IsItemHovered())
							buttons[2] = tex_map["Assets/StatsButtonLit.dds"];
						else
							buttons[2] = tex_map["Assets/StatsButtonUnlit.dds"];

						ImGui::SetCursorPos(cursor);
						ImGui::Dummy(ImVec2{ 0.f, 10.0f });
						if(ImGui::ImageButton((void*)static_cast<size_t>(buttons[1]->GetID()), ImVec2{ buttons[1]->GetWidth() * 1.0f,buttons[1]->GetHeight() * 1.0f }))
						{
							turn_order_state = TURN_ORDER_STATE::ORDER_DICE_STAT;
						}
						if (ImGui::IsItemHovered())
							buttons[1] = tex_map["Assets/DiceRollStatsButtonLit.dds"];
						else
							buttons[1] = tex_map["Assets/DiceRollStatsButtonUnlit.dds"];

						ImGui::SameLine();
						ImGui::Dummy(ImVec2{ 20.f, 0.f });
						ImGui::SameLine();

						if(ImGui::ImageButton((void*)static_cast<size_t>(buttons[3]->GetID()), ImVec2{ buttons[3]->GetWidth() * 1.0f,buttons[3]->GetHeight() * 1.0f }))
						{
							turn_order_state = TURN_ORDER_STATE::ORDER_CUSTOM;
						}
						if (ImGui::IsItemHovered())
							buttons[3] = tex_map["Assets/CustomButtonLit.dds"];
						else
							buttons[3] = tex_map["Assets/CustomButtonUnlit.dds"];
					}
					ImGui::EndChild();
					
				}
					break;
				case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_DICE:
				{
					auto tex = tex_map["Assets/DiceRollLogo.dds"];
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.2f - tex->GetWidth() * 0.5f, viewport->Size.y * 0.25f - tex->GetHeight() * 0.5f });
					ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 1.0f,tex->GetHeight() * 1.0f });

					ImGui::SetCursorPos(ImVec2{ 0, viewport->Size.y * 0.4f });
					ImGui::Dummy(ImVec2{ 20.f, 0.f });
					ImGui::SameLine();
					if (ImGui::BeginChild("TurnOrderText", ImVec2{ viewport->Size.x * 0.4f, viewport->Size.y * 0.5f }))
					{
						ImGui::PushFont(FONT_BODY);
						std::string text = "Select the dice that you want to use to decide the turn order of the units.";
						ImGui::TextWrapped(text.c_str());
						ImGui::PopFont();

						ImGui::Dummy(ImVec2{ 20.f, 100.0f });
						for (int i = 1; i < 6; ++i)
						{
							tex = tex_map["Assets/Dice_" + std::to_string(i) + ".dds"];
							ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 1.0f,tex->GetHeight() * 1.0f });
							ImGui::SameLine();
							ImGui::Dummy(ImVec2{ 10.f, 0.f });
							ImGui::SameLine();
						}
						

						if (UI::UIButton_2("Roll", "Roll", ImVec2{ viewport->Size.x * 0.2f, viewport->Size.y * 0.4f }, { 0,0 }, FONT_PARA))
						{
							std::shuffle(added_entities.begin(), added_entities.end(), els::random::prng);
						}
					}
					ImGui::EndChild();
				}
					break;
				case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_STAT:
				{
					auto tex = tex_map["Assets/StatsLogo.dds"];
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.2f - tex->GetWidth() * 0.5f, viewport->Size.y * 0.25f - tex->GetHeight() * 0.5f });
					ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 1.0f,tex->GetHeight() * 1.0f });

					ImGui::SetCursorPos(ImVec2{ 0, viewport->Size.y * 0.4f });
					ImGui::Dummy(ImVec2{ 20.f, 0.f });
					ImGui::SameLine();
					if (ImGui::BeginChild("TurnOrderText", ImVec2{ viewport->Size.x * 0.4f, viewport->Size.y * 0.5f }))
					{
						ImGui::PushFont(FONT_BODY);
						std::string text = "Select the stat that you want to use to decide the turn order of the units.";
						ImGui::TextWrapped(text.c_str());
						ImGui::PopFont();

						ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.2f - 100.0f,viewport->Size.y * 0.25f });
						auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
						Entity StateLineId = UNDEFINED;
						for (auto id : StatsView)
							StateLineId = id;
						auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);
						ImGui::PushItemWidth(200.0f);
						if (ImGui::BeginCombo("##combo", current_stat.c_str())) 
						{
							for (int i = 0; i < sl->size(); i++)
							{
								if ((*sl)(i))
								{
									bool is_selected = (current_stat == sl->operator[](i));
									if (ImGui::Selectable(sl->operator[](i).c_str(), is_selected))
									{
										current_stat = sl->operator[](i);
										
										// sort based on stat
										std::sort(added_entities.begin(), added_entities.end(), [i, &instance](const auto id1, const auto id2)
											{
												int first = 0, second = 0;
												if (tc::Character* cs = instance.ecs.get_if<tc::Character>(id1))
												{
													first = cs->get_stat(i);
												}
												if (tc::Character* cs = instance.ecs.get_if<tc::Character>(id2))
												{
													second = cs->get_stat(i);
												}

												return first > second;

											});

									}

									if (is_selected)
										ImGui::SetItemDefaultFocus();
								}
								
							}
							ImGui::EndCombo();
						}
						ImGui::PopItemWidth();
					}
					ImGui::EndChild();
				}
					break;
				case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_DICE_STAT:
				{
					auto tex = tex_map["Assets/CombinedLogo.dds"];
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.2f - tex->GetWidth() * 0.5f, viewport->Size.y * 0.25f - tex->GetHeight() * 0.5f });
					ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 1.0f,tex->GetHeight() * 1.0f });

					ImGui::SetCursorPos(ImVec2{ 0, viewport->Size.y * 0.4f });
					ImGui::Dummy(ImVec2{ 20.f, 0.f });
					ImGui::SameLine();
					if (ImGui::BeginChild("TurnOrderText", ImVec2{ viewport->Size.x * 0.4f, viewport->Size.y * 0.5f }))
					{
						ImGui::PushFont(FONT_BODY);
						std::string text = "Select the Dice roll and type of stat that you want to use to decide the turn order of the units.";
						ImGui::TextWrapped(text.c_str());
						ImGui::PopFont();


						ImGui::Dummy(ImVec2{ 20.f, 100.0f });
						for (int i = 1; i < 6; ++i)
						{
							tex = tex_map["Assets/Dice_" + std::to_string(i) + ".dds"];
							ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 1.0f,tex->GetHeight() * 1.0f });
							ImGui::SameLine();
							ImGui::Dummy(ImVec2{ 10.f, 0.f });
							ImGui::SameLine();
						}


						if (UI::UIButton_2("Roll", "Roll", ImVec2{ viewport->Size.x * 0.2f, viewport->Size.y * 0.4f }, { 0,0 }, FONT_PARA))
						{

						}
					}
					ImGui::EndChild();
				}
					break;
				case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_CUSTOM:
				{
					auto tex = tex_map["Assets/CustomLogo.dds"];
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.2f - tex->GetWidth() * 0.5f, viewport->Size.y * 0.25f - tex->GetHeight() * 0.5f });
					ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 1.0f,tex->GetHeight() * 1.0f });

					ImGui::SetCursorPos(ImVec2{ 0, viewport->Size.y * 0.4f });
					ImGui::Dummy(ImVec2{ 20.f, 0.f });
					ImGui::SameLine();
					if (ImGui::BeginChild("TurnOrderText", ImVec2{ viewport->Size.x * 0.4f, viewport->Size.y * 0.5f }))
					{
						ImGui::PushFont(FONT_BODY);
						std::string text = "Drag around the units in the grid to rearrange the their order.";
						ImGui::TextWrapped(text.c_str());
						ImGui::PopFont();
					}
					ImGui::EndChild();
				}
					break;
				default:
					break;
				}
				


				// character added section
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.47f, viewport->Size.y * 0.3f });
				const ImVec2 ChildSize{ viewport->Size.x * 0.45f, viewport->Size.y * 0.5f };
				tvector<unsigned> remove;
				if (ImGui::BeginChild("Character added", ChildSize))
				{
					unsigned i = 0;
					float ypos = ImGui::GetCursorPosY();
					// TODO: loop through the added units here
					for ([[maybe_unused]]auto id : added_entities)
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + i++ * (ChildSize.x - (character_icon->GetWidth() + 2.0f) * 5) * 0.5f);
						ImGui::SetCursorPosY(ypos);
						ImGui::PushID(i);
						ImGui::BeginGroup();

						auto cs = instance.ecs.get_if<tc::Character>(id);

						if (ImGui::ImageButton((void*)static_cast<size_t>(character_icon->GetID()), ImVec2{ character_icon->GetWidth() * 1.0f, character_icon->GetHeight() * 1.0f },
							ImVec2{ 0,0 }, ImVec2{ 1,1 }, -1, ImVec4{ 0,0,0,0 }, ImVec4{ cs->color.x, cs->color.y,cs->color.z,1 }))
						{
							if (turn_order_state == TURN_ORDER_STATE::ORDER_ADD_UNITS)
								remove.push_back(i-1);
						}
						if (cs)
						{
							ImGui::Text(cs->name.c_str());
						}

						ImGui::EndGroup();
						ImGui::PopID();

						if (turn_order_state == TURN_ORDER_STATE::ORDER_CUSTOM)
						{
							if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
							{
								ImGui::SetDragDropPayload("CUSTOM_TURN_ORDERING", &i, sizeof(unsigned));
								ImGui::Text("Swap %s", cs->name.c_str());
								ImGui::EndDragDropSource();
							}

							if (ImGui::BeginDragDropTarget())
							{
								if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CUSTOM_TURN_ORDERING"))
								{
									unsigned payload_id = *(const unsigned*)payload->Data - 1;
									const auto temp_id = id;
									added_entities[i - 1] = added_entities[payload_id];
									added_entities[payload_id] = temp_id;
								}
								ImGui::EndDragDropTarget();
							}
						}
						
						if (i / 5)
						{
							i = 0;
							ypos += character_icon->GetHeight() + (ChildSize.y - (character_icon->GetHeight() - 35.0f) * 3) / 3.0f;
						}
					}
				}

				if (turn_order_state == TURN_ORDER_STATE::ORDER_ADD_UNITS)
				{
					for (auto i : remove)
					{
						added_entities.erase(std::begin(added_entities) + i);
					}
				}

				remove.clear();


				ImGui::EndChild();

				// back button (get rid of this first
			/*	ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f,viewport->Size.y * 0.03f });
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
				auto tex = tex_map["Assets/BackMenuBtn.png"];

				if (ImGui::ImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }))
				{
					OverlayOpen = false;
					Service<EventManager>::Get().instant_dispatch<CombatModeVisibility>(true);
				}

				ImGui::PopStyleColor(3);*/

				// save
				if (UI::UIButton_2("Next", "Next", ImVec2{ viewport->Size.x * 0.9f, viewport->Size.y * 0.9f }, {0,0}, FONT_PARA))
				{
					switch (turn_order_state)
					{
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_ADD_UNITS:
						if(added_entities.size() > 1)
							turn_order_state = TURN_ORDER_STATE::ORDER_TURN_MAIN;
						else
							Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("At least two units required!");
						break;
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_TURN_MAIN:
						// change the state
						OverlayOpen = false;
						if (!new_instance)
							Service<EventManager>::Get().instant_dispatch<ChangeTurnOrder>(added_entities);

						Service<EventManager>::Get().instant_dispatch<OpenPlaceUnitsOverlay>(added_entities, instance, new_instance);
						break;
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_DICE:
						turn_order_state = TURN_ORDER_STATE::ORDER_TURN_MAIN;
						break;
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_STAT:
						turn_order_state = TURN_ORDER_STATE::ORDER_TURN_MAIN;
						break;
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_DICE_STAT:
						turn_order_state = TURN_ORDER_STATE::ORDER_TURN_MAIN;
						break;
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_CUSTOM:
						turn_order_state = TURN_ORDER_STATE::ORDER_TURN_MAIN;
						break;
					default:
						break;
					}
					
				}

				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f,viewport->Size.y * 0.025f });
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
				auto image = tex_map["Assets/BackMenuBtn.dds"];
				ImVec4 btnTintHover = { 0.922f,0.922f,0.922f,1.f };
				ImVec4 btnTintPressed = { 0.768f, 0.768f, 0.768f, 1.f };
				if (UI::UIImageButton((void*)static_cast<size_t>(image->GetID()), ImVec2{ image->GetWidth() * 0.7f, image->GetHeight() * 0.7f }, { 0, 0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
				{
					//Service<EventManager>::Get().instant_dispatch<OpenBuildModeOverlay>();
					switch (turn_order_state)
					{
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_ADD_UNITS:
						Service<EventManager>::Get().instant_dispatch<OpenConflictResTrigger>();
						OverlayOpen = false;
						//turn_order_state = TURN_ORDER_STATE::ORDER_ADD_UNITS;
						break;
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_TURN_MAIN:
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_DICE:
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_STAT:
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_DICE_STAT:
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_CUSTOM:
						turn_order_state = TURN_ORDER_STATE::ORDER_ADD_UNITS;
						break;
					default:
						break;
					}
				}
				ImGui::PopStyleColor(3);

			}

			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}
	}
}