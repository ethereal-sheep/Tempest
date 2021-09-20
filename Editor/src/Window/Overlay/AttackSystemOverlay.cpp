#include "CombatModeOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"
#include "AttackSystemOverlay.h"
#include "Util/UIElements.h"

namespace Tempest
{
	void AttackSystemOverlay::open_popup(const Event& e)
	{
		auto a = event_cast<OpenActionGraphTrigger>(e);

		if (a.instance.ecs.has<tc::Graph>(a.id))
		{
			id = a.id;
			OverlayOpen = true;
			temp_graph = a.instance.ecs.get<tc::Graph>(id).g;
		}


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
				const float graph_context_height = ImGui::GetWindowHeight() * 0.7f;

				//auto& g = instance.ecs.get<tc::Graph>(id);
				
				if (ImGui::BeginTable("graph header table", 3))
				{
					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(1);
					UI::Header_1("Attack System");

					ImGui::PushFont(FONT_HEAD);

					ImGui::TableSetColumnIndex(0);
					ImGui::Dummy({ 0.1f, title_top_padding });
					ImGui::Dummy({ title_left_padding, 0.1f });
					ImGui::SameLine();
					static string test = "Testing";
					ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0,0,0 });
					ImGui::InputText("##testing", &temp_graph.name);
					ImGui::PopStyleColor();
					//ImGui::Text(g.g.get_name().c_str());
					ImGui::PopFont();


					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Dummy({ 0.1f, title_bottom_padding });

					ImGui::EndTable();
				}


				ImGui::Separator();

				// draw the context
				ImGui::PushStyleColor(ImGuiCol_Border, { 0,0,0,0 });
				draw_context(instance, graph_context_height);
				ImGui::PopStyleColor();
				ImGui::Separator();


				float button_y = ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.3f;
				// first button pos
				ImVec2 pos1 = {ImGui::GetContentRegionAvailWidth() - 480.f, button_y};

				// second button pos
				ImVec2 pos2 = { ImGui::GetContentRegionAvailWidth() - 300.f, button_y };

				// third button pos
				ImVec2 pos3 = { ImGui::GetContentRegionAvailWidth() - 120.f, button_y };



				if (UI::UIButton_1("Navigate to Content", "Navigate to Content", pos1, { 50.f, 10.f }, FONT_PARA))
				{
					ax::NodeEditor::NavigateToContent();
				}
				if (UI::UIButton_1("Save & Close", "Save & Close", pos2, { 50.f, 10.f }, FONT_PARA))
				{
					OverlayOpen = false;
					instance.ecs.get<tc::Graph>(id).g = temp_graph;
					Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>();
				}

				if (UI::UIButton_1("Close", "Close", pos3, { 50.f, 10.f }, FONT_PARA))
				{
					OverlayOpen = false;
					Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>();
				}

				

				//ImGui::SameLine();
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
				ax::NodeEditor::SetNodePosition(node_id, ImVec2(node_ptr->position));
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

	void AttackSystemOverlay::draw_node(node_ptr n, Instance& instance)
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

	void AttackSystemOverlay::draw_link(link l, pin_id_t from, pin_id_t to, pin_type type)
	{
		auto color = get_pin_color(type);

		ax::NodeEditor::Link(l, from, to, color, 2.f);
	}

	void AttackSystemOverlay::draw_input_pin(const input_pin& p)
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
			}
			else
			{
				tc::Statline* statline = nullptr;
				for (auto i : instance.ecs.view<tc::Statline>())
					statline = instance.ecs.get_if<tc::Statline>(i);

				LOG_ASSERT(statline);

				for (auto i = 0; i < statline->size(); ++i)
				{
					if (!(*statline)(i)) continue;

					std::string text = "Get Attacker " + (*statline)[i];
					if (!filter.PassFilter(text.c_str())) continue;


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
				}
			}

			// defender stats
			if (filter_string.empty())
			{
				if (ImGui::TreeNodeEx("Get Defender Stats"))
				{
					tc::Statline* statline = nullptr;
					for (auto i : instance.ecs.view<tc::Statline>())
						statline = instance.ecs.get_if<tc::Statline>(i);

					LOG_ASSERT(statline);

					for (auto i = 0; i < statline->size(); ++i)
					{
						if (!(*statline)(i)) continue;

						std::string text = "Get Defender " + (*statline)[i];
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
			}
			else
			{
				tc::Statline* statline = nullptr;
				for (auto i : instance.ecs.view<tc::Statline>())
					statline = instance.ecs.get_if<tc::Statline>(i);

				LOG_ASSERT(statline);

				for (auto i = 0; i < statline->size(); ++i)
				{
					if (!(*statline)(i)) continue;

					std::string text = "Get Defender " + (*statline)[i];
					if (!filter.PassFilter(text.c_str())) continue;
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
						ImGui::Unindent(10.f);
						ImGui::TreePop();
					}
				}
				else
				{

					for (auto i : instance.ecs.view<tc::ActionGraph>())
					{
						auto& agtc = instance.ecs.get<tc::Graph>(i);
						std::string text = agtc.g.get_name() + ": " + std::to_string(i);

						if (!filter.PassFilter(text.c_str())) continue;

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
					}

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
					bool non_delete_nodes = n->get_name() != "Input" && n->get_name() != "Start";

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

	ax::Drawing::IconType AttackSystemOverlay::get_pin_icon(pin_type type)
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

}