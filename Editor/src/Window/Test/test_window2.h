#pragma once
#include "Instance/Instance.h"
#include "Util/magic_enum.hpp"
#include "Events/Events/ScriptEvents.h"

namespace Tempest
{
	class test_window2 : public Window
	{

		ax::NodeEditor::EditorContext* context;
		ImVec2 mouse = ImVec2(0,0);
		int simulate_val = 0;

		const char* window_name() override
		{
			return "test_window2";
		}

		void init(Instance& instance) override
		{
			context = ax::NodeEditor::CreateEditor();
			ax::NodeEditor::SetCurrentEditor(context);

			for (auto id : instance.ecs.view<tc::System>())
			{
				auto& sys = instance.ecs.get<tc::System>(id);
				auto& g = sys.g;

				for (auto& [node_id, node_ptr] : g.get_nodes())
				{
					ax::NodeEditor::SetNodePosition(node_id, ImVec2(node_ptr->position));
				}

			}
			
		}

		void exit(Instance&) override
		{
			ax::NodeEditor::DestroyEditor(context);
		}

		void try_system(Entity id, int value, Instance& instance)
		{
			// assuming built
			instance.srm.instant_dispatch_to_id<Input>(id, id, value);
			if (auto var = instance.srm.get_variable_to_id(id, "Output"))
			{
				LOG_ASSERT(var->get_type() == pin_type::Int);
				simulate_val = var->get<int>();
			}
		}

		void try_build(Entity id, graph& g, Instance& instance)
		{
			instance.srm.clear();
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
				if (ImGui::Button("Test"))
				{
					auto i = instance.ecs.create();
					instance.ecs.emplace<tc::System>(i);
				}
				ImGui::SameLine();
				if (ImGui::Button("Navigate to Content"))
				{
					ax::NodeEditor::NavigateToContent();
				}

				if (ImGui::BeginTabBar("Node Tab"))
				{
					for (auto id : instance.ecs.view<tc::System>())
					{
						if(ImGui::BeginTabItem(std::to_string(id).c_str()))
						{
							auto& sys = instance.ecs.get<tc::System>(id);
							auto& g = sys.g;

							if (ImGui::Button("Build"))
							{
								try_build(id, g, instance);
							}

							static int send = 0;
							static int uone = 1;
							ImGui::InputScalar("Input", ImGuiDataType_S32, &send, &uone);
							ImGui::SameLine();
							if (ImGui::Button("Simulate"))
							{
								try_system(id, send, instance);
							}
							ImGui::SameLine();
							ImGui::Text("Output: %d", simulate_val);

							ax::NodeEditor::Begin("Test");

							update_create(g);
							update_delete(g);

							ax::NodeEditor::Suspend();
							if (ax::NodeEditor::ShowBackgroundContextMenu())
							{
								mouse = ImGui::GetMousePos();
								ImGui::OpenPopup("Create New Node");
							}

							ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));

							draw_background_context(g);

							ImGui::PopStyleVar();

							ax::NodeEditor::Resume();


							for (auto& [node_id, ptr] : g.get_nodes())
							{
								draw_node(ptr);
							}

							for (auto [link_id, from, to] : g.get_links_as_tri())
							{
								auto [input, index, parent] = pin_to_component(from);
								auto node = g.get_node(parent);
								auto pin = node->get_output_pin(index);

								draw_link(link_id, from, to, pin->get_type());
							}
							ax::NodeEditor::End();
							ImGui::EndTabItem();
						}
					}
					ImGui::EndTabBar();
				}

				
				// ----------------------------------------------------------------

				//update_create(g);
				//update_delete(g);
				//update_delete(g);

				//ImGui::PushID(graph.GetName().c_str());

				//// draw nodes

				//for (auto& node : graph.GetNodes())
				//{
				//	draw_node(*node, graph);
				//}
				//// draw links
				//for (auto& link : graph.GetLinks())
				//{
				//	draw_link(link);
				//}

				//ImGui::PopID();

				// ----------------------------------------------------------------

				//DragDropTarget(graph);

			}

			ImGui::End();
		}

		void draw_node(node_ptr n)
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

			// Title
			ImGui::Text(n->get_name().c_str());
			// Update Position
			auto pos = ax::NodeEditor::GetNodePosition(id);
			n->position = { pos.x, pos.y };

			// Input group
			ImGui::BeginGroup();
			for (const auto& input : n->get_inputs())
			{
				draw_input_pin(input);
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

			ax::Drawing::IconType iconType;
			ImColor color = get_pin_color(p.get_type());
			color.Value.w = static_cast<float>(alpha) / 255.0f;
			switch (p.get_type())
			{
			case pin_type::Flow: iconType = ax::Drawing::IconType::Flow;				break;
			case pin_type::Bool: iconType = ax::Drawing::IconType::Diamond;				break;
			case pin_type::Byte: iconType = ax::Drawing::IconType::Circle;				break;
			case pin_type::Int: iconType = ax::Drawing::IconType::Circle;				break;
			case pin_type::Int64: iconType = ax::Drawing::IconType::Circle;				break;
			case pin_type::Float: iconType = ax::Drawing::IconType::Square;				break;
			case pin_type::String: iconType = ax::Drawing::IconType::Grid;				break;
			case pin_type::Vec2: iconType = ax::Drawing::IconType::RoundSquare;			break;
			case pin_type::Vec3: iconType = ax::Drawing::IconType::RoundSquare;			break;
			case pin_type::Vec4: iconType = ax::Drawing::IconType::RoundSquare;			break;
			case pin_type::Entity: iconType = ax::Drawing::IconType::Square;			break;
			case pin_type::Vector: iconType = ax::Drawing::IconType::Square;			break;
			default:
				return;
			}

			BeginPin(p.get_id(), ax::NodeEditor::PinKind::Input);
			ax::NodeEditor::PinPivotAlignment(ImVec2(0.5f, 0.5f));
			ax::NodeEditor::PinPivotSize(ImVec2(0.f, 0.f));


			ax::Widgets::Icon(ImVec2(24, 24), iconType, p.is_linked(), color, ImColor(32, 32, 32, alpha));
			ax::NodeEditor::EndPin();

			ImGui::SameLine();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.9f);
			ImGui::AlignTextToFramePadding();
			ImGui::Text(p.get_name().c_str());
			ImGui::SameLine();
			ImGui::Dummy({ 1.f, 0.f });

			if (p.get_type() == pin_type::Int && !p.is_linked())
			{
				ImGui::SameLine();
				static int uone = 1;
				int& a = p.default_var.get<int>();
				string s = "##";
				s += std::to_string(p.get_id());
				ImGui::SetNextItemWidth(90);

				ImGui::InputScalar(s.c_str(), ImGuiDataType_S32, &a, &uone);
			}
		}

		void draw_output_pin(const output_pin& p, float owidth)
		{
			int alpha = 255;

			ax::Drawing::IconType iconType;
			ImColor color = get_pin_color(p.get_type());
			color.Value.w = static_cast<float>(alpha) / 255.0f;
			switch (p.get_type())
			{
			case pin_type::Flow: iconType = ax::Drawing::IconType::Flow;				break;
			case pin_type::Bool: iconType = ax::Drawing::IconType::Diamond;				break;
			case pin_type::Byte: iconType = ax::Drawing::IconType::Circle;				break;
			case pin_type::Int: iconType = ax::Drawing::IconType::Circle;				break;
			case pin_type::Int64: iconType = ax::Drawing::IconType::Circle;				break;
			case pin_type::Float: iconType = ax::Drawing::IconType::Square;				break;
			case pin_type::String: iconType = ax::Drawing::IconType::Grid;				break;
			case pin_type::Vec2: iconType = ax::Drawing::IconType::RoundSquare;			break;
			case pin_type::Vec3: iconType = ax::Drawing::IconType::RoundSquare;			break;
			case pin_type::Vec4: iconType = ax::Drawing::IconType::RoundSquare;			break;
			case pin_type::Entity: iconType = ax::Drawing::IconType::Square;			break;
			case pin_type::Vector: iconType = ax::Drawing::IconType::Square;			break;
			default:
				return;
			}


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

		void draw_context(graph&)
		{
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


		void draw_background_context(graph& g)
		{


			if(ImGui::BeginPopup("Create New Node", ImGuiWindowFlags_NoResize))
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

				if (ImGui::Selectable("Output"))
				{
					auto node = g.add_node(SystemNode::create_node("Output"));

					if (node)
					{
						ax::NodeEditor::SetNodePosition(
							node->get_id(), ax::NodeEditor::ScreenToCanvas(mouse)
						);
					}
					ImGui::CloseCurrentPopup();
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
						if(n->get_name() != "Input")
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
			case pin_type::Int: return ImColor(46, 139, 87); // sea green
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

		
	};
}