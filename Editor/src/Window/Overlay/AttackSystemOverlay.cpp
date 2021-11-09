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
#include "AttackSystemOverlay.h"
#include "Util/UIElements.h"

namespace Tempest
{
	void AttackSystemOverlay::open_popup(const Event& e)
	{
		auto a = event_cast<OpenGraphTrigger>(e);
		OverlayOpen = true;
		type = a.type;

		id = UNDEFINED;

		if (a.instance.ecs.has<tc::Graph>(a.id))
		{
			id = a.id;
			temp_graph = a.instance.ecs.get<tc::Graph>(id).g;
		}

		else
		{
			if (type == OPEN_GRAPH_TYPE::GRAPH_ACTION)
			{
				for (auto thisGraph : a.instance.ecs.view<tc::ActionGraph>(exclude_t<tc::Destroyed>()))
				{
					id = thisGraph;
					temp_graph = a.instance.ecs.get<tc::Graph>(id).g;
					break;
				}
			}
			
			else
			{
				for (auto thisGraph : a.instance.ecs.view<tc::ConflictGraph>(exclude_t<tc::Destroyed>()))
				{
					id = thisGraph;
					temp_graph = a.instance.ecs.get<tc::Graph>(id).g;
					break;
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

		ax::NodeEditor::NavigateToContent();
	}
	void AttackSystemOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		if (OverlayOpen)
		{
			if (ImGui::Begin("Attack System", nullptr, window_flags))
			{

				const float title_top_padding = 40.f;
				const float title_left_padding = 40.f;
				const float title_bottom_padding = 10.f;
				const float graph_context_height = viewport->Size.y * 0.95f;

				//auto& g = instance.ecs.get<tc::Graph>(id);
				
				//if (ImGui::BeginTable("graph header table", 3))
				//{
				//	ImGui::TableNextRow();

				//	ImGui::TableSetColumnIndex(1);
				//	UI::Header_1("Attack System");

				//	ImGui::PushFont(FONT_HEAD);

				//	ImGui::TableSetColumnIndex(0);
				//	ImGui::Dummy({ 0.1f, title_top_padding });
				//	ImGui::Dummy({ title_left_padding, 0.1f });
				//	ImGui::SameLine();
				//	static string test = "Testing";
				//	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0,0,0 });
				//	ImGui::InputText("##testing", &temp_graph.name);
				//	ImGui::PopStyleColor();
				//	//ImGui::Text(g.g.get_name().c_str());
				//	ImGui::PopFont();


				//	ImGui::TableNextRow();
				//	ImGui::TableSetColumnIndex(0);
				//	ImGui::Dummy({ 0.1f, title_bottom_padding });

				//	ImGui::EndTable();
				//}
				//ImGui::Separator();

				auto tex = tex_map["Assets/GraphBG.png"];
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


				//float button_y = ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.3f;
				//// first button pos
				//ImVec2 pos1 = {ImGui::GetContentRegionAvailWidth() - 480.f, button_y};

				//// second button pos
				//ImVec2 pos2 = { ImGui::GetContentRegionAvailWidth() - 300.f, button_y };

				//// third button pos
				//ImVec2 pos3 = { ImGui::GetContentRegionAvailWidth() - 120.f, button_y };

				//if (UI::UIButton_2("Navigate to Content", "Navigate to Content", pos1, { 0.f, 10.f }, FONT_PARA))
				//{
				//	ax::NodeEditor::NavigateToContent();
				//}
				//if (UI::UIButton_2("Save & Close", "Save & Close", pos2, { 0.f, 10.f }, FONT_PARA))
				//{
				//	OverlayOpen = false;
				//	instance.ecs.get<tc::Graph>(id).g = temp_graph;
				//	Service<EventManager>::Get().instant_dispatch<OpenConflictResTrigger>();
				//}

				//if (UI::UIButton_2("Close", "Close", pos3, { 0.f, 10.f }, FONT_PARA))
				//{
				//	OverlayOpen = false;
				////	Service<EventManager>::Get().instant_dispatch<OpenConflictResTrigger>();
				//}

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
				if (id)
				{
					ImGui::PushFont(FONT_HEAD);
					ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
					ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.f,0.f,0.f, 0.f });
					const ImVec2 text_size{ ImGui::CalcTextSize(temp_graph.name.c_str()) };
					ImGui::SetCursorPos(ImVec2{viewport->Size.x * 0.8f - text_size.x * 0.5f, viewport->Size.y * 0.1f });
					if (ImGui::BeginChild("Editing name", ImVec2{ text_size.x * 2.0f, text_size.y + 5.0f}, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar))
					{
						ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0,0,0 });
						ImGui::PushItemWidth(text_size.x * 2.0f);
						ImGui::InputText("##testing", &temp_graph.name);
						ImGui::PopItemWidth();
						ImGui::PopStyleColor();
					}
					ImGui::EndChild();
					ImGui::PopStyleVar();
					ImGui::PopStyleColor();
					ImGui::PopFont();
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
						textcol = { 0,0,0,0.7 };
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
									instance.ecs.emplace<tc::Destroyed>(id);
									id = UNDEFINED;
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
									instance.ecs.emplace<tc::Destroyed>(id);
									id = UNDEFINED;
								}

								++i;
							}
						}

						// create new graphs
						if (UI::UIButton_1("+", "+", { cursor.x , cursor.y + i * 80 }, { 150,-10 }, FONT_HEAD))
						{
							auto i = instance.ecs.create();

							if (type == OPEN_GRAPH_TYPE::GRAPH_ACTION)
							{
								instance.ecs.emplace<tc::ActionGraph>(i);
								instance.ecs.emplace<tc::Graph>(i, "ACTION", graph_type::action);
								
							}

							else
							{
								instance.ecs.emplace<tc::ConflictGraph>(i);
								instance.ecs.emplace<tc::Graph>(i, "SEQUENCE", graph_type::conflict);
							}

							id = i;
							temp_graph = instance.ecs.get<tc::Graph>(id).g;
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
					if (ImGui::BeginChild("Top Buttons", ImVec2{ 400.0f, 100.0f }, false))
					{
						ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f,viewport->Size.y * 0.03f });
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
						tex = tex_map["Assets/BackMenuBtn.png"];

						if (ImGui::ImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }))
						{
							OverlayOpen = false;
							Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>();
						}

						ImGui::SameLine();
						ImGui::Dummy(ImVec2{ 10.0f, 0.0f });
						ImGui::SameLine();

						tex = tex_map["Assets/QuickMenuBtn.png"];

						if (ImGui::ImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }))
						{
							Service<EventManager>::Get().instant_dispatch<QuickMenuPopupTrigger>(QUICKMENU_POPUP_TYPE::UNITS);
						}

						ImGui::PopStyleColor(3);
					}
					ImGui::EndChild();
				}
			
			}
			ImGui::End();
		}
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


			// context
			// ---------
			ax::NodeEditor::Suspend();

			if (ax::NodeEditor::ShowBackgroundContextMenu())
			{
				mouse = ImGui::GetMousePos();
				ImGui::OpenPopup("Create New Node");
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));

			draw_background_context(g, instance);

			ImGui::PopStyleVar();

			ax::NodeEditor::Resume();
			// ---------


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
			// ---------
			ax::NodeEditor::End();
			// -----------------------------------------------------------------

			for (auto& [node_id, node_ptr] : g.get_nodes())
			{
				auto v = ax::NodeEditor::GetNodePosition(node_id);
				node_ptr->position.x = v.x;
				node_ptr->position.y = v.y;

			}
		}
	}

	void AttackSystemOverlay::draw_node(node_ptr n, const graph& g, Instance& instance)
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
				tc::Statline* statline = nullptr;
				for (auto i : instance.ecs.view<tc::Statline>())
					statline = instance.ecs.get_if<tc::Statline>(i);

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
				default:
					name = "If you see this, the node is corrupted";
					break;
				}

				ImGui::Text(name.c_str());
				twidth = ImGui::CalcTextSize(name.c_str()).x;
			}
			else
				ImGui::Text(n->get_name().c_str());
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

	void AttackSystemOverlay::draw_link(link l, pin_id_t from, pin_id_t to, pin_type type)
	{
		auto color = get_pin_color(type);

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
					ImGui::Indent(10.f);
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
					ImGui::Unindent(10.f);
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


	void AttackSystemOverlay::draw_background_context(graph& g, Instance& instance)
	{
		if (ImGui::BeginPopup("Create New Node", ImGuiWindowFlags_NoResize))
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

			ImGui::BeginChild("Nodes", ImVec2(350.f, 300.f), false, ImGuiWindowFlags_NoResize);

			node_context<ArithmeticNode>(g, "Operators");
			node_context<DiceNode>(g, "Dice Rolls");
			node_context<SwitchNode>(g, "Switches");
			node_context<CompareNode>(g, "Compare");


			string filter_string = filter.InputBuf;

			// attacker stats
			if (filter_string.empty())
			{
				if (ImGui::TreeNodeEx("Get Stats"))
				{
					tc::Statline* statline = nullptr;
					for (auto i : instance.ecs.view<tc::Statline>())
						statline = instance.ecs.get_if<tc::Statline>(i);

					LOG_ASSERT(statline);

					std::string text = "Get Main";
					ImGui::Indent(10.f);
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
					ImGui::Unindent(10.f);

					for (auto i = 0; i < statline->size(); ++i)
					{
						if (!(*statline)(i)) continue;

						text = "Get " + (*statline)[i];
						ImGui::Indent(10.f);
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
						ImGui::Unindent(10.f);
					}
					ImGui::TreePop();
				}
			}
			else
			{
				tc::Statline* statline = nullptr;
				for (auto i : instance.ecs.view<tc::Statline>())
					statline = instance.ecs.get_if<tc::Statline>(i);

				LOG_ASSERT(statline);

				std::string text = "Get Main";
				if (filter.PassFilter(text.c_str()))
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


				for (auto i = 0; i < statline->size(); ++i)
				{
					if (!(*statline)(i)) continue;

					text = "Get " + (*statline)[i];
					if (!filter.PassFilter(text.c_str())) continue;


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
			}

			// graph specific context
			if (g.get_type() == graph_type::action)
			{
				if (filter_string.empty())
				{
					if (ImGui::TreeNodeEx("Output"))
					{
						ImGui::Indent(10.f);
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
						ImGui::Unindent(10.f);
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
						ImGui::Indent(10.f);
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
						ImGui::Unindent(10.f);
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
			ImGui::EndChild();

			ImGui::EndPopup();
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
						}
					}
					else if (e_pin->get_type() != pin_type::Flow && e_pin->is_linked())
					{
						UI::ShowLabel("+ Replace Value Link", ImColor(45, 32, 32, 180));
						if (ax::NodeEditor::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
						{
							g.remove_links_to_input_pin(e);
							g.add_link(s, e);
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

	ImColor AttackSystemOverlay::get_pin_color(pin_type type)
	{
		switch (type)
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

	ax::Drawing::IconType AttackSystemOverlay::get_pin_icon(pin_type type)
	{
		switch (type)
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