#pragma once
#include "Instance/Instance.h"
#include "Util/magic_enum.hpp"
#include "Events/Events/ScriptEvents.h"

namespace Tempest
{
	class test_window2 : public Window
	{

		Entity current = INVALID;
		Entity conflict = INVALID;
		Entity attacking = INVALID;
		Entity defending = INVALID;

		ax::NodeEditor::EditorContext* context;
		ImVec2 mouse = ImVec2(0, 0);
		string simulate_val = "NULL";


		float border = 10.f;
		float swidth = 250.f;
		float min_swidth = 50.f;
		float max_swidth = 500.f;

		const char* window_name() override
		{
			return "test_window2";
		}

		void init(Instance&) override
		{
			context = ax::NodeEditor::CreateEditor();
			ax::NodeEditor::SetCurrentEditor(context);


			auto& editorStyle = ax::NodeEditor::GetStyle();

			editorStyle.Colors[0] = { 0,0,0,0 };
			editorStyle.Colors[2] = { 0,0,0,200/255.f };
		}

		void exit(Instance&) override
		{
			ax::NodeEditor::DestroyEditor(context);
		}

		void try_system(Instance& instance)
		{
			// assuming built
			instance.srm.instant_dispatch_to_id<Simulate>(conflict, attacking, defending);
			if (auto var = instance.srm.get_variable_to_id(conflict, "Win"))
			{
				LOG_ASSERT(var->get_type() == pin_type::Int);
				simulate_val = var->get<int>() ? "Win" : "Lose";
			}
		}

		void try_build_all(Instance& instance)
		{
			instance.srm.clear();
			for (auto id : instance.ecs.view<tc::Graph>())
			{
				auto& sys = instance.ecs.get<tc::Graph>(id);
				try_build(id, sys.g, instance);
			}
		}

		void try_build(Entity id, graph& g, Instance& instance)
		{
			tmap<node_id_t, script*> map;

			// add variables
			for (auto& [name, type, var] : g.get_variables())
			{
				instance.srm.add_variable_to_id(id, name, var);
			}

			// add node scripts
			for (auto& [node_id, ptr] : g.get_nodes())
			{
				map[node_id] = ptr->create_script_pack(id, instance);
			}

			// link scripts
			for (auto [start, end] : g.get_links_as_split())
			{
				auto [x, s_index, s_parent] = pin_to_component(start);
				auto [y, e_index, e_parent] = pin_to_component(end);

				LOG_ASSERT(map.count(s_parent));
				LOG_ASSERT(map.count(e_parent));

				auto s_node = g.get_node(s_parent);
				auto e_node = g.get_node(e_parent);

				auto pin = s_node->get_output_pin(s_index);
				auto type = pin->get_type();

				if (type == pin_type::Flow)
				{
					map[s_parent]->set_next(map[e_parent], s_index); // script of next script
				}
				else
				{
					map[e_parent]->set_input(
						e_index - e_node->get_num_input_flows(), // index of input pin in script
						map[s_parent], // script of prev script
						s_index - s_node->get_num_output_flows()); // index of out pin in prev script
				}
			}
		}

		void show(Instance& instance) override
		{
			if (ImGui::Begin(window_name(), &visible, window_flags))
			{

				draw_splitter();
				draw_sidebar(instance);
				ImGui::SameLine();

				draw_context(instance);

			}

			ImGui::End();


		}

		void draw_splitter()
		{
			ImVec2 backup_pos = ImGui::GetCursorPos();
			ImGui::SetCursorPosX(backup_pos.x + swidth);

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
			// We don't draw while active/pressed because as we move the panes the splitter button will be 1 frame late
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.6f, 0.6f, 0.10f));
			ImGui::Button("##Splitter", ImVec2(border, -1.f));
			ImGui::PopStyleColor(3);

			ImGui::SetItemAllowOverlap(); // This is to allow having other buttons OVER our splitter. 

			if (ImGui::IsItemActive())
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
				float mouse_delta = ImGui::GetIO().MouseDelta.x;

				// Minimum pane size
				if (mouse_delta < min_swidth - swidth)
					mouse_delta = min_swidth - swidth;
				if (mouse_delta > max_swidth - swidth)
					mouse_delta = max_swidth - swidth;

				// Apply resize
				swidth += mouse_delta;
			}
			else
			{
				if (ImGui::IsItemHovered())
					ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
				else
					ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
			}

			ImGui::SetCursorPos(backup_pos);
		}

		void draw_sidebar(Instance& instance)
		{
			if (ImGui::BeginChild("##NodeEditorSideBar", ImVec2(swidth, -10.f), false,
				ImGuiWindowFlags_MenuBar))
			{

				// add test graph
				if (ImGui::Button("Add Action"))
				{
					auto i = instance.ecs.create();
					instance.ecs.emplace<tc::ActionGraph>(i);
					instance.ecs.emplace<tc::Graph>(i, "Action", graph_type::action);
				}

				ImGui::SameLine();
				if (ImGui::Button("Add Conflict"))
				{
					auto i = instance.ecs.create();
					instance.ecs.emplace<tc::ConflictGraph>(i);
					instance.ecs.emplace<tc::Graph>(i, "Conflict", graph_type::conflict);
				}

				// navigate to content
				if (ImGui::Button("Navigate to Content"))
				{
					ax::NodeEditor::NavigateToContent();
				}


				UI::PaddedSeparator(0.5f);


				// UI elements
				// -----------------------------------------------------------------
				if (ImGui::Button((string{ "Build" }).c_str()))
				{
					try_build_all(instance);
				}
				if (ImGui::Button((string{ "Simulate" }).c_str()))
				{
					try_system(instance);
				}
				ImGui::SameLine();
				ImGui::Text("Output: %s", simulate_val.c_str());

				ImGui::InputScalar("Conflict", ImGuiDataType_U32, &conflict);

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SelectConflict"))
					{
						IM_ASSERT(payload->DataSize == sizeof(Entity));
						conflict = *static_cast<Entity*>(payload->Data);
					}
				}


				ImGui::InputScalar("Attacking", ImGuiDataType_U32, &attacking);

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SelectUnit"))
					{
						IM_ASSERT(payload->DataSize == sizeof(Entity));
						attacking = *static_cast<Entity*>(payload->Data);
					}
				}

				ImGui::InputScalar("Defending", ImGuiDataType_U32, &defending);
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SelectUnit"))
					{
						IM_ASSERT(payload->DataSize == sizeof(Entity));
						defending = *static_cast<Entity*>(payload->Data);
					}
				}

				UI::PaddedSeparator(0.5f);


				if (ImGui::TreeNodeEx("Conflict Graphs"))
				{
					for (auto id : instance.ecs.view<tc::ConflictGraph>())
					{
						std::string text = std::to_string(id);
						ImGui::Indent(10.f);
						if (ImGui::Selectable(text.c_str(), current == id))
						{
							current = id;
						}
						if (ImGui::BeginDragDropSource())
						{

							static Entity payload = 0;
							payload = id;
							ImGui::SetDragDropPayload("SelectConflict", &payload, sizeof(Entity));

							ImGui::Text("%u", id);
							ImGui::EndDragDropSource();
						}
						ImGui::Unindent(10.f);
					}
					ImGui::TreePop();
				}



				if (ImGui::TreeNodeEx("Action Graphs"))
				{
					for (auto id : instance.ecs.view<tc::ActionGraph>())
					{
						std::string text = std::to_string(id);
						ImGui::Indent(10.f);
						if (ImGui::Selectable(text.c_str(), current == id))
						{
							current = id;
						}
						ImGui::Unindent(10.f);
					}
					ImGui::TreePop();
				}

				UI::PaddedSeparator(0.5f);

				if (ImGui::TreeNodeEx("Units"))
				{
					for (auto id : instance.ecs.view<tc::Character>())
					{
						std::string text = std::to_string(id);
						ImGui::Indent(10.f);
						ImGui::Selectable(text.c_str());
						if (ImGui::BeginDragDropSource())
						{

							static Entity payload = 0;
							payload = id;
							ImGui::SetDragDropPayload("SelectUnit", &payload, sizeof(Entity));

							ImGui::Text("%u", id);
							ImGui::EndDragDropSource();
						}
						ImGui::Unindent(10.f);
					}
					ImGui::TreePop();
				}

				UI::PaddedSeparator(0.5f);


				auto& editorStyle = ax::NodeEditor::GetStyle();
				for (int i = 0; i < ax::NodeEditor::StyleColor_Count; ++i)
				{
					auto name = ax::NodeEditor::GetStyleColorName((ax::NodeEditor::StyleColor)i);

					ImGui::ColorEdit4(name, &editorStyle.Colors[i].x, ImGuiColorEditFlags_RGB);
				}

			}
			ImGui::EndChild();
		}

		void draw_context(Instance& instance)
		{
			if (current)
			{
				// graphs

				auto& sys = instance.ecs.get<tc::Graph>(current);
				auto& g = sys.g;

				for (auto& [node_id, node_ptr] : g.get_nodes())
				{
					ax::NodeEditor::SetNodePosition(node_id, ImVec2(node_ptr->position));
				}

				// -----------------------------------------------------------------

				// Canvas
				// -----------------------------------------------------------------
				ax::NodeEditor::Begin("Test");

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
					draw_node(node_ptr, instance);
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
					node_ptr->position = els::to_vec2(ax::NodeEditor::GetNodePosition(node_id));
				}
			}
		}

		void draw_node(node_ptr n, Instance& instance)
		{
			auto id = n->get_id();
			ax::NodeEditor::BeginNode(id);

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

				if (auto gn = dynamic_cast<ActionGraphNode*>(n.get()))
				{
					auto gid = gn->graph_entity;

					ImGui::Text("%s: %u", instance.ecs.get<tc::Graph>(gid).g.get_name().c_str(), gid);
				}
				else if (auto gsn = dynamic_cast<GetStatNode*>(n.get()))
				{
					tc::Statline* statline = nullptr;
					for (auto i : instance.ecs.view<tc::Statline>())
						statline = instance.ecs.get_if<tc::Statline>(i);

					string s = magic_enum::enum_name(gsn->get_type()).data();
					auto index = std::stoi(gsn->get_name());

					string name = "Get " + s + " " + (*statline)[index];
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
				draw_input_pin(input);
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

		void draw_link(link l, pin_id_t from, pin_id_t to, pin_type type)
		{
			auto color = get_pin_color(type);

			ax::NodeEditor::Link(l, from, to, color, 2.f);
		}

		void draw_input_pin(const input_pin& p)
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
				static int uone = 1;
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
			else
			{
				ImGui::SameLine();
				ImGui::Dummy({ 90.f, 0.1f });
			}
		}

		void draw_output_pin(const output_pin& p, float owidth)
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
		void node_context(
			graph& g,
			const char* title)
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


		void draw_background_context(graph& g, Instance& instance)
		{
			if (ImGui::BeginPopup("Create New Node", ImGuiWindowFlags_NoResize))
			{
				ImGui::Text("All Actions For This Blueprint");

				ImGui::Text("Filter:");
				ImGui::SameLine();
				UI::HelpMarker("Filter Usage:\n"
					"  \"xxx\"      display lines containing \"xxx\"\n"
					"  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
					"  \"-xxx\"     hide lines containing \"xxx\"");
				ImGui::SameLine();
				//m_SearchFilter.Draw("", 280.f);

				ImGui::Separator();

				ImGui::BeginChild("Nodes", ImVec2(350.f, 300.f), false, ImGuiWindowFlags_NoResize);

				node_context<ArithmeticNode>(g, "Arithmetic");
				node_context<DiceNode>(g, "Dice");
				node_context<SwitchNode>(g, "Switch");
				node_context<CompareNode>(g, "Compare");


				if (ImGui::TreeNodeEx("Get Attacker Stats"))
				{

					tc::Statline* statline = nullptr;
					for (auto i : instance.ecs.view<tc::Statline>())
						statline = instance.ecs.get_if<tc::Statline>(i);

					for (auto i = 0; i < statline->size(); ++i)
					{
						if (!(*statline)(i)) continue;

						std::string text = "Get Attacker " + (*statline)[i];
						ImGui::Indent(10.f);
						if (ImGui::Selectable(text.c_str()))
						{
							auto node = g.add_node(GetStatNode::create_node(string("Attacker:") + std::to_string(i)));

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

				if (ImGui::TreeNodeEx("Get Defender Stats"))
				{
					tc::Statline* statline = nullptr;
					for (auto i : instance.ecs.view<tc::Statline>())
						statline = instance.ecs.get_if<tc::Statline>(i);

					for (auto i = 0; i < statline->size(); ++i)
					{
						if (!(*statline)(i)) continue;

						std::string text = "Get Attacker " + (*statline)[i];
						ImGui::Indent(10.f);
						if (ImGui::Selectable(text.c_str()))
						{
							auto node = g.add_node(GetStatNode::create_node(string("Defender:") + std::to_string(i)));

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


				if (g.get_type() == graph_type::action)
				{
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
				}
				else if (g.get_type() == graph_type::conflict)
				{
					if (ImGui::TreeNodeEx("Action Graphs"))
					{
						for (auto i : instance.ecs.view<tc::ActionGraph>())
						{
							auto& agtc = instance.ecs.get<tc::Graph>(i);
							std::string text = agtc.g.get_name() + ": " + std::to_string(i);
							ImGui::Indent(10.f);
							if (ImGui::Selectable(text.c_str()))
							{
								auto node = g.add_node(ActionGraphNode::create_node(std::to_string(i)));

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

				}
				ImGui::EndChild();

				ImGui::EndPopup();
			}
		}

		void update_create(graph& g)
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

		void update_delete(graph& g)
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
						if (n->get_name() != "Input")
							g.remove_node((node_id_t)nodeId.Get());
					}
				}
			}
			ax::NodeEditor::EndDelete();
		}

		ImColor get_pin_color(pin_type type)
		{
			switch (type)
			{
			case pin_type::Flow: return ImColor(192, 192, 192);
			case pin_type::Bool: return ImColor(128, 0, 0); // maroon
			case pin_type::Byte: return ImColor(0, 82, 83); // sherpa blue
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

		ax::Drawing::IconType get_pin_icon(pin_type type)
		{
			switch (type)
			{
			case pin_type::Flow: return ax::Drawing::IconType::Flow;				break;
			case pin_type::Bool: return ax::Drawing::IconType::Diamond;				break;
			case pin_type::Byte: return ax::Drawing::IconType::Circle;				break;
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

	};
}