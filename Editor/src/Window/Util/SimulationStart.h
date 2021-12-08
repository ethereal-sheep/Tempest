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

	class SimulationStart : public Window
	{
		const int max_thread = 10;
		Entity attacker = INVALID;
		Entity defender = INVALID;
		Entity attack_action = INVALID;
		Entity defend_action = INVALID;
		Entity conflict = INVALID;
		uint32_t frequency = 1;
		std::atomic_uint32_t* win = nullptr;
		std::atomic_uint32_t* lose = nullptr;
		std::atomic_int32_t* attack = nullptr;
		std::atomic_int32_t* defend = nullptr;
		std::atomic_bool* finish = nullptr;

		enum struct State {
			DONE,
			LOAD,
			RUN
		};

		State state = State::DONE;

		tvector<future_bool> futures;
		tvector<uint32_t> wins;
		tvector<uint32_t> loses;
		tvector<int> attacks;
		tvector<int> defends;
		tvector<std::function<void()>> fn;

		// graphs to clear up after we are done
		tvector<Entity> extra;

		// 

		const char* window_name() override
		{
			return "";
		}

		void init(Instance&) override
		{
			Service<EventManager>::Get().register_listener<SimulateConflict>(&SimulationStart::start_simulation, this);
		}

		void start_simulation(const Event& e)
		{
			if (state == State::DONE)
			{
				auto& a = event_cast<SimulateConflict>(e);
				state = State::LOAD;
				attacker = a.atker;
				defender = a.defer;
				attack_action = a.atk_act;
				defend_action = a.def_act;
				conflict = a.conflict;
				frequency = a.freq;
				win = &a.win;
				lose = &a.lose;
				finish = &a.finish;
				attack = &a.attack;
				defend = &a.defend;

				win->store(0);
				lose->store(0);
				finish->store(false);
			}
		}

		void show(Instance& instance) override
		{
			switch (state)
			{
			case State::DONE:
				break;
			case State::LOAD:
				setup_simulate(instance);
				state = State::RUN;
				//ImGui::OpenPopup("Simulating Conflict");
				[[fallthrough]];
			case State::RUN:
			{
				const auto wrap_width = 10.f;
				// popup
				/*ImVec2 center = ImGui::GetMainViewport()->GetCenter();
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
				}*/

				bool still_running = false;
				for (int i = 0; i < max_thread; ++i)
				{
					if (futures[i].wait_for(std::chrono::seconds(0)) == std::future_status::ready)
					{
						/*if (win) *win += wins[i];
						if (lose) *lose += loses[i];*/
					}
					else
						still_running = true;
				}

				if (!still_running)
				{
					state = State::DONE;
					if(finish) finish->store(true);
					clean_simulate(instance);
					ImGui::CloseCurrentPopup();
				}

				break;
			}
			default:
				break;
			}

		}

		void running(Instance&)
		{

		}

		void setup_simulate(Instance& instance)
		{
			// setup 10 vars for 10 threads
			futures.resize(max_thread);
			fn.resize(max_thread);
			wins.resize(max_thread);
			loses.resize(max_thread);
			attacks.resize(max_thread);
			defends.resize(max_thread);

			for (int i = 0; i < max_thread; ++i)
			{
				// create 5 entities
				auto a = instance.ecs.clone(attacker); // atker entity
				auto d = instance.ecs.clone(defender); // defender entity
				auto aa = instance.ecs.clone(attack_action); // atker entity
				auto da = instance.ecs.clone(defend_action); // defender entity
				auto s = instance.ecs.clone(conflict); // sequence entity

				extra.push_back(a);
				extra.push_back(d);
				extra.push_back(s);
				extra.push_back(aa);
				extra.push_back(da);


				fn[i] = [
					&instance, 
					&num_win = *win, &num_lose = *lose,
					&num_attack = *attack, &num_defend = *defend,
					freq = frequency / max_thread, 
					a, d, aa, da, s
				]() {
					for (unsigned i = 0; i < freq; ++i)
					{
						instance.srm.instant_dispatch_to_id<Simulate>(s, a, d, aa, da);
						if (auto var = instance.srm.get_variable_to_id(s, "Win"))
						{
							var->get<int>() ? ++num_win : ++num_lose;
						}
						if (auto var = instance.srm.get_variable_to_id(s, "AttackRollOutput"))
						{
							num_attack += var->get<int>();
						}
						if (auto var = instance.srm.get_variable_to_id(s, "DefendRollOutput"))
						{
							num_defend += var->get<int>();
						}
					}
				};
			}

			// build all extras
			try_build_all(instance);

			// for all fns we submit
			for (int i = 0; i < max_thread; ++i)
			{
				futures[i] = Service<thread_pool>::Get().submit_task(fn[i]);
			}

		}

		void clean_simulate(Instance& instance)
		{
			for (auto id : extra)
			{
				instance.ecs.destroy(id);
			}
			extra.clear();
			futures.clear();
			fn.clear();
			wins.clear();
			loses.clear();
		}

		void try_system(Instance&)
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