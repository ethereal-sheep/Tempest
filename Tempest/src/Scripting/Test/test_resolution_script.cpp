
#include "test_scripting.h"
#include "../Util/var_data.h"
#include "../Util/var_set.h"
#include "Util.h"
#include "Util/view.h"
#include "Scripting/SRM.h"
#include "Instance/NullTimeInstance.h"

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

	void TestingDice()
	{
		TestingDice1();
		TestingDice2();
		TestingDice3();
		TestingDice4();
	}
}