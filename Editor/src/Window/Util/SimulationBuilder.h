/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Instance/Instance.h"
#include "Util/magic_enum.hpp"
#include "Events/EventManager.h"
#include "Events/Events/ScriptEvents.h"
#include "Triggers/SimulationTriggers.h"
#include "Util/thread_pool.h"

#include "Font.h"


namespace Tempest
{
	class SimulationBuilder : public Window
	{

		Entity attacking = INVALID;
		Entity defending = INVALID;
		Entity conflict = INVALID;
		uint32_t frequency = 1;
		uint32_t* win = nullptr;
		uint32_t* lose = nullptr;

		enum struct State {
			DONE,
			LOAD,
			RUN
		};

		State state = State::DONE;
		future_bool future;

		uint32_t num_win = 0;
		uint32_t num_lose = 0;

		const char* window_name() override
		{
			return "";
		}

		void init(Instance&) override
		{
			Service<EventManager>::Get().register_listener<SimulateConflict>(&SimulationBuilder::start_simulation, this);
		}

		void start_simulation(const Event& )
		{
			/*if (state == State::DONE)
			{
				auto& a = event_cast<SimulateConflict>(e);
				state = State::LOAD;
				attacking = a.atk;
				defending = a.def;
				conflict = a.conflict;
				frequency = a.freq;
				win = &a.win;
				lose = &a.lose;

				num_win = 0;
				num_lose = 0;
			}*/
		}


		void show(Instance& instance) override
		{
			switch (state)
			{
			case Tempest::SimulationBuilder::State::DONE:
				break;
			case Tempest::SimulationBuilder::State::LOAD:
				try_build_all(instance);
				setup_simulate(instance);
				state = State::RUN;

				ImGui::OpenPopup("Simulating Conflict");
			case Tempest::SimulationBuilder::State::RUN:
			{
				const auto wrap_width = 10.f;
				// popup
				ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowSize(ImVec2(220.f, 100.f));
				ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
				if (ImGui::BeginPopupModal("Simulating Conflict", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImVec2 pos = ImGui::GetCursorScreenPos();
					ImVec2 marker_min = ImVec2(pos.x + wrap_width, pos.y);
					ImVec2 marker_max = ImVec2(pos.x + wrap_width + 10, pos.y + ImGui::GetTextLineHeight());
					ImGui::Text("\n");
					ImGui::Dummy(ImVec2{ wrap_width, 0.01f });
					ImGui::SameLine();
					ImGui::Text(ICON_FA_EXCLAMATION_CIRCLE);
					ImGui::SameLine();
					ImGui::Text("Loading...");
					ImGui::Text("\n");

					ImGui::EndPopup();
				}


				if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
				{
					if (win) *win = num_win;
					if (lose) *lose = num_lose;
					state = State::DONE;
					ImGui::CloseCurrentPopup();
				}
				break;
			}
			default:
				break;
			}

		}

		void running(Instance& )
		{
			
		}

		void setup_simulate(Instance& instance)
		{
			auto var = instance.srm.get_variable_to_id(conflict, "Win");
			LOG_ASSERT(var);
			LOG_ASSERT(var->get_type() == pin_type::Int);


			auto running = [&, var]() {
				for (unsigned i = 0; i < frequency; ++i)
				{
					//instance.srm.instant_dispatch_to_id<Simulate>(conflict, attacking, defending);
					var->get<int>() ? ++num_win : ++num_lose;
				}
			};

			future = Service<thread_pool>::Get().submit_task(running);
		}

		void try_system(Instance& )
		{
			// assuming built
			/*instance.srm.instant_dispatch_to_id<Simulate>(conflict, attacking, defending);
			if (auto var = instance.srm.get_variable_to_id(conflict, "Win"))
			{
				LOG_ASSERT(var->get_type() == pin_type::Int);
				simulate_val = var->get<int>() ? "Win" : "Lose";
			}*/
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
	};
}