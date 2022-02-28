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
#include <Tempest/src/Audio/AudioEngine.h>
namespace Tempest
{
	float easyInBack(float x)
	{
		const float c1 = 1.3f;
		const float c3 = c1 + 1;

		return c3 * x * x * x - c1 * x * x;
	}


	const float left_time = .4f;
	const float both_time = .6f;


	void TurnOrderOverlay::open_popup(const Event& e)
	{
		auto a = event_cast<OpenTurnOrderOverlay>(e);
		new_instance = a.newInstance;
		OverlayOpen = true;
		change_state(TURN_ORDER_STATE::ORDER_ADD_UNITS);
		if (!new_instance)
			added_entities = a.entities;

		tutorial_index = 0;
	}

	void TurnOrderOverlay::change_state(TURN_ORDER_STATE state)
	{
		turn_order_state = state;
		inter_nest[0].start(-1.f, 0.f, both_time, 0.f, [](float x) { return glm::cubicEaseOut(x); });
	}

	void TurnOrderOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		{
			float dt = ImGui::GetIO().DeltaTime;
			for (auto& i : inter_nest)
				i.update(dt);
		}

		if (OverlayOpen)
		{
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 0.0f,0.0f,0.0f,0.8f});
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

			if (ImGui::Begin("Turn Order Overlay", nullptr, window_flags))
			{
				// just for testing
				if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
				{
					instance.tutorial_enable = true;
					instance.tutorial_level = 1;
					tutorial_index = 0;

				}

				if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_X)))
				{
					instance.tutorial_enable = true;
					tutorial_index++;
				}

				// tutorial progress
				if (instance.tutorial_enable && !instance.tutorial_temp_exit)
				{
					auto drawlist = ImGui::GetForegroundDrawList();
					//if (instance.tutorial_level != 1) //set Slide to false if not tut level 1
					//	instance.tutorial_slide = false;
					if (instance.tutorial_level == 1)
					{
						switch (tutorial_index)
						{
						case 0:
						{
							ImVec2 pos = { 0, viewport->Size.y * 0.275f };
							ImVec2 size = { 640.f, 290.f };
							UI::TutArea(pos, size);
							string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to add your units into the turn order";
							drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y * 0.5f}, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
						}
							break;

						case 1:
						{
							ImVec2 pos = { viewport->Size.x * 0.9f - 90.0f, viewport->Size.y * 0.9f - 24.0f};
							ImVec2 size = { 180.f, 50.f };
							UI::TutArea(pos, size);
							string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to go to the next page";
							drawlist->AddText({ pos.x - ImGui::CalcTextSize(str.c_str()).x - 5.0f, pos.y + 25.0f}, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
						}
						break;

						case 2:
						{
							ImVec2 pos = { viewport->Size.x * 0.221f, viewport->Size.y * 0.673f };
							ImVec2 size = { 220.f, 140.f };
							UI::TutArea(pos, size);
							string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to customize your turn order";
							drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
						}
						break;

						case 3:
						{
							ImVec2 pos = { viewport->Size.x * 0.28f, viewport->Size.y * 0.475f };
							ImVec2 size = { 105.f, 50.f };
							UI::TutArea(pos, size);
							string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to set turn order according to Move";
							drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
						}
						break;


						case 4:
						{
							// draw the other section 

							ImVec2 pos = { viewport->Size.x * 0.12f , viewport->Size.y * 0.7f };
							ImVec2 size = { 270.f, 110.f };
							UI::TutArea(pos, size);
							string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to set turn order according to Randomize + Move";
							drawlist->AddText({ pos.x + 140.f - ImGui::CalcTextSize(str.c_str()).x * 0.5f, pos.y - 20.0f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
						}
						break;

						case 5:
						{
							// draw the other section

							ImVec2 pos = { viewport->Size.x * 0.9f - 90.0f, viewport->Size.y * 0.9f - 24.0f };
							ImVec2 size = { 180.f, 50.f };
							UI::TutArea(pos, size);
							string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to save and return to the previous page";
							drawlist->AddText({ pos.x - ImGui::CalcTextSize(str.c_str()).x - 5.0f, pos.y + 25.0f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
						}
						break;

						case 6:
						{
							ImVec2 pos = { viewport->Size.x * 0.9f - 90.0f, viewport->Size.y * 0.9f - 24.0f };
							ImVec2 size = { 180.f, 50.f };
							UI::TutArea(pos, size);
							string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to save the turn order and start the game";
							drawlist->AddText({ pos.x - ImGui::CalcTextSize(str.c_str()).x - 5.0f, pos.y + 25.0f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
						}
						break;

						default:
							break;
						}
					}

					//Tutorial Exit Button
					if (instance.tutorial_slide == false)
					{
						auto exitBtn = tex_map["Assets/Tutorial_exit.dds"];
						ImVec2 tut_min = { viewport->Size.x * 0.85f, viewport->Size.y * 0.05f };
						ImVec2 tut_max = { tut_min.x + exitBtn->GetWidth() * 0.7f, tut_min.y + exitBtn->GetHeight() * 0.7f };
						drawlist->AddImage((void*)static_cast<size_t>(exitBtn->GetID()), tut_min, tut_max);

						if (UI::MouseIsWithin(tut_min, tut_max))
						{
							ImGui::SetMouseCursor(7);
							if (ImGui::IsMouseClicked(0))
							{
								instance.tutorial_temp_exit = true;
								ImGui::OpenPopup("TutorialExitPopupConfirm");
							}
						}
					}
				}

				// exit tutorial
				if (UI::ConfirmTutorialPopup("TutorialExitPopupConfirm", "Do you want to exit the tutorial?", true, [&]() {instance.tutorial_temp_exit = false; }))
				{
					instance.tutorial_temp_exit = false;
					instance.tutorial_enable = false;
				}

				// background
				auto texBG = tex_map["Assets/TurnOrderBG.dds"];
				{

					ImVec2 Min{ 0,0 };
					ImVec2 Max{ Min.x + viewport->Size.x, Min.y + viewport->Size.y };
					ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(texBG->GetID()), Min, Max);
				}

				// title
				ImGui::SetCursorPos(ImVec2{ 0,inter_nest[0].get() * 200.f });
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
					std::string text = "Select and add the units from the list that you want in the game.";
					ImGui::PushFont(FONT_BODY);
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.05f + inter_nest[0].get() * 800.f, viewport->Size.y * 0.25f });
					ImGui::Text(text.c_str());
					ImGui::PopFont();

					auto tex = tex_map["Assets/SelectedUnitsTitle.dds"];
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.695f - tex->GetWidth() * 0.5f - inter_nest[0].get() * 800.f, viewport->Size.y * 0.2f });
					ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 1.0f, tex->GetHeight() * 1.0f });

					// character selection section
					ImGui::SetCursorPos(ImVec2{ 0.f + inter_nest[0].get() * 1000.f, viewport->Size.y * 0.3f });
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

					next_button_name = "Next";
				}
					break;
				case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_TURN_MAIN:
				{
					auto tex = tex_map["Assets/TurnOrderLogo.dds"];
					ImGui::SetCursorPos(ImVec2{viewport->Size.x * 0.2f - tex->GetWidth() * 0.5f + (inter_nest[0].get() + inter_nest[1].get()) * 1000.f, viewport->Size.y * 0.30f - tex->GetHeight() * 0.5f });
					ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 1.0f,tex->GetHeight() * 1.0f });

					tex = tex_map["Assets/SelectedUnitsTitle.dds"];
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.695f - tex->GetWidth() * 0.5f - inter_nest[0].get() * 800.f, viewport->Size.y * 0.2f });
					ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 1.0f, tex->GetHeight() * 1.0f });

					ImGui::SetCursorPos(ImVec2{ 0 + (inter_nest[0].get() + inter_nest[1].get()) * 600.f, viewport->Size.y * 0.45f });
					ImGui::Dummy(ImVec2{ 50.f, 0.f });
					ImGui::SameLine();
					if (ImGui::BeginChild("TurnOrderText", ImVec2{ viewport->Size.x * 0.3f, viewport->Size.y * 0.35f}, true))
					{
						std::string text = "Turn order decides the order in which each entity is able to act in the game. This order can be decided in several ways depending on the mechanics of the tabletop RPG.\n\nEntities with the fastest turn order may have a significant advantage over others.";
						ImGui::PushFont(FONT_BODY);
						ImGui::TextWrapped(text.c_str());
						ImGui::PopFont();

						tex = tex_map["Assets/CustomizeTurnOrderLeft.dds"];
						ImGui::SetCursorPos(ImVec2{0,viewport->Size.y * 0.35f - tex->GetHeight() * 1.0f});
						ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.9f, tex->GetHeight() * 0.9f });

						tex = tex_map["Assets/CustomizeTurnOrderUnlit.dds"];
						ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.3f - tex->GetWidth() * 0.9f - 10.0f,viewport->Size.y * 0.35f - tex->GetHeight() * 1.0f - 5.0f});

						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });

						if (ImGui::ImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.9f, tex->GetHeight() * 0.9f }))
						{
							inter_nest[1].start(0.f, -1.f, left_time, 0.f, [](float x) { return easyInBack(x); });
							auto fn = [&]() {
								turn_order_state = TURN_ORDER_STATE::ORDER_TURN_SUB;
								inter_nest[1].start(-1.f, 0.f, left_time, 0.f, [](float x) { return glm::cubicEaseOut(x); });
							};

							Service<EventManager>::Get().instant_dispatch<DelayTrigger>(DelayTrigger(left_time, fn));
						}

						ImGui::PopStyleColor(3);


						ImGui::SetCursorPos(ImVec2{0, ImGui::GetContentRegionMax().y * 0.75f});
						ImGui::Dummy(ImVec2{ 5.f, 0.0f });
						if (ImGui::BeginChild("TurnOrderTextInside", ImVec2{ ImGui::GetContentRegionMax().x * 0.65f, 40.0f }, true))
						{
							std::string text = "Select from stats, randomized order or both for your unit's turn order.";
							ImGui::PushFont(FONT_BODY);
							ImGui::TextWrapped(text.c_str());
							ImGui::PopFont();
						}
						ImGui::EndChild();


					}
					ImGui::EndChild();
					
					next_button_name = "Save";
				}
					break;

				case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_TURN_SUB: // change to sub
				{
					auto tex = tex_map["Assets/TurnOrderLogo.dds"];
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.2f - tex->GetWidth() * 0.5f + (inter_nest[0].get() + inter_nest[1].get()) * 1000.f, viewport->Size.y * 0.30f - tex->GetHeight() * 0.5f });
					ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 1.0f,tex->GetHeight() * 1.0f });

					tex = tex_map["Assets/SelectedUnitsTitle.dds"];
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.695f - tex->GetWidth() * 0.5f - inter_nest[0].get() * 800.f, viewport->Size.y * 0.2f });
					ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 1.0f, tex->GetHeight() * 1.0f });

					ImGui::SetCursorPos(ImVec2{ 0 + (inter_nest[0].get() + inter_nest[1].get()) * 600.f, viewport->Size.y * 0.43f });
					ImGui::Dummy(ImVec2{ 50.f, 0.f });
					ImGui::SameLine();
					if (ImGui::BeginChild("TurnOrderText", ImVec2{ viewport->Size.x * 0.31f, viewport->Size.y * 0.38f }, true))
					{
						ImGui::PushFont(FONT_BODY);
						std::string text = "Select from stats, randomized order or both for your unit's turn order.";
						ImGui::Text(text.c_str());
						ImGui::PopFont();
						
						ImGui::Dummy(ImVec2{ 0.0f, 10.0f });
						auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
						Entity StateLineId = UNDEFINED;
						for (auto id : StatsView)
							StateLineId = id;
						auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);

						ImGui::SetCursorPos(ImVec2{ ImGui::GetCursorPosX() + 55.f , ImGui::GetCursorPosY() + 35.f });
						const ImVec2 cursor{ ImGui::GetCursorPos() };
						int col = 0; int row = 0;

						// sort by stats
						for (int i = 0; i < sl->size(); i++)
						{
							if ((*sl)(i))
							{
							//	bool is_selected = (current_stat == sl->operator[](i));
								if (UI::UIButton_2(sl->operator[](i).c_str(), sl->operator[](i).c_str(), 
									{ cursor.x + col++ * 120, cursor.y + row * 80 }, { -70, 5 }, FONT_PARA, current_stat == sl->operator[](i)))
								{
									if (current_stat == sl->operator[](i))
										current_stat = "NULL";

									else
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
								}

								// display in cols of 5
								if (col / 5)
								{
									col = 0;
									row++;
								}
							}
						}

						// randomize
						tex = tex_map["Assets/RandomButtonUnlit.dds"];
						ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.155f - tex->GetWidth() * 0.9f * 0.5f, viewport->Size.y * 0.38f - tex->GetHeight() * 1.0f - 10.0f});

						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });

						if (ImGui::ImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.9f, tex->GetHeight() * 0.9f }))
						{
							std::shuffle(added_entities.begin(), added_entities.end(), els::random::prng);
						}

						ImGui::PopStyleColor(3);
					}
					ImGui::EndChild();

					next_button_name = "Return";
				}
					break;
				default:
					break;
				}
				

				// character added section
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.47f - inter_nest[0].get() * 1000.f, viewport->Size.y * 0.25f });
				const ImVec2 ChildSize{ viewport->Size.x * 0.45f, viewport->Size.y * 0.5f };
				tvector<unsigned> remove;
				if (ImGui::BeginChild("Character added", ChildSize, true))
				{
					unsigned i = 0;
					unsigned counter = 0;
					float ypos = ImGui::GetCursorPosY();
					// TODO: loop through the added units here
					for ([[maybe_unused]]auto id : added_entities)
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + i++ * (ChildSize.x - (character_icon->GetWidth() + 2.0f) * 5) * 0.5f);
						ImGui::SetCursorPosY(ypos);
						ImGui::PushID(++counter);
						ImGui::BeginGroup();

						auto cs = instance.ecs.get_if<tc::Character>(id);
						if (turn_order_state == TURN_ORDER_STATE::ORDER_ADD_UNITS)
						{
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.06f,0.06f,0.06f,1.0f });

							auto PairResult = UI::UICharButton_WithDelete((void*)static_cast<size_t>(character_icon->GetID()), ImVec2{ character_icon->GetWidth() * 1.0f, character_icon->GetHeight() * 1.0f },
								cs->name.c_str(), string("##unitcombat" + std::to_string(id)), false, { 0,0 }, { 1,1 }, 2, ImVec4{ 0,0,0,0 }, ImVec4{ cs->color.x, cs->color.y, cs->color.z,1 });
						
							ImGui::PopStyleColor();

							if (PairResult.second)
							{
								ImGui::OpenPopup(string("RemoveUnitCombat##" + std::to_string(id)).c_str());
							}

							if (UI::ConfirmDeletePopup(string("RemoveUnitCombat##" + std::to_string(id)).c_str(), "Remove this unit?"))
							{
								remove.push_back(counter - 1);
							}
						}
						else 
						{
							ImGui::Dummy(ImVec2{ 0.0f, 20.0f });
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });

							ImGui::ImageButton((void*)static_cast<size_t>(character_icon->GetID()), ImVec2{ character_icon->GetWidth() * 1.0f, character_icon->GetHeight() * 1.0f },
								ImVec2{ 0,0 }, ImVec2{ 1,1 }, -1, ImVec4{ 0,0,0,0 }, ImVec4{ cs->color.x, cs->color.y,cs->color.z,1 });

							if (cs)
							{
								ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.06f,0.06f,0.06f,1.0f });
								ImGui::PushFont(FONT_BODY);
								ImGui::Text(cs->name.c_str());
								ImGui::PopFont();
								ImGui::PopStyleColor();
							}

							ImGui::PopStyleColor(3);
						}
						

						ImGui::EndGroup();
						ImGui::PopID();

						if (turn_order_state == TURN_ORDER_STATE::ORDER_TURN_SUB)
						{
							if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
							{
								ImGui::SetDragDropPayload("CUSTOM_TURN_ORDERING", &counter, sizeof(unsigned));
								ImGui::Text("Swap %s", cs->name.c_str());
								ImGui::EndDragDropSource();
							}

							if (ImGui::BeginDragDropTarget())
							{
								if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CUSTOM_TURN_ORDERING"))
								{
									unsigned payload_id = *(const unsigned*)payload->Data - 1;
									const auto temp_id = id;
									added_entities[counter - 1] = added_entities[payload_id];
									added_entities[payload_id] = temp_id;
								}
								ImGui::EndDragDropTarget();
							}
						}
						
						if (i / 5)
						{
							i = 0;
							ypos += character_icon->GetHeight() + (ChildSize.y - character_icon->GetHeight()  * 3) / 3.0f;
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
				if (UI::UIButton_2(next_button_name.c_str(), next_button_name.c_str(), ImVec2{ viewport->Size.x * 0.9f, viewport->Size.y * 0.9f }, {0,0}, FONT_PARA))
				{
					switch (turn_order_state)
					{
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_ADD_UNITS:
						if (added_entities.size() > 1)
						{
							inter_nest[0].start(0.f, -1.f, both_time, 0.f, [](float x) { return easyInBack(x); });

							auto fn = [&]() {
								change_state(TURN_ORDER_STATE::ORDER_TURN_MAIN);
							};
							Service<EventManager>::Get().instant_dispatch<DelayTrigger>(DelayTrigger(.6f, fn));

						}
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
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_TURN_SUB:
					{
						inter_nest[1].start(0.f, -1.f, left_time, 0.f, [](float x) { return easyInBack(x); });

						auto fn = [&]() {
							turn_order_state = TURN_ORDER_STATE::ORDER_TURN_MAIN;
							inter_nest[1].start(-1.f, 0.f, left_time, 0.f, [](float x) { return glm::cubicEaseOut(x); });
						};
						Service<EventManager>::Get().instant_dispatch<DelayTrigger>(DelayTrigger(left_time, fn));

					}
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
						ImGui::OpenPopup("TurnOrderGoBackConfirmation##");
						// TODO: Do a popup for comfirmation. Exit to Main menu screen if yes.

						// if (new_instance)
						// 	Service<EventManager>::Get().instant_dispatch<OpenConflictResTrigger>();
						// else
						// 	Service<EventManager>::Get().instant_dispatch<CombatModeVisibility>(true);
						// OverlayOpen = false;
						break;
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_TURN_MAIN:
					case Tempest::TurnOrderOverlay::TURN_ORDER_STATE::ORDER_TURN_SUB:
					{

						inter_nest[0].start(0.f, -1.f, both_time, 0.f, [](float x) { return easyInBack(x); });

						auto fn = [&]() {
							change_state(TURN_ORDER_STATE::ORDER_ADD_UNITS);
						};
						Service<EventManager>::Get().instant_dispatch<DelayTrigger>(DelayTrigger(.6f, fn));
					}
						break;
					default:
						break;
					}
				}

				if (UI::ConfirmDeletePopup("TurnOrderGoBackConfirmation##", new_instance ? "Go back to select conflict resolution?": "Go back to combat mode?"))
				{
					OverlayOpen = false;

					if (new_instance)
					{
						Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(
							instance.get_full_path(),
							MemoryStrategy{},
							InstanceType::EDIT_TIME);
						AudioEngine ae;
						ae.StopAllChannels();
						ae.Play("Sounds2D/CoReSyS_BGM1.wav", "BGM", 0.7f, true);
						Service<EventManager>::Get().instant_dispatch<OpenMainMenuTrigger>(2);
					}
						
					else
						Service<EventManager>::Get().instant_dispatch<CombatModeVisibility>(true);
					
				}

				ImGui::PopStyleColor(3);

			}

			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}
	}
}