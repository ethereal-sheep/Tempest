
#include "test_scripting.h"
#include "../Util/var_data.h"
#include "../Util/var_set.h"
#include "Util.h"
#include "Util/view.h"
#include "Scripting/SRM.h"
#include "Instance/NullTimeInstance.h"
#include "Scripting/Graph/system_graph.h"
#include "Events/Events/ScriptEvents.h"

namespace Tempest
{


	void TestingDice1()
	{
		NullTimeInstance instance;

		Entity t = instance.ecs.create();
		auto node = DiceNode::create_node("D6");
		node->get_input_pin(0)->default_var.set(5);
		auto script = node->create_script_pack(t, instance);

		script->operator()();
		int out = std::any_cast<int>(script->operator[](0));

		LOG("Out: {0}", out);
	}

	void TestingDice2()
	{
		NullTimeInstance instance;

		Entity t = instance.ecs.create();
		auto node = DiceNode::create_node("D10");
		auto node2 = DiceNode::create_node("D10");

		auto script = node->create_script(t, instance);
		auto script2 = node2->create_script(t, instance);
		script->set_next(script2);
		script2->set_input(0, script, 0);

		(*script)();

		int out = std::any_cast<int>(script2->operator[](0));

		LOG("Out2: {0}", out);

	}
	void TestingDice3()
	{
		NullTimeInstance instance;

		Entity t = instance.ecs.create();
		auto node = DiceNode::create_node("D10");
		auto node2 = ArithmeticNode::create_node("Plus");
		node2->get_input_pin(1)->default_var.set(5);

		auto script = node->create_script(t, instance);
		auto script2 = node2->create_script_pack(t, instance);
		script->set_next(script2);
		script2->set_input(0, script, 0);

		(*script)();

		int out = std::any_cast<int>(script2->operator[](0));

		LOG("OutPlus: {0}", out);

	}

	void TestingDice4()
	{
		NullTimeInstance instance;

		Entity t = instance.ecs.create();
		auto node = DiceNode::create_node("D10");
		auto node2 = ArithmeticNode::create_node("Minus");
		node2->get_input_pin(1)->default_var.set(9);

		auto script = node->create_script(t, instance);
		auto script2 = node2->create_script_pack(t, instance);
		script->set_next(script2);
		script2->set_input(0, script, 0);

		(*script)();

		int out = std::any_cast<int>(script2->operator[](0));

		LOG("OutMinus: {0}", out);

	}
	void TestingDice5()
	{
		NullTimeInstance instance;

		Entity t = instance.ecs.create();
		auto node = DiceNode::create_node("D10");
		auto node2 = ArithmeticNode::create_node("Multiply");
		node2->get_input_pin(1)->default_var.set(9);

		auto script = node->create_script(t, instance);
		auto script2 = node2->create_script_pack(t, instance);
		script->set_next(script2);
		script2->set_input(0, script, 0);

		(*script)();

		int out = std::any_cast<int>(script2->operator[](0));

		LOG("OutMultiply: {0}", out);

	}
	void TestingDice6()
	{
		NullTimeInstance instance;

		Entity t = instance.ecs.create();
		auto node = DiceNode::create_node("D10");
		auto node2 = ArithmeticNode::create_node("Divide");
		node2->get_input_pin(1)->default_var.set(2);

		auto script = node->create_script(t, instance);
		auto script2 = node2->create_script_pack(t, instance);
		script->set_next(script2);
		script2->set_input(0, script, 0);

		(*script)();

		int out = std::any_cast<int>(script2->operator[](0));

		LOG("OutDivide: {0}", out);

	}

	void TestingResolution()
	{
		NullTimeInstance instance;

		Entity global = instance.ecs.create();

		//const char* s = R"(S:\Development\Projects)";

		GMS gms;
		gms.new_graph(graph_type::action);


		for (auto& g : gms.get_graphs())
		{
			tmap<node_id_t, script*> map;
			// add variables
			for (auto& [name, type, var] : g.get_variables())
			{
				instance.srm.add_variable_to_id(global, name, var);
			}
			
			// add node scripts
			for (auto& [id, ptr] : g.get_nodes())
			{
				map[id] = ptr->create_script_pack(global, instance);
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
					map[s_parent]->set_next(map[e_parent]); // script of next script
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

		{
			int newNum = 12;
			instance.srm.instant_dispatch_to_id<Input>(global, INVALID, newNum);
			auto i = instance.srm.get_variable_to_id(global, "Output")->get<int>();
			LOG_ASSERT(i == newNum);
		}
		
		{
			int newNum = 234323;
			instance.srm.instant_dispatch_to_id<Input>(global, INVALID, newNum);
			auto i = instance.srm.get_variable_to_id(global, "Output")->get<int>();
			LOG_ASSERT(i == newNum);
		}
	}

	void retest_graphs()
	{
		//ECS ecs;
		//auto global = ecs.create();

		
	}

	void TestingDice()
	{
		/*TestingDice1();
		TestingDice2();
		TestingDice3();
		TestingDice4();
		TestingDice5();
		TestingDice6();*/
		TestingResolution();
	}
}