/**********************************************************************************
* \author		Huang Xurong(h.xurong@digipen.edu),
				Lim Ziyi Jean(ziyijean.lim@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "CombatModeOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "AttackSystemOverlay.h"
#include "Util/UIElements.h"
#include "Instance/EditTimeInstance.h"
#include <Tempest/src/Audio/AudioEngine.h>

#include "Particles/ParticleSystem_2D.h"

namespace Tempest
{
	const float indent = 30.f;

	void AttackSystemOverlay::open_popup(const Event& e)
	{
		auto a = event_cast<OpenGraphTrigger>(e);
		OverlayOpen = true;
		type = a.type;

		id = UNDEFINED;

		temp_graph.clear();
		temp_graph.name = "";

		if (a.instance.ecs.has<tc::Graph>(a.id))
		{
			id = a.id;
			temp_graph = a.instance.ecs.get<tc::Graph>(id).g;
		}
		else
		{
			if (type == OPEN_GRAPH_TYPE::GRAPH_ACTION)
			{
				if(auto thisGraph = a.instance.ecs.view_first<tc::ActionGraph>(exclude_t<tc::Destroyed>()))
				{
					id = thisGraph;
					temp_graph = a.instance.ecs.get<tc::Graph>(id).g;
				}
			}
			else
			{
				if (auto thisGraph = a.instance.ecs.view_first<tc::ConflictGraph>(exclude_t<tc::Destroyed>()))
				{
					id = thisGraph;
					temp_graph = a.instance.ecs.get<tc::Graph>(id).g;
				}
			}
		}

		if (a.type == OPEN_GRAPH_TYPE::GRAPH_ACTION)
		{
			overlay_title = "Editing Action";
			sidebar_title = "ACTIONS";
		}

		else
		{
			overlay_title = "Editing Sequence";
			sidebar_title = "SEQUENCES";
		}
		tutorial_index = 0;
		particle_0 = false;
		particle_2 = false;
		particle_3 = false;
		particle_4 = false;
		tut_openSlide = true;
		ax::NodeEditor::NavigateToContent();
		inter.start(-0.1f, 0.02f, .25f, 0, [](float x) { return glm::cubicEaseOut(x); }); // back
		inter_nest[0].start(0.5f, .15f, .4f, 0, [](float x) { return glm::cubicEaseOut(x); }); // graphs 
	}

	void AttackSystemOverlay::close_popup(const Event& e)
	{
		auto a = event_cast<CloseOverlayTrigger>(e);
		if (a.current == QUICKMENU_POPUP_TYPE::ACTIONS ||
			a.current == QUICKMENU_POPUP_TYPE::SEQUENCES)
			OverlayOpen = false;
	}

	void AttackSystemOverlay::force_close(const Event&)
	{
		OverlayOpen = false;
		attack_action = false;
		defend_action = false;
		id = UNDEFINED;
	}



	void AttackSystemOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		{
			float dt = ImGui::GetIO().DeltaTime;
			/*
			for (auto& i : inter_nest)
				i.update(dt);*/

			inter.update(dt);
		}

		if (OverlayOpen)
		{
			if (ImGui::Begin("Attack System", nullptr, window_flags))
			{

				const float title_top_padding = 40.f;
				const float title_left_padding = 40.f;
				const float title_bottom_padding = 10.f;
				const float graph_context_height = viewport->Size.y * 0.95f;

				auto tex = tex_map["Assets/GraphBG.dds"];
				{

					ImVec2 Min{ 0,0 };
					ImVec2 Max{ Min.x + viewport->Size.x, Min.y + viewport->Size.y };
					ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
				}


				// draw the context
				ImGui::PushStyleColor(ImGuiCol_Border, { 0,0,0,0 });
				draw_context(instance, graph_context_height);
				ImGui::PopStyleColor();
				//ImGui::Separator();

				// title
				ImGui::SetCursorPos(ImVec2{ 0,0 });
				ImGui::Dummy(ImVec2{ 0.f, viewport->Size.y * 0.05f });
				UI::SubHeader(overlay_title.c_str());
				ImGui::Dummy(ImVec2{ 0.f, viewport->Size.y * 0.05f });

				// side bar
				const ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };
				ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.f);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
				ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.06f,0.06f, 0.06f, 0.85f });


				// graph name (got bug, first letter always hidden when entering new name)
				if (id && instance.ecs.has<tc::Graph>(id))
				{
					ImGui::PushFont(FONT_HEAD);
					ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
					ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.f,0.f,0.f, 0.f });
					const ImVec2 text_size{ ImGui::CalcTextSize(temp_graph.name.c_str()) };
					ImGui::SetCursorPos(ImVec2{viewport->Size.x * 0.8f - text_size.x * 0.5f, viewport->Size.y * 0.1f });
					if (ImGui::BeginChild("Editing name", ImVec2{ text_size.x * 2.0f, text_size.y + 5.0f}, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar))
					{
						ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0,0,0 });
						ImGui::PushItemWidth(text_size.x * 2.0f);
						ImGui::InputText("##testing", &temp_graph.name);
						ImGui::PopItemWidth();
						ImGui::PopStyleColor();
					}
					ImGui::EndChild();
					ImGui::PopStyleColor();
					ImGui::PopStyleVar();
					ImGui::PopFont();

					// render the category
					if (type == OPEN_GRAPH_TYPE::GRAPH_ACTION)
					{
						ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
						ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.f,0.f,0.f, 0.f });
						auto& action_graph = instance.ecs.get<tc::ActionGraph>(id);
						ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.8f , viewport->Size.y * 0.15f });
						if (ImGui::BeginChild("Category edit", ImVec2{ 150.0f, 80.0f}, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar))
						{
							attack_action = action_graph.category & tc::ActionGraph::AC_ATTK;
							defend_action = action_graph.category & tc::ActionGraph::AC_DEF;

							ImGui::SameLine();
							ImGui::PushFont(FONT_BODY);
							ImGui::Text("Attack");
							ImGui::SameLine();
							if(UI::UICheckBox_1("##attack_cat", &attack_action))
								(int&)action_graph.category ^= tc::ActionGraph::ACTION_CAT::AC_ATTK;

							ImGui::Dummy(ImVec2{ 0.f,5.f });

							ImGui::Text("Defend");
							ImGui::SameLine();
							if (UI::UICheckBox_1("##defend_cat", &defend_action))
								(int&)action_graph.category ^= tc::ActionGraph::ACTION_CAT::AC_DEF;
						
							ImGui::PopFont();
						}
						ImGui::EndChild();
						ImGui::PopStyleColor();
						ImGui::PopStyleVar();
			
					}
				}

				const ImVec2 ChildSize{ viewport->Size.x * 0.15f, viewport->Size.y * 0.75f };
				ImGui::SetCursorPos(ImVec2{ 0, viewport->Size.y * 0.5f - ChildSize.y * 0.5f});
				if (ImGui::BeginChild("Editing graph", ChildSize, true))
				{
					ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
					ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 5.f };
					ImVec2 winMax = { winMin.x + ImGui::GetWindowWidth() * 0.35f, winMin.y + ImGui::GetWindowHeight() * 0.05f };
					ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
					ImVec4 textcol = { 0,0,0,1 };
					if (!ImGui::IsWindowFocused())
					{
						col = { 0.980f, 0.768f, 0.509f, 0.7f };
						textcol = { 0,0,0,0.7f };
					}

					ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
					ImGui::PushFont(FONT_OPEN);
					ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), sidebar_title.c_str());
					ImGui::PopFont();

					ImGui::Dummy(ImVec2{0.f,25.0f});
					ImVec2 cursor{ ImGui::GetCursorPosX() + 120.0f, ImGui::GetCursorPosY() + 20.0f };

					// render the buttons here
					if (ImGui::BeginChild("Graph content", ImVec2{ ImGui::GetContentRegionAvailWidth() * 1.0f, ImGui::GetContentRegionAvail().y * 0.92f}, false))
					{
						if (id != UNDEFINED)
							instance.ecs.get<tc::Graph>(id).g = temp_graph;

						unsigned i = 0;
						if (type == OPEN_GRAPH_TYPE::GRAPH_ACTION)
						{
							for (auto current_graph : instance.ecs.view<tc::ActionGraph>(exclude_t<tc::Destroyed>()))
							{
								auto& action = instance.ecs.get<tc::Graph>(current_graph);
								auto PairResult = UI::UIButtonWithDelete(action.g.name, string("##Actiongraph" + std::to_string(i)), { cursor.x, cursor.y + i * 80 }, { 15, 20 }, FONT_PARA, id == current_graph);
								if (PairResult.first)
								{
									id = current_graph;
									temp_graph = action.g;
								}

								if (PairResult.second)
								{
									ImGui::OpenPopup(string("DeleteAction##" + std::to_string(i)).c_str());
								}

								if (UI::ConfirmDeletePopup(string("DeleteAction##" + std::to_string(i)).c_str(), "Delete this action?"))
								{
									instance.ecs.emplace<tc::Destroyed>(current_graph);
									if (current_graph == id)
									{
										id = UNDEFINED;
										temp_graph.clear();
										temp_graph.name = "";
									}
								}

								++i;
							}
						}

						else
						{
							for (auto current_graph : instance.ecs.view<tc::ConflictGraph>(exclude_t<tc::Destroyed>()))
							{
								auto& sequence = instance.ecs.get<tc::Graph>(current_graph);
								auto PairResult = UI::UIButtonWithDelete(sequence.g.name, string("##Sequencegraph" + std::to_string(i)), { cursor.x, cursor.y + i * 80 }, { 15, 20 }, FONT_PARA, id == current_graph);
								if (PairResult.first)
								{
									id = current_graph;
									temp_graph = sequence.g;
								}
								if (PairResult.second)
								{
									ImGui::OpenPopup(string("DeleteSequence##" + std::to_string(i)).c_str());
								}

								if (UI::ConfirmDeletePopup(string("DeleteSequence##" + std::to_string(i)).c_str(), "Delete this sequence?"))
								{
									instance.ecs.emplace<tc::Destroyed>(current_graph);
									if (current_graph == id)
									{
										id = UNDEFINED;
										temp_graph.clear();
										temp_graph.name = "";
									}
								}

								++i;
							}
						}

						// create new graphs
						if (UI::UIButton_1("+", "+", { cursor.x , cursor.y + i * 80 }, { 150,-10 }, FONT_HEAD))
						{
							auto new_graph = instance.ecs.create();

							if (type == OPEN_GRAPH_TYPE::GRAPH_ACTION)
							{
								instance.ecs.emplace<tc::ActionGraph>(new_graph);
								auto g = instance.ecs.emplace<tc::Graph>(new_graph, "ACTION", graph_type::action);
								auto p = tpath("Graphs") / "ACTION.json";
								if (fs::exists(p))
								{
									g->g = graph(p);
									LOG_INFO("Loaded Default Action");
								}
							}
							else
							{
								instance.ecs.emplace<tc::ConflictGraph>(new_graph);
								auto g = instance.ecs.emplace<tc::Graph>(new_graph, "SEQUENCE", graph_type::conflict);
								auto p = tpath("Graphs") / "SEQUENCE.json";
								if (fs::exists(p))
								{
									g->g = graph(p);
									LOG_INFO("Loaded Default Sequence");
								}

							}

							//Tutorial progression
							if (instance.tutorial_enable && tutorial_index == 0)
								tutorial_index = 1;

							id = new_graph;
							temp_graph = instance.ecs.get<tc::Graph>(id).g;
						}

						if (!ImGui::GetHoveredID())
							HoveredID = 0;
						else if (HoveredID != ImGui::GetHoveredID())
						{
							AudioEngine ae;
							ae.Play("Sounds2D/Button_Highlight.wav", "SFX", 0.8f);
							HoveredID = ImGui::GetHoveredID();
						}
					}

					ImGui::EndChild();
				}
				ImGui::EndChild();

				ImGui::PopStyleVar(3);
				ImGui::PopStyleColor(2);

				// display top buttons (why doesn't this work w simulate -- add the close graph triggger
				{
					ImGui::SetCursorPos(ImVec2{ 0,0 });
					if (ImGui::BeginChild("Top Buttons", ImVec2{ 470.0f, 100.0f }, false))
					{
						ImGui::SetCursorPos(ImVec2{ viewport->Size.x * inter.get(),viewport->Size.y * 0.03f });
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
						ImVec4 btnTintHover = { 0.922f,0.922f,0.922f,1.f };
						ImVec4 btnTintPressed = { 0.768f, 0.768f, 0.768f, 1.f };
						tex = tex_map["Assets/BackMenuBtn.dds"];

						if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
						{
							OverlayOpen = false;
							//Service<EventManager>::Get().instant_dispatch<OpenMainMenuTrigger>(3);
							Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>(instance);
						}

						ImGui::SameLine();
						ImGui::Dummy(ImVec2{ 10.0f, 0.0f });
						ImGui::SameLine();

						tex = tex_map["Assets/QuickMenuBtn.dds"];

						if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
						{
							QUICKMENU_POPUP_TYPE t = type == OPEN_GRAPH_TYPE::GRAPH_ACTION ? QUICKMENU_POPUP_TYPE::ACTIONS : QUICKMENU_POPUP_TYPE::SEQUENCES;
							Service<EventManager>::Get().instant_dispatch<QuickMenuPopupTrigger>(t);
							if (instance.tutorial_enable && tutorial_index == 2)
								tutorial_index = 3;
						}

						ImGui::SameLine();
						ImGui::Dummy(ImVec2{ 10.0f, 0.0f });
						ImGui::SameLine();

						tex = tex_map["Assets/TutorialBtn.dds"];
						if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
						{
							
							if(type == OPEN_GRAPH_TYPE::GRAPH_ACTION)
								Service<EventManager>::Get().instant_dispatch<TutorialPopupTrigger>(TUTORIAL_POPUP_TYPES::GRAPH_ACTION_TUT);
							else if (type == OPEN_GRAPH_TYPE::GRAPH_SEQUENCE)
								Service<EventManager>::Get().instant_dispatch<TutorialPopupTrigger>(TUTORIAL_POPUP_TYPES::GRAPH_SEQUENCE_TUT);
						}

						ImGui::PopStyleColor(3);
					}
					ImGui::EndChild();
				}
			
				// exit tutorial
				if (UI::ConfirmTutorialPopup("TutorialExitPopupConfirm", "Do you want to exit the tutorial?", true, [&]() {instance.tutorial_temp_exit = false;}))
				{
					instance.tutorial_temp_exit = false;
					instance.tutorial_enable = false;
				}

				// tutorial progrss
				if (instance.tutorial_enable && !instance.tutorial_temp_exit)
				{
					auto drawlist = ImGui::GetForegroundDrawList();
					if (instance.tutorial_level != 1) //set Slide to false if not tut level 1
						instance.tutorial_slide = false;

					if (instance.tutorial_level == 1)
					{
						if (instance.tutorial_slide && type == OPEN_GRAPH_TYPE::GRAPH_ACTION && tut_openSlide)
						{
							tut_openSlide = false;
							Service<EventManager>::Get().instant_dispatch<TutorialPopupTrigger>(TUTORIAL_POPUP_TYPES::GRAPH_ACTION_TUT);
						}		
						else if (type == OPEN_GRAPH_TYPE::GRAPH_ACTION && instance.tutorial_slide == false)
						{
							switch (tutorial_index) // Action 
							{
							case 0:
							{
								ImVec2 pos = { viewport->Size.x * 0.01f, viewport->Size.y * 0.165f };
								ImVec2 size = { 200.f, 70.f };
								UI::TutArea(pos, size, false);
								string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to create a new action.";
								drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								if (particle_0 == false)
								{
									glm::vec2 real_buttonSize;
									real_buttonSize.x = size.x;
									real_buttonSize.y = size.y;

									glm::vec2 real_mousePosition;
									real_mousePosition.x = pos.x;
									real_mousePosition.y = pos.y;

									particle_0 = true;
									if (!m_waypointEmitter)
										m_waypointEmitter = ParticleSystem_2D::GetInstance().ButtonEmitter(real_mousePosition, real_buttonSize);
									else
										ParticleSystem_2D::GetInstance().ReuseButtonEmitter(m_waypointEmitter, real_mousePosition, real_buttonSize);
								}
							}
							break;

							case 1:
							{
								if (m_waypointEmitter)
									m_waypointEmitter->m_GM.m_active = false;

								// Task List
								string str = "";
								auto selected = tex_map["Assets/Selected.dds"];
								auto unselected = tex_map["Assets/Unselected.dds"];
								bool taskCompleted = true;
								float xPos = viewport->Size.x * 0.75f;
								str = string(ICON_FK_EXCLAMATION_CIRCLE);
								ImGui::PushFont(FONT_HEAD);
								drawlist->AddText({ xPos, viewport->Size.y * 0.4f }, ImGui::GetColorU32({ 1.f,1.f,1.f,1 }), str.c_str());
								str = " Tasks";
								drawlist->AddText({ xPos + ImGui::GetFontSize(), viewport->Size.y * 0.4f }, ImGui::GetColorU32({ 0.98f,0.768f,0.51f,1 }), str.c_str());
								drawlist->AddLine({ xPos, viewport->Size.y * 0.4f + ImGui::GetFontSize() }, { viewport->Size.x, viewport->Size.y * 0.4f + ImGui::GetFontSize() }, ImGui::GetColorU32({ 1,1,1,1 }), 2.f);
								ImGui::PopFont();

								ImGui::PushFont(FONT_BODY);
								ImVec2 min = { xPos, viewport->Size.y * 0.45f };
								str = "Rename the action";
								if (temp_graph.name != "ACTION")
								{
									drawlist->AddImage((void*)static_cast<size_t>(selected->GetID()), min, { min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f });
									taskCompleted &= true;
								}
								else
								{
									drawlist->AddImage((void*)static_cast<size_t>(unselected->GetID()), min, { min.x + (float)unselected->GetWidth() * 0.6f, min.y + (float)unselected->GetHeight() * 0.6f });
									taskCompleted &= false;
								}
								drawlist->AddText({ xPos + selected->GetWidth() * 0.7f , min.y + (float)unselected->GetHeight() * 0.2f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								min = { min.x, min.y + unselected->GetWidth() * 0.9f };
								str = "Create a 'Get Attack' Node (Attacker)";
								auto action_lambda1 = [&]() {
									for (auto const& this_node : temp_graph.get_nodes())
									{
										if (this_node.second->get_type_string() == "GetStat:1")
											return true;
									}
									return false;
								};

								if (action_lambda1())
								{
									drawlist->AddImage((void*)static_cast<size_t>(selected->GetID()), min, { min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f });
									taskCompleted &= true;
								}
								else
								{
									drawlist->AddImage((void*)static_cast<size_t>(unselected->GetID()), min, { min.x + (float)unselected->GetWidth() * 0.6f, min.y + (float)unselected->GetHeight() * 0.6f });
									taskCompleted &= false;
								}
								drawlist->AddText({ xPos + selected->GetWidth() * 0.7f, min.y + (float)unselected->GetHeight() * 0.2f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								min = { min.x, min.y + unselected->GetWidth() * 0.9f };
								str = "Connect the 'Get Attack' node to the D6 node";

								auto action_lambda2 = [&]() {
									std::pair<pin_id_t, pin_id_t> pins;
									for (auto const& this_node : temp_graph.get_nodes())
									{
										if (this_node.second->get_type_string() == "GetStat:1")
										{
											pins.first = this_node.second->get_output_pin(0)->get_id();
										}
										if (this_node.second->get_name() == "D6")
										{
											pins.second = this_node.second->get_input_pin(0)->get_id();
										}
									}

									for (auto const& this_link : temp_graph.get_links())
									{
										if (this_link == pins)
											return true;
									}
									return false;
								};
								if (action_lambda2())
								{
									drawlist->AddImage((void*)static_cast<size_t>(selected->GetID()), min, { min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f });
									taskCompleted &= true;
								}
								else
								{
									drawlist->AddImage((void*)static_cast<size_t>(unselected->GetID()), min, { min.x + (float)unselected->GetWidth() * 0.6f, min.y + (float)unselected->GetHeight() * 0.6f });
									taskCompleted &= false;
								}
								drawlist->AddText({ xPos + selected->GetWidth() * 0.7f , min.y + (float)unselected->GetHeight() * 0.2f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
								ImGui::PopFont();

								auto nextBtn = tex_map["Assets/NextBtn.dds"];
								ImVec2 tut_min = { min.x, min.y + unselected->GetWidth() * 0.9f };
								ImVec2 tut_max = { tut_min.x + nextBtn->GetWidth() * 1.f, tut_min.y + nextBtn->GetHeight() * 1.f };

								if (taskCompleted)
								{
									drawlist->AddImage((void*)static_cast<size_t>(nextBtn->GetID()), tut_min, tut_max);

									if (UI::MouseIsWithin(tut_min, tut_max))
									{
										ImGui::SetMouseCursor(7);
										if (ImGui::IsMouseClicked(0))
											tutorial_index = 2;
									}
								}
								else
									drawlist->AddImage((void*)static_cast<size_t>(nextBtn->GetID()), tut_min, tut_max, { 0,0 }, { 1,1 }, ImGui::GetColorU32({ 1,1,1,0.4f }));
							}
							break;

							case 2:
							{
								ImVec2 pos = { viewport->Size.x * 0.102f, viewport->Size.y * 0.0261f };
								ImVec2 size = { 200.f, 50.f };
								UI::TutArea(pos, size, false);
								string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to access the quick menu.";
								drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								if (particle_2 == false)
								{
									glm::vec2 real_buttonSize;
									real_buttonSize.x = size.x;
									real_buttonSize.y = size.y;

									glm::vec2 real_mousePosition;
									real_mousePosition.x = pos.x;
									real_mousePosition.y = pos.y;

									if (!m_waypointEmitter)
										m_waypointEmitter = ParticleSystem_2D::GetInstance().ButtonEmitter(real_mousePosition, real_buttonSize);
									else
										ParticleSystem_2D::GetInstance().ReuseButtonEmitter(m_waypointEmitter, real_mousePosition, real_buttonSize);

									particle_2 = true;
								}
							}
							break;

							case 3:
							{
								ImVec2 pos = { viewport->Size.x * 0.505f, viewport->Size.y * 0.1f };
								ImVec2 size = { 310.f, 140.f };
								UI::TutArea(pos, size, false);
								string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to access the sequence page.";
								drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								if (particle_3 == false)
								{
									glm::vec2 real_buttonSize;
									real_buttonSize.x = size.x;
									real_buttonSize.y = size.y;

									glm::vec2 real_mousePosition;
									real_mousePosition.x = pos.x;
									real_mousePosition.y = pos.y;

									if (!m_waypointEmitter)
										m_waypointEmitter = ParticleSystem_2D::GetInstance().ButtonEmitter(real_mousePosition, real_buttonSize);
									else
										ParticleSystem_2D::GetInstance().ReuseButtonEmitter(m_waypointEmitter, real_mousePosition, real_buttonSize);

									particle_3 = true;
								}
							}
							break;

							}

							UI::TutProgressBar(drawlist, ImVec2{ viewport->Size }, 3);

						}
						else if (instance.tutorial_slide && type == OPEN_GRAPH_TYPE::GRAPH_SEQUENCE && tut_openSlide)
						{
							tut_openSlide = false;
							if(m_waypointEmitter)
								m_waypointEmitter->m_GM.m_active = false;
							Service<EventManager>::Get().instant_dispatch<TutorialPopupTrigger>(TUTORIAL_POPUP_TYPES::GRAPH_SEQUENCE_TUT);
						}
						else if(instance.tutorial_slide == false)// Sequence 
						{
							switch (tutorial_index)
							{
							case 0:
							{
								ImVec2 pos = { viewport->Size.x * 0.01f, viewport->Size.y * 0.165f };
								ImVec2 size = { 200.f, 70.f };
								UI::TutArea(pos, size);
								string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to create a new sequence.";
								drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
								if (particle_4 == false)
								{
									glm::vec2 real_buttonSize;
									real_buttonSize.x = size.x;
									real_buttonSize.y = size.y;

									glm::vec2 real_mousePosition;
									real_mousePosition.x = pos.x;
									real_mousePosition.y = pos.y;

									if (!m_waypointEmitter)
										m_waypointEmitter = ParticleSystem_2D::GetInstance().ButtonEmitter(real_mousePosition, real_buttonSize);
									else
										ParticleSystem_2D::GetInstance().ReuseButtonEmitter(m_waypointEmitter, real_mousePosition, real_buttonSize);

									particle_4 = true;
								}
							}
							break;

							case 1:
							{
								//Task List
								m_waypointEmitter->m_GM.m_active = false;
								auto selected = tex_map["Assets/Selected.dds"];
								auto unselected = tex_map["Assets/Unselected.dds"];
								bool taskCompleted = true;
								string str = "";
								str = string(ICON_FK_EXCLAMATION_CIRCLE);
								ImGui::PushFont(FONT_HEAD);
								float xPos = viewport->Size.x * 0.7f;
								drawlist->AddText({ xPos, viewport->Size.y * 0.4f }, ImGui::GetColorU32({ 1.f,1.f,1.f,1 }), str.c_str());
								str = " Tasks";
								drawlist->AddText({ xPos + ImGui::GetFontSize(), viewport->Size.y * 0.4f }, ImGui::GetColorU32({ 0.98f,0.768f,0.51f,1 }), str.c_str());
								drawlist->AddLine({ xPos, viewport->Size.y * 0.4f + ImGui::GetFontSize() }, { viewport->Size.x, viewport->Size.y * 0.4f + ImGui::GetFontSize() }, ImGui::GetColorU32({ 1,1,1,1 }), 2.f);
								ImGui::PopFont();


								ImGui::PushFont(FONT_BODY);
								ImVec2 min = { xPos, viewport->Size.y * 0.45f };
								str = "Rename the sequence";
								if (temp_graph.name != "SEQUENCE")
								{
									drawlist->AddImage((void*)static_cast<size_t>(selected->GetID()), min, { min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f });
									taskCompleted &= true;
								}
									else
								{
									drawlist->AddImage((void*)static_cast<size_t>(unselected->GetID()), min, { min.x + (float)unselected->GetWidth() * 0.6f, min.y + (float)unselected->GetHeight() * 0.6f });
									taskCompleted &= false;
								}
								drawlist->AddText({ xPos + selected->GetWidth() * 0.7f , min.y + (float)unselected->GetHeight() * 0.2f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								min = { min.x, min.y + unselected->GetWidth() * 0.9f };
								str = "Create a 'Defend Roll' Node";
								auto action_lambda1 = [&]() {
									for (auto const& this_node : temp_graph.get_nodes())
									{
										if (this_node.second->get_name() == "Defend Roll")
											return true;
									}
									return false;
								};
								if (action_lambda1())
								{
									drawlist->AddImage((void*)static_cast<size_t>(selected->GetID()), min, { min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f });
									taskCompleted &= true;
								}
								else
								{
									drawlist->AddImage((void*)static_cast<size_t>(unselected->GetID()), min, { min.x + (float)unselected->GetWidth() * 0.6f, min.y + (float)unselected->GetHeight() * 0.6f });
									taskCompleted &= false;
								}
								drawlist->AddText({ xPos + selected->GetWidth() * 0.7f, min.y + (float)unselected->GetHeight() * 0.2f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								min = { min.x, min.y + unselected->GetWidth() * 0.9f };
								str = "Connect the 'Attack Roll' node to the 'Defend Roll' node";
								auto action_lambda2 = [&]() {
									std::pair<pin_id_t, pin_id_t> pins;
									for (auto const& this_node : temp_graph.get_nodes())
									{
										if (this_node.second->get_name() == "Defend Roll")
											pins.second = this_node.second->get_input_pin(0)->get_id();
										if (this_node.second->get_name() == "Attack Roll")
											pins.first = this_node.second->get_output_pin(0)->get_id();
									}

									for (auto const& this_link : temp_graph.get_links())
									{
										if (this_link == pins)
											return true;
									}

									return false;
								};
								if (action_lambda2())
								{
									drawlist->AddImage((void*)static_cast<size_t>(selected->GetID()), min, { min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f });
									taskCompleted &= true;
								}
								else
								{
									drawlist->AddImage((void*)static_cast<size_t>(unselected->GetID()), min, { min.x + (float)unselected->GetWidth() * 0.6f, min.y + (float)unselected->GetHeight() * 0.6f });
									taskCompleted &= false;
								}
								drawlist->AddText({ xPos + selected->GetWidth() * 0.7f , min.y + (float)unselected->GetHeight() * 0.2f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								min = { min.x, min.y + unselected->GetWidth() * 0.9f };
								str = "Connect the 'Defend Roll' node to the 'Compare Flow' node";
								auto action_lambda3 = [&]() {
									std::pair<pin_id_t, pin_id_t> pins;
									for (auto const& this_node : temp_graph.get_nodes())
									{
										if (this_node.second->get_name() == "Compare Flow")
											pins.second = this_node.second->get_input_pin(0)->get_id();
										if (this_node.second->get_name() == "Defend Roll")
											pins.first = this_node.second->get_output_pin(0)->get_id();
									}

									for (auto const& this_link : temp_graph.get_links())
									{
										if (this_link == pins)
											return true;
									}

									return false;
								};
								if (action_lambda3())
								{
									drawlist->AddImage((void*)static_cast<size_t>(selected->GetID()), min, { min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f });
									taskCompleted &= true;
								}
								else
								{
									drawlist->AddImage((void*)static_cast<size_t>(unselected->GetID()), min, { min.x + (float)unselected->GetWidth() * 0.6f, min.y + (float)unselected->GetHeight() * 0.6f });
									taskCompleted &= false;
								}
								drawlist->AddText({ xPos + selected->GetWidth() * 0.7f , min.y + (float)unselected->GetHeight() * 0.2f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								min = { min.x, min.y + unselected->GetWidth() * 0.9f };
								str = "Connect the output of the 'Defend Roll' to the input of";
								auto action_lambda4 = [&]() {
									std::pair<pin_id_t, pin_id_t> pins;
									for (auto const& this_node : temp_graph.get_nodes())
									{
										if (this_node.second->get_name() == "Compare Flow")
											pins.second = this_node.second->get_input_pin(2)->get_id();
										if (this_node.second->get_name() == "Defend Roll")
											pins.first = this_node.second->get_output_pin(1)->get_id();
									}

									for (auto const& this_link : temp_graph.get_links())
									{
										if (this_link == pins)
											return true;
									}

									return false;
								};
								if (action_lambda4())
								{
									drawlist->AddImage((void*)static_cast<size_t>(selected->GetID()), min, { min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f });
									taskCompleted &= true;
								}
								else
								{
									drawlist->AddImage((void*)static_cast<size_t>(unselected->GetID()), min, { min.x + (float)unselected->GetWidth() * 0.6f, min.y + (float)unselected->GetHeight() * 0.6f });
									taskCompleted &= false;
								}
								drawlist->AddText({ xPos + selected->GetWidth() * 0.7f , min.y + (float)unselected->GetHeight() * 0.2f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
								min = { min.x, min.y + 17.f };
								str = "'Compare Flow' node";
								drawlist->AddText({ xPos + selected->GetWidth() * 0.7f , min.y + (float)unselected->GetHeight() * 0.2f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								ImGui::PopFont();

								auto nextBtn = tex_map["Assets/NextBtn.dds"];
								ImVec2 tut_min = { min.x, min.y + unselected->GetWidth() * 0.9f };
								ImVec2 tut_max = { tut_min.x + nextBtn->GetWidth() * 1.f, tut_min.y + nextBtn->GetHeight() * 1.f };

								if (taskCompleted)
								{
									drawlist->AddImage((void*)static_cast<size_t>(nextBtn->GetID()), tut_min, tut_max);

									if (UI::MouseIsWithin(tut_min, tut_max))
									{
										ImGui::SetMouseCursor(7);
										if (ImGui::IsMouseClicked(0))
										{
											OverlayOpen = false;
											instance.tutorial_slide = true; // Open tutorial Slide show for simulate Screen part2
											Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>(instance);
											Service<EventManager>::Get().instant_dispatch<SimulateTutorialP2Trigger>();
										}
									}
								}
								else
									drawlist->AddImage((void*)static_cast<size_t>(nextBtn->GetID()), tut_min, tut_max, { 0,0 }, { 1,1 }, ImGui::GetColorU32({ 1,1,1,0.4f }));
							}
							break;

							}

							UI::TutProgressBar(drawlist, ImVec2{ viewport->Size }, 4);
						}
					}

					else if (instance.tutorial_level == 2)
					{
						UI::TutProgressBar2(drawlist, ImVec2{ viewport->Size }, instance);
					}

					else if (instance.tutorial_level == 3)
					{
						UI::TutProgressBar3(drawlist, ImVec2{ viewport->Size }, 1);
					}

					//Tutorial Exit Button
					if (instance.tutorial_slide == false) //Dont show tutorial exit button during slide show
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
			}
			ImGui::End();
		}
		if (m_waypointEmitter && (!OverlayOpen || !instance.tutorial_enable))
			m_waypointEmitter->m_GM.m_active = false;
	}
	
	void AttackSystemOverlay::draw_context(Instance& instance, float height)
	{
		if (id)
		{
			// graphs

			//auto& sys = instance.ecs.get<tc::Graph>(id);
			auto& g = temp_graph;

			for (auto& [node_id, node_ptr] : g.get_nodes())
			{
				ax::NodeEditor::SetNodePosition(node_id, ImVec2(node_ptr->position.x, node_ptr->position.y));
			}

			// -----------------------------------------------------------------

			// Canvas
			// -----------------------------------------------------------------
			ImVec2 test{ 0, height };
			ax::NodeEditor::Begin("GraphEditor", test);

			update_create(g);
			update_delete(g);



			// draw nodes
			// ---------
			for (auto& [node_id, node_ptr] : g.get_nodes())
			{
				draw_node(node_ptr, g, instance);
			}
			// ---------

			// draw links
			// ---------
			for (auto [link_id, from, to] : g.get_links_as_tri())
			{
				auto [input, index, parent] = pin_to_component(from);
				auto node = g.get_node(parent);
				auto pin = node->get_output_pin(index);

				draw_link(link_id, from, to, pin->get_type());
			}

			// context
			// ---------
			draw_context(g, instance);
			// ---------
			// ---------
			ax::NodeEditor::End();
			// -----------------------------------------------------------------

			for (auto& [node_id, node_ptr] : g.get_nodes())
			{
				auto v = ax::NodeEditor::GetNodePosition(node_id);
				node_ptr->position.x = v.x;
				node_ptr->position.y = v.y;

				auto new_size = ax::NodeEditor::GetNodeSize(node_id);
				node_ptr->size.x = new_size.x;
				node_ptr->size.y = new_size.y;

			}

		}
	}

	void AttackSystemOverlay::draw_node(node_ptr n, const graph& g, Instance& instance)
	{
		// if its a comment node do this instead
		if (n->get_category() == category_type::Group)
		{
			auto ID = n->get_id();
			const auto& size = n->size;
			// Start Node
			ax::NodeEditor::BeginNode(ID);

			// Title
			ImGui::Text(n->get_name().c_str());

			ax::NodeEditor::Group({ size.x, size.y });
			ax::NodeEditor::EndNode();

			// Comment
			if (ax::NodeEditor::BeginGroupHint(ID))
			{
				auto min = ax::NodeEditor::GetGroupMin();
				auto spacing = ImVec2(-8, ImGui::GetTextLineHeightWithSpacing()-4);

				ImGui::SetCursorScreenPos({ min.x - spacing.x, min.y - spacing.y });
				ImGui::BeginGroup();
				ImGui::Text(n->get_name().c_str());
				ImGui::EndGroup();
			}
			ax::NodeEditor::EndGroupHint();
		}
		else
		{
			ax::NodeEditor::BeginNode(n->get_id());

			float owidth = 0.f;
			for (const auto& output : n->get_outputs())
			{
				owidth = std::max(
					owidth,
					ImGui::CalcTextSize(output.get_name().c_str()).x);
			}
			float iwidth = 0.f;
			for (const auto& input : n->get_inputs())
			{
				iwidth = std::max(
					iwidth,
					ImGui::CalcTextSize(input.get_name().c_str()).x);
			}

			float twidth = ImGui::CalcTextSize(n->get_name().c_str()).x;

			// Title
			//auto size = ax::NodeEditor::GetNodeSize(id);
			//ImGui::SameLine(100.f);

			if (n->get_name() != "")
			{
				ImGui::PushFont(FONT_BOLD);
				ImGui::Dummy({ 10.f, 1.f });
				ImGui::SameLine();

				/*if (auto gn = dynamic_cast<ActionGraphNode*>(n.get()))
				{
					auto gid = gn->graph_entity;

					ImGui::Text("%s: %u", instance.ecs.get<tc::Graph>(gid).g.get_name().c_str(), gid);
				}
				else */
				if (auto gsn = dynamic_cast<StatNode*>(n.get()))
				{
					tc::Statline* statline = instance.ecs.get_if<tc::Statline>(instance.ecs.view_first<tc::Statline>());

					string s = magic_enum::enum_name(gsn->get_type()).data();
					auto index = std::stoi(gsn->get_name());

					string name;
					switch (gsn->get_type())
					{
					case Tempest::StatNode::inner_type::GetStat:
						name = "Get " + (*statline)[index];
						break;
					case Tempest::StatNode::inner_type::SetStat:
						name = "Set " + (*statline)[index];
						break;
					case Tempest::StatNode::inner_type::GetMain:
						name = "Get Main";
						break;
					case Tempest::StatNode::inner_type::SetMain:
						name = "Set Main";
						break;
					case Tempest::StatNode::inner_type::ReduceStat:
						name = "Reduce " + (*statline)[index] + " by";
						break;
					case Tempest::StatNode::inner_type::RestoreStat:
						name = "Restore " + (*statline)[index] + " by";
						break;
					case Tempest::StatNode::inner_type::ResetStat:
						name = "Reset " + (*statline)[index];
						break;
					default:
						name = "If you see this, the node is corrupted";
						break;
					}

					ImGui::Text(name.c_str());
					twidth = ImGui::CalcTextSize(name.c_str()).x;
				}
				else
				{
					ImGui::Text(n->get_name().c_str());
				}
				ImGui::PopFont();
			}

			// Input group
			ImGui::BeginGroup();
			for (const auto& input : n->get_inputs())
			{
				draw_input_pin(input, g);
			}

			float extra_offset = 0.f;
			if (!n->get_num_inputs())
			{
				extra_offset = 50.f;
				ImGui::Dummy({ twidth - owidth, 0.1f });
			}
			ImGui::EndGroup();

			// Output group
			ImGui::SameLine();
			ImGui::BeginGroup();
			for (const auto& output : n->get_outputs())
			{
				draw_output_pin(output, owidth);
			}
			ImGui::EndGroup();
			ax::NodeEditor::EndNode();
		}
	}

	void AttackSystemOverlay::draw_link(link l, pin_id_t from, pin_id_t to, pin_type pin_t)
	{
		auto color = get_pin_color(pin_t);

		ax::NodeEditor::Link(l, from, to, color, 2.f);
	}

	void AttackSystemOverlay::draw_input_pin(const input_pin& p, const graph& g)
	{
		int alpha = 255;

		ax::Drawing::IconType iconType = get_pin_icon(p.get_type());
		ImColor color = get_pin_color(p.get_type());
		color.Value.w = static_cast<float>(alpha) / 255.0f;

		BeginPin(p.get_id(), ax::NodeEditor::PinKind::Input);
		ax::NodeEditor::PinPivotAlignment(ImVec2(0.5f, 0.5f));
		ax::NodeEditor::PinPivotSize(ImVec2(0.f, 0.f));


		ax::Widgets::Icon(ImVec2(24, 24), iconType, p.is_linked(), color, ImColor(32, 32, 32, alpha));
		ax::NodeEditor::EndPin();

		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.9f);
		ImGui::AlignTextToFramePadding();
		ImGui::Text(p.get_name().c_str());

		if (p.get_type() == pin_type::Int && !p.is_linked())
		{
			ImGui::SameLine();
			int uone = 1;
			int& a = p.default_var.get<int>();
			string s = "##";
			s += std::to_string(p.get_id());
			ImGui::SetNextItemWidth(90);

			/*ImGui::PushStyleColor(ImGuiCol_Button, { 250 / 255.f, 196 / 255.f, 130 / 255.f, 255 / 255.f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 232 / 255.f, 137 / 255.f, 64 / 255.f, 255 / 255.f });
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.f, 0.f, 0.f, 1.f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 250,250,250,255 });*/
			ImGui::InputScalar(s.c_str(), ImGuiDataType_S32, &a, &uone);
			//ImGui::PopStyleColor(4);
		}
		else if (p.get_type() == pin_type::Byte && !p.is_linked())
		{
			// get all variables of this type
			tvector<string> strs;
			if (g.get_type() == graph_type::action)
			{
				strs.push_back("Owner");
				strs.push_back("Enemy");
			}
			else if (g.get_type() == graph_type::conflict)
			{
				strs.push_back("Attacker");
				strs.push_back("Defender");
			}
			else
			{
				strs.push_back("None");
			}

			ImGui::SameLine();
			uint8_t& a = p.default_var.get<uint8_t>();
			string s = strs[a];
			s += "##";
			s += std::to_string(p.get_id());
			//ImGui::SetNextItemWidth(90);

			if (a)
			{
				auto v1 = ImVec4(196, 8, 93, 255) * ImVec4(1/255.f, 1/255.f, 1/255.f, 1/255.f);
				auto v2 = ImVec4(221, 9, 105, 255) * ImVec4(1 / 255.f, 1 / 255.f, 1 / 255.f, 1 / 255.f);
				auto v3 = ImVec4(245, 10, 117, 255) * ImVec4(1 / 255.f, 1 / 255.f, 1 / 255.f, 1 / 255.f);
				ImGui::PushStyleColor(ImGuiCol_Button, v1);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, v2);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, v3);
			}
			else
			{
				auto v1 = ImVec4(196, 111, 8, 255) * ImVec4(1 / 255.f, 1 / 255.f, 1 / 255.f, 1 / 255.f);
				auto v2 = ImVec4(221, 125, 9, 255) * ImVec4(1 / 255.f, 1 / 255.f, 1 / 255.f, 1 / 255.f);
				auto v3 = ImVec4(245, 139, 10, 255) * ImVec4(1 / 255.f, 1 / 255.f, 1 / 255.f, 1 / 255.f);
				ImGui::PushStyleColor(ImGuiCol_Button, v1);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, v2);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, v3);
			}
			if (ImGui::Button(s.c_str(), {90, 0}))
			{
				a ? a = 0 : a = (uint8_t)strs.size()-1;
			}
			ImGui::PopStyleColor(3);

			//if (UI::BeginNodeCombo("##combo 1", strs[a].c_str()))
			//{
			//	for (int n = 0; n < strs.size(); n++)
			//	{
			//		const bool is_selected = (a == n);
			//		if (ImGui::Selectable(strs[n].c_str(), is_selected))
			//			a = n;

			//	
			//		// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			//		if (is_selected)
			//			ImGui::SetItemDefaultFocus();
			//	}
			//	ImGui::EndCombo();
			//}

			//ImGui::Combo("##no_label", &i, strs.data(), strs.size());

			/*if (ImGui::InputScalar(s.c_str(), ImGuiDataType_U8, &a, &uone))
				a = std::clamp(a, (uint8_t)0, (uint8_t)1);*/
		}
		else
		{
			ImGui::SameLine();
			ImGui::Dummy({ 90.f, 0.1f });
		}
	}

	void AttackSystemOverlay::draw_output_pin(const output_pin& p, float owidth)
	{
		int alpha = 255;

		ax::Drawing::IconType iconType = get_pin_icon(p.get_type());
		ImColor color = get_pin_color(p.get_type());
		color.Value.w = static_cast<float>(alpha) / 255.0f;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + owidth
			- ImGui::CalcTextSize(p.get_name().c_str()).x);

		ImGui::Dummy({ 1.f, 0.f });
		ImGui::SameLine();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.9f);
		ImGui::AlignTextToFramePadding();
		ImGui::SetNextItemWidth(owidth);
		ImGui::Text(p.get_name().c_str());
		ImGui::SameLine();


		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 1.9f);
		BeginPin(p.get_id(), ax::NodeEditor::PinKind::Output);
		ax::NodeEditor::PinPivotAlignment(ImVec2(0.5f, 0.5f));
		ax::NodeEditor::PinPivotSize(ImVec2(0.f, 0.f));

		ax::Widgets::Icon(ImVec2(24, 24), iconType, p.is_linked(), color, ImColor(32, 32, 32, alpha));

		ax::NodeEditor::EndPin();
	}

	template <typename TNode>
	void AttackSystemOverlay::node_context(
		graph& g,
		const char* title)
	{
		string filter_string = filter.InputBuf;

		if (filter_string.empty())
		{
			if (ImGui::TreeNodeEx(title))
			{
				for (unsigned i = 0; i < static_cast<unsigned>(TNode::inner_type::END); ++i)
				{
					std::string text = magic_enum::enum_name(static_cast<typename TNode::inner_type>(i)).data();
					ImGui::Indent(indent);
					if (ImGui::Selectable(text.c_str()))
					{
						auto node = g.add_node(TNode::create_node(text));

						if (node)
						{
							ax::NodeEditor::SetNodePosition(
								node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
							);
						}
						ImGui::CloseCurrentPopup();
					}
					ImGui::Unindent(indent);
				}
				ImGui::TreePop();
			}
		}
		else
		{
			for (unsigned i = 0; i < static_cast<unsigned>(TNode::inner_type::END); ++i)
			{
				std::string text = magic_enum::enum_name(static_cast<typename TNode::inner_type>(i)).data();


				if (!filter.PassFilter(text.c_str())) continue;

				if (ImGui::Selectable(text.c_str()))
				{
					auto node = g.add_node(TNode::create_node(text));

					if (node)
					{
						ax::NodeEditor::SetNodePosition(
							node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
						);
					}
					ImGui::CloseCurrentPopup();
				}
			}
		}
		
	}


	void AttackSystemOverlay::draw_context(graph& g, Instance& instance)
	{
		static ax::NodeEditor::NodeId contextNodeId = 0;
		static ax::NodeEditor::PinId contextPinId = 0;
		static ax::NodeEditor::LinkId contextLinkId = 0;

		ax::NodeEditor::Suspend();


		if (ax::NodeEditor::ShowNodeContextMenu(&contextNodeId))
		{
			mouse = ImGui::GetMousePos();
			ImGui::OpenPopup("Node Context Menu");
		}

		else if (ax::NodeEditor::ShowPinContextMenu(&contextPinId))
		{
			mouse = ImGui::GetMousePos();
			ImGui::OpenPopup("Pin Context Menu");
		}

		else if (ax::NodeEditor::ShowLinkContextMenu(&contextLinkId))
		{
			mouse = ImGui::GetMousePos();
			ImGui::OpenPopup("Link Context Menu");
		}

		else if (ax::NodeEditor::ShowBackgroundContextMenu())
		{
			mouse = ImGui::GetMousePos();
			ImGui::OpenPopup("Create New Node");
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));

		draw_node_context(g, instance, contextNodeId);
		draw_pin_context(g, instance, contextPinId);
		draw_link_context(g, instance, contextLinkId);
		draw_background_context(g, instance);

		ImGui::PopStyleVar();


		ax::NodeEditor::Resume();

	}

	void AttackSystemOverlay::draw_node_context(graph& g, Instance& , ax::NodeEditor::NodeId contextNodeId)
	{
		if (ImGui::BeginPopup("Node Context Menu", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			if (auto* node = g.get_node((node_id_t)contextNodeId.Get()))
			{
				if (node->get_category() == category_type::Group)
				{
					ImGui::PushID((node_id_t)contextNodeId.Get());
					ImGui::InputText("", &node->name);
					ImGui::PopID();
				}
				else
					ImGui::TextUnformatted(ICON_FA_PROJECT_DIAGRAM " Node");

				ImGui::Separator();

				ImGui::Text("Category: %s", magic_enum::enum_name(node->get_category()).data());
				ImGui::SameLine();
				UI::Tooltip(ICON_FA_QUESTION_CIRCLE, "Test");
				ImGui::Separator();

				if (ImGui::MenuItem("Delete"))
					ax::NodeEditor::DeleteNode(contextNodeId);

				/*if (ImGui::MenuItem("Copy", "", false, ax::NodeEditor::GetSelectedObjectCount()))
					copy_selected();*/
			}
			else
			{
				ImGui::Text("Unknown node: %u", contextNodeId.Get());
			}

			ImGui::EndPopup();
		}
	}

	void AttackSystemOverlay::draw_pin_context(graph& g, Instance&, ax::NodeEditor::PinId contextPinId)
	{
		if (ImGui::BeginPopup("Pin Context Menu", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::TextUnformatted(ICON_FA_MAP_MARKER " Pin");
			ImGui::Separator();
			if (auto* i_pin = g.get_input_pin((pin_id_t)contextPinId.Get()))
			{
				ImGui::Text("Input");
				ImGui::Text("Type: %s", magic_enum::enum_name(i_pin->get_type()).data());
				auto [a,b,c] = pin_to_component(i_pin->get_id());
				if (auto n = g.get_node(c))
					ImGui::Text("Node: %s", n->get_name().c_str());
				else
					ImGui::Text("Node: %s", "<none>");
			}
			else if (auto* o_pin = g.get_output_pin((pin_id_t)contextPinId.Get()))
			{
				ImGui::Text("Output");
				ImGui::Text("Type: %s", magic_enum::enum_name(o_pin->get_type()).data());
				auto [a, b, c] = pin_to_component(o_pin->get_id());
				if (auto n = g.get_node(c))
					ImGui::Text("Node: %s", n->get_name().c_str());
				else
					ImGui::Text("Node: %s", "<none>");
			}
			else
			{
				ImGui::Text("Unknown pin: %u", contextPinId.Get());
			}


			ImGui::EndPopup();
		}
	}

	void AttackSystemOverlay::draw_link_context(graph& g, Instance&, ax::NodeEditor::LinkId contextLinkId)
	{
		if (ImGui::BeginPopup("Link Context Menu", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			auto linkid = (pin_id_t)contextLinkId.Get();
			auto [from, to] = split_uint64_t(linkid);

			ImGui::TextUnformatted(ICON_FA_LINK " Link");
			ImGui::Separator();
			if (linkid)
			{
				if (auto* pin = g.get_input_pin(to))
				{
					ImGui::Text("Type: %s", magic_enum::enum_name(pin->get_type()).data());
				}
			}
			else
			{
				ImGui::Text("Unknown link: %u", linkid);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Flow"))
			{
				//ax::NodeEditor::Resume();
				ax::NodeEditor::Flow(contextLinkId);
				//ax::NodeEditor::Suspend();
			}
			ImGui::Separator();

			if (ImGui::MenuItem("Delete"))
				ax::NodeEditor::DeleteLink(linkid);

			ImGui::EndPopup();
		}
	}

	void AttackSystemOverlay::draw_background_context(graph& g, Instance& instance)
	{
		if (ImGui::BeginPopup("Create New Node", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			// filter
			{
				filter.Draw("##graph_bg_context");
				ImGui::SameLine();
				UI::Tooltip(ICON_FA_QUESTION_CIRCLE, "Filter usage:\n"
					"  \"\"         display all lines\n"
					"  \"xxx\"      display lines containing \"xxx\"\n"
					"  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
					"  \"-xxx\"     hide lines containing \"xxx\"", false);

			}

			ImGui::Separator();

			ImGui::BeginChild("Nodes", ImVec2(250.f, 300.f), false, ImGuiWindowFlags_NoResize);

			node_context<ArithmeticNode>(g, "Operators");
			node_context<DiceNode>(g, "Dice Rolls");
			node_context<SwitchNode>(g, "Switches");
			node_context<CompareNode>(g, "Compare");

			string filter_string = filter.InputBuf;

			// stats
			if (filter_string.empty())
			{
				if (ImGui::TreeNodeEx("Stats"))
				{
					tc::Statline* statline = instance.ecs.get_if<tc::Statline>(instance.ecs.view_first<tc::Statline>());
					LOG_ASSERT(statline);
					if (ImGui::TreeNodeEx("Getters"))
					{
						std::string text = "Get Main";
						ImGui::Indent(indent);
						if (ImGui::Selectable(text.c_str()))
						{
							auto node = g.add_node(StatNode::create_node(string("GetMain:0")));

							if (node)
							{
								ax::NodeEditor::SetNodePosition(
									node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
								);
							}
							ImGui::CloseCurrentPopup();
						}
						ImGui::Unindent(indent);

						for (auto i = 0; i < statline->size(); ++i)
						{
							if (!(*statline)(i)) continue;

							text = "Get " + (*statline)[i];
							ImGui::Indent(indent);
							if (ImGui::Selectable(text.c_str()))
							{
								auto node = g.add_node(StatNode::create_node(string("GetStat:") + std::to_string(i)));

								if (node)
								{
									ax::NodeEditor::SetNodePosition(
										node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
									);
								}
								ImGui::CloseCurrentPopup();
							}
							ImGui::Unindent(indent);
						}
						ImGui::TreePop();
					}
					if (ImGui::TreeNodeEx("Setters"))
					{
						string text;
						for (auto i = 0; i < statline->size(); ++i)
						{
							if (!(*statline)(i)) continue;

							text = "Set " + (*statline)[i];
							ImGui::Indent(indent);
							if (ImGui::Selectable(text.c_str()))
							{
								auto node = g.add_node(StatNode::create_node(string("SetStat:") + std::to_string(i)));

								if (node)
								{
									ax::NodeEditor::SetNodePosition(
										node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
									);
								}
								ImGui::CloseCurrentPopup();
							}
							ImGui::Unindent(indent);
						}
						ImGui::TreePop();
					}
					if (ImGui::TreeNodeEx("Reducers"))
					{
						string text;
						for (auto i = 0; i < statline->size(); ++i)
						{
							if (!(*statline)(i)) continue;

							text = "Reduce " + (*statline)[i];
							ImGui::Indent(indent);
							if (ImGui::Selectable(text.c_str()))
							{
								auto node = g.add_node(StatNode::create_node(string("ReduceStat:") + std::to_string(i)));

								if (node)
								{
									ax::NodeEditor::SetNodePosition(
										node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
									);
								}
								ImGui::CloseCurrentPopup();
							}
							ImGui::Unindent(indent);
						}
						ImGui::TreePop();
					}
					if (ImGui::TreeNodeEx("Restorers"))
					{
						string text;
						for (auto i = 0; i < statline->size(); ++i)
						{
							if (!(*statline)(i)) continue;

							text = "Restore " + (*statline)[i];
							ImGui::Indent(indent);
							if (ImGui::Selectable(text.c_str()))
							{
								auto node = g.add_node(StatNode::create_node(string("RestoreStat:") + std::to_string(i)));

								if (node)
								{
									ax::NodeEditor::SetNodePosition(
										node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
									);
								}
								ImGui::CloseCurrentPopup();
							}
							ImGui::Unindent(indent);
						}
						ImGui::TreePop();
					}
					if (ImGui::TreeNodeEx("Resetters"))
					{
						string text;
						for (auto i = 0; i < statline->size(); ++i)
						{
							if (!(*statline)(i)) continue;

							text = "Restore " + (*statline)[i];
							ImGui::Indent(indent);
							if (ImGui::Selectable(text.c_str()))
							{
								auto node = g.add_node(StatNode::create_node(string("ResetStat:") + std::to_string(i)));

								if (node)
								{
									ax::NodeEditor::SetNodePosition(
										node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
									);
								}
								ImGui::CloseCurrentPopup();
							}
							ImGui::Unindent(indent);
						}
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}

			}
			else
			{
				const char* alt = "Stats";
				tc::Statline* statline = instance.ecs.get_if<tc::Statline>(instance.ecs.view_first<tc::Statline>());

				LOG_ASSERT(statline);

				std::string text = "Get Main";
				if (filter.PassFilter(text.c_str()) || filter.PassFilter(alt))
				{
					if (ImGui::Selectable(text.c_str()))
					{
						auto node = g.add_node(StatNode::create_node(string("GetMain:0")));

						if (node)
						{
							ax::NodeEditor::SetNodePosition(
								node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
							);
						}
						ImGui::CloseCurrentPopup();
					}
				}

				// getter
				for (auto i = 0; i < statline->size(); ++i)
				{
					if (!(*statline)(i)) continue;

					text = "Get " + (*statline)[i];
					if (!filter.PassFilter(text.c_str()) && !filter.PassFilter(alt)) continue;


					if (ImGui::Selectable(text.c_str()))
					{
						auto node = g.add_node(StatNode::create_node(string("GetStat:") + std::to_string(i)));

						if (node)
						{
							ax::NodeEditor::SetNodePosition(
								node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
							);
						}
						ImGui::CloseCurrentPopup();
					}
				}
				// setter
				for (auto i = 0; i < statline->size(); ++i)
				{
					if (!(*statline)(i)) continue;

					text = "Set " + (*statline)[i];
					if (!filter.PassFilter(text.c_str()) && !filter.PassFilter(alt)) continue;


					if (ImGui::Selectable(text.c_str()))
					{
						auto node = g.add_node(StatNode::create_node(string("SetStat:") + std::to_string(i)));

						if (node)
						{
							ax::NodeEditor::SetNodePosition(
								node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
							);
						}
						ImGui::CloseCurrentPopup();
					}
				}
				// reducer
				for (auto i = 0; i < statline->size(); ++i)
				{
					if (!(*statline)(i)) continue;

					text = "Reduce " + (*statline)[i];
					if (!filter.PassFilter(text.c_str()) && !filter.PassFilter(alt)) continue;


					if (ImGui::Selectable(text.c_str()))
					{
						auto node = g.add_node(StatNode::create_node(string("ReduceStat:") + std::to_string(i)));

						if (node)
						{
							ax::NodeEditor::SetNodePosition(
								node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
							);
						}
						ImGui::CloseCurrentPopup();
					}
				}
				// restore
				for (auto i = 0; i < statline->size(); ++i)
				{
					if (!(*statline)(i)) continue;

					text = "Restore " + (*statline)[i];
					if (!filter.PassFilter(text.c_str()) && !filter.PassFilter(alt)) continue;


					if (ImGui::Selectable(text.c_str()))
					{
						auto node = g.add_node(StatNode::create_node(string("RestoreStat:") + std::to_string(i)));

						if (node)
						{
							ax::NodeEditor::SetNodePosition(
								node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
							);
						}
						ImGui::CloseCurrentPopup();
					}
				}
				// reset
				for (auto i = 0; i < statline->size(); ++i)
				{
					if (!(*statline)(i)) continue;

					text = "Reset " + (*statline)[i];
					if (!filter.PassFilter(text.c_str()) && !filter.PassFilter(alt)) continue;


					if (ImGui::Selectable(text.c_str()))
					{
						auto node = g.add_node(StatNode::create_node(string("ResetStat:") + std::to_string(i)));

						if (node)
						{
							ax::NodeEditor::SetNodePosition(
								node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
							);
						}
						ImGui::CloseCurrentPopup();
					}
				}
			}

			// graph specific contexts
			if (g.get_type() == graph_type::action)
			{
				if (filter_string.empty())
				{
					if (ImGui::TreeNodeEx("Output"))
					{
						ImGui::Indent(indent);
						if (ImGui::Selectable("Output"))
						{
							auto node = g.add_node(ActionNode::create_node("Output"));

							if (node)
							{
								ax::NodeEditor::SetNodePosition(
									node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
								);
							}
							ImGui::CloseCurrentPopup();
						}
						ImGui::Unindent(indent);
						ImGui::TreePop();
					}


				}
				else
				{
					if (filter.PassFilter("Output") && ImGui::Selectable("Output"))
					{
						auto node = g.add_node(ActionNode::create_node("Output"));

						if (node)
						{
							ax::NodeEditor::SetNodePosition(
								node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
							);
						}
						ImGui::CloseCurrentPopup();
					}
				}


			}
			else if (g.get_type() == graph_type::conflict)
			{

				if (filter_string.empty())
				{
					if (ImGui::TreeNodeEx("Resolution"))
					{
						ImGui::Indent(indent);
						if (ImGui::Selectable("Win"))
						{
							auto node = g.add_node(ConflictNode::create_node("Win"));

							if (node)
							{
								ax::NodeEditor::SetNodePosition(
									node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
								);
							}
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Selectable("Lose"))
						{
							auto node = g.add_node(ConflictNode::create_node("Lose"));

							if (node)
							{
								ax::NodeEditor::SetNodePosition(
									node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
								);
							}
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Selectable("Attack Roll"))
						{
							auto node = g.add_node(ConflictNode::create_node("AttackRoll"));

							if (node)
							{
								ax::NodeEditor::SetNodePosition(
									node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
								);
							}
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Selectable("Defend Roll"))
						{
							auto node = g.add_node(ConflictNode::create_node("DefendRoll"));

							if (node)
							{
								ax::NodeEditor::SetNodePosition(
									node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
								);
							}
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Selectable("Resolve Attack"))
						{
							auto node = g.add_node(ConflictNode::create_node("AttackResolve"));

							if (node)
							{
								ax::NodeEditor::SetNodePosition(
									node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
								);
							}
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Selectable("Resolve Defend"))
						{
							auto node = g.add_node(ConflictNode::create_node("DefendResolve"));

							if (node)
							{
								ax::NodeEditor::SetNodePosition(
									node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
								);
							}
							ImGui::CloseCurrentPopup();
						}
						ImGui::Unindent(indent);
						ImGui::TreePop();
					}
				}
				else
				{
					if (filter.PassFilter("Win") && ImGui::Selectable("Win"))
					{
						auto node = g.add_node(ConflictNode::create_node("Win"));

						if (node)
						{
							ax::NodeEditor::SetNodePosition(
								node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
							);
						}
						ImGui::CloseCurrentPopup();
					}
					if (filter.PassFilter("Lose") && ImGui::Selectable("Lose"))
					{
						auto node = g.add_node(ConflictNode::create_node("Lose"));

						if (node)
						{
							ax::NodeEditor::SetNodePosition(
								node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
							);
						}
						ImGui::CloseCurrentPopup();
					}
					if (filter.PassFilter("Attack Roll") && ImGui::Selectable("Attack Roll"))
					{
						auto node = g.add_node(ConflictNode::create_node("AttackRoll"));

						if (node)
						{
							ax::NodeEditor::SetNodePosition(
								node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
							);
						}
						ImGui::CloseCurrentPopup();
					}
					if (filter.PassFilter("Defend Roll") && ImGui::Selectable("Defend Roll"))
					{
						auto node = g.add_node(ConflictNode::create_node("DefendRoll"));

						if (node)
						{
							ax::NodeEditor::SetNodePosition(
								node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
							);
						}
						ImGui::CloseCurrentPopup();
					}
					if (filter.PassFilter("Resolve Attack") && ImGui::Selectable("Resolve Attack"))
					{
						auto node = g.add_node(ConflictNode::create_node("AttackResolve"));

						if (node)
						{
							ax::NodeEditor::SetNodePosition(
								node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
							);
						}
						ImGui::CloseCurrentPopup();
					}
					if (filter.PassFilter("Resolve Defend") && ImGui::Selectable("Resolve Defend"))
					{
						auto node = g.add_node(ConflictNode::create_node("DefendResolve"));

						if (node)
						{
							ax::NodeEditor::SetNodePosition(
								node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
							);
						}
						ImGui::CloseCurrentPopup();
					}
				}

			}

			// comment
			if (filter_string.empty())
			{
				std::string text = "Comment";
				if (ImGui::Selectable(text.c_str()))
				{
					auto node = g.add_node(GroupNode::create_node(string("Comment")));

					if (node)
					{
						ax::NodeEditor::SetNodePosition(
							node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
						);
					}
					ImGui::CloseCurrentPopup();
				}
			}
			else
			{
				std::string text = "Comment";
				if (filter.PassFilter(text.c_str()))
				{
					if (ImGui::Selectable(text.c_str()))
					{
						auto node = g.add_node(GroupNode::create_node(string("Comment")));

						if (node)
						{
							ax::NodeEditor::SetNodePosition(
								node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
							);
						}
						ImGui::CloseCurrentPopup();
					}
				}
			}

			ImGui::Separator();

			if (ImGui::Button("Save Current as Default"))
			{
				try
				{
					g.serialize(instance.get_path() / "conflict_resolutions");
				}
				catch (const std::exception& a)
				{
					LOG_ERROR(a.what());
				}
			}


			
			ImGui::EndChild();

			ImGui::EndPopup();
		}
	}

	void Tempest::AttackSystemOverlay::copy_selected()
	{
		//const auto copyable_size = 1000;
		//nodes_copied.clear();
		//links_copied.clear();

		//{
		//	tvector<ax::NodeEditor::NodeId> nodes_copied_temp(copyable_size);
		//	auto n = ax::NodeEditor::GetSelectedNodes(nodes_copied_temp.data(), copyable_size);
		//	for (int i = 0; i < n; ++i)
		//	{
		//		//nodes_copied.push_back((node_id_t)nodes_copied_temp[i].Get());
		//	}

		//}
		//{
		//	tvector<ax::NodeEditor::LinkId> links_copied_temp(copyable_size);
		//	auto n = ax::NodeEditor::GetSelectedLinks(links_copied_temp.data(), copyable_size);
		//	for (int i = 0; i < n; ++i)
		//	{
		//		links_copied.push_back((size_t)links_copied_temp[i].Get());
		//	}
		//}

		//copy_from = id;
	}

	void Tempest::AttackSystemOverlay::paste_selected(graph& , Instance& instance)
	{
		if (auto from = instance.ecs.get_if<tc::Graph>(copy_from))
		{
		}
	}

	void AttackSystemOverlay::update_create(graph& g)
	{
		

		if (ax::NodeEditor::BeginCreate())
		{
			// create links
			if (ax::NodeEditor::PinId start = 0, end = 0;
				QueryNewLink(&start, &end))
			{

				auto s = (pin_id_t)start.Get();
				auto e = (pin_id_t)end.Get();

				auto [x, s_index, s_parent] = pin_to_component(s);
				auto [y, e_index, e_parent] = pin_to_component(e);

				if (x == true)
				{
					std::swap(x, y);
					std::swap(s_index, e_index);
					std::swap(s_parent, e_parent);
					std::swap(s, e);
				}

				auto s_pin = g.get_output_pin(s);
				auto e_pin = g.get_input_pin(e);

				ImColor bad = ImColor(45, 32, 32, 180);


				if (!s_pin)
				{
					UI::ShowLabel("x Linking two input pins", bad);
					ax::NodeEditor::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (!e_pin)
				{
					UI::ShowLabel("x Linking two output pins", bad);
					ax::NodeEditor::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (s == e)
				{
					UI::ShowLabel("x Linking pin to itself", bad);
					ax::NodeEditor::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (s_parent == e_parent)
				{
					UI::ShowLabel("x Cannot connect to self", bad);
					ax::NodeEditor::RejectNewItem(ImColor(255, 0, 0), 1.0f);
				}
				else if (s_pin->get_type() != e_pin->get_type())
				{
					UI::ShowLabel("x Incompatible Pin Type", bad);
					ax::NodeEditor::RejectNewItem(ImColor(255, 128, 128), 1.0f);
				}
				else // link is valid (almost)
				{
					// flow pins can have mulitple inputs
					// reg pins can have multiple outputs
					if (e_pin->get_type() == pin_type::Flow && s_pin->is_linked())
					{
						UI::ShowLabel("+ Replace Flow Link", ImColor(45, 32, 32, 180));
						if (ax::NodeEditor::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
						{
							g.remove_links_to_output_pin(s);
							g.add_link(s, e);

							mouse = ImGui::GetMousePos();

							glm::vec2 tempVec;
							tempVec.x = mouse.x;
							tempVec.y = mouse.y;

							if(m_explosionEmitter)
								ParticleSystem_2D::GetInstance().ReuseExplosionEmitter(m_explosionEmitter, tempVec);
							else
								m_explosionEmitter = ParticleSystem_2D::GetInstance().ExplosionEmitter_2(tempVec);
						}
					}
					else if (e_pin->get_type() != pin_type::Flow && e_pin->is_linked())
					{
						UI::ShowLabel("+ Replace Value Link", ImColor(45, 32, 32, 180));
						if (ax::NodeEditor::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
						{
							g.remove_links_to_input_pin(e);
							g.add_link(s, e);

							mouse = ImGui::GetMousePos();

							glm::vec2 tempVec;
							tempVec.x = mouse.x;
							tempVec.y = mouse.y;

							if (m_explosionEmitter)
								ParticleSystem_2D::GetInstance().ReuseExplosionEmitter(m_explosionEmitter, tempVec);
							else
								m_explosionEmitter = ParticleSystem_2D::GetInstance().ExplosionEmitter_2(tempVec);
						}
					}
					else if (s_pin->is_linked() && e_pin->is_linked())
					{
						UI::ShowLabel("x Too many inputs/outputs!", ImColor(45, 32, 32, 180));
						ax::NodeEditor::RejectNewItem(ImColor(255, 128, 128), 1.0f);
					}
					else // valid
					{
						UI::ShowLabel("+ Create Link", ImColor(32, 45, 32, 180));
						if (ax::NodeEditor::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
						{
							g.add_link(s, e);

							mouse = ImGui::GetMousePos();

							glm::vec2 tempVec;
							tempVec.x = mouse.x;
							tempVec.y = mouse.y;

							if(m_explosionEmitter)
								ParticleSystem_2D::GetInstance().ReuseExplosionEmitter(m_explosionEmitter, tempVec);
							else
								m_explosionEmitter = ParticleSystem_2D::GetInstance().ExplosionEmitter_2(tempVec);
						}
					}
				}
			}
		}
		ax::NodeEditor::EndCreate();
	}

	void AttackSystemOverlay::update_delete(graph& g)
	{
		if (ax::NodeEditor::BeginDelete())
		{
			// delete links
			ax::NodeEditor::LinkId linkId = 0;
			while (QueryDeletedLink(&linkId))
			{
				if (ax::NodeEditor::AcceptDeletedItem())
				{
					g.remove_link(linkId.Get());
				}
			}

			// delete nodes
			ax::NodeEditor::NodeId nodeId = 0;
			while (QueryDeletedNode(&nodeId))
			{
				if (ax::NodeEditor::AcceptDeletedItem())
				{
					// no deletion of input nodes
					auto n = g.get_node((node_id_t)nodeId.Get());
					tvector<string> non_deletables = { "Input", "Start", "Resolve", "Roll" };
					bool non_delete_nodes = true;

					for (auto& s : non_deletables) non_delete_nodes &= n->get_name() != s;

					if (non_delete_nodes)
						g.remove_node((node_id_t)nodeId.Get());
				}
			}
		}
		ax::NodeEditor::EndDelete();
	}

	ImColor AttackSystemOverlay::get_pin_color(pin_type pin_t)
	{
		switch (pin_t)
		{
		case pin_type::Flow: return ImColor(192, 192, 192);
		case pin_type::Bool: return ImColor(128, 0, 0); // maroon
		case pin_type::Byte: return ImColor(9, 105, 221); // neon blue
		case pin_type::Int: return ImColor(250, 196, 130); // some orange
		case pin_type::Int64: return ImColor(173, 223, 173); // moss green
		case pin_type::Float: return ImColor(154, 205, 50); // yellow green
		case pin_type::String: return ImColor(212, 175, 55); // gold
		case pin_type::Vec2: return ImColor(224, 176, 255); // mauve
		case pin_type::Vec3: return ImColor(255, 0, 255); // magenta
		case pin_type::Vec4: return ImColor(255, 192, 203); // pink
		case pin_type::Entity: return ImColor(17, 166, 212); // light blue
		case pin_type::Vector: return ImColor(235, 217, 52); // light yellow
		default: return ImColor(255, 255, 255);
		}
	}

	ax::Drawing::IconType AttackSystemOverlay::get_pin_icon(pin_type pin_t)
	{
		switch (pin_t)
		{
		case pin_type::Flow: return ax::Drawing::IconType::Flow;				break;
		case pin_type::Bool: return ax::Drawing::IconType::Diamond;				break;
		case pin_type::Byte: return ax::Drawing::IconType::RoundSquare;			break;
		case pin_type::Int: return ax::Drawing::IconType::Circle;				break;
		case pin_type::Int64: return ax::Drawing::IconType::Circle;				break;
		case pin_type::Float: return ax::Drawing::IconType::Square;				break;
		case pin_type::String: return ax::Drawing::IconType::Grid;				break;
		case pin_type::Vec2: return ax::Drawing::IconType::RoundSquare;			break;
		case pin_type::Vec3: return ax::Drawing::IconType::RoundSquare;			break;
		case pin_type::Vec4: return ax::Drawing::IconType::RoundSquare;			break;
		case pin_type::Entity: return ax::Drawing::IconType::Square;			break;
		case pin_type::Vector: return ax::Drawing::IconType::Square;			break;
		default:
			return ax::Drawing::IconType::Square;			break;
		}
	}

}