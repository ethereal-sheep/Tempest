/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "../Util/node.h"
#include "test_scripting.h"
#include "../Util/var_data.h"

namespace Tempest
{
	// test create and pin to component
	void testing_node_fn_1()
	{
		auto in = false;
		size_t ind = 21;
		id_t id = 5;

		auto new_pin_id = create_pin_id(in, ind, id);

		auto [input, index, parent] = pin_to_component(new_pin_id);
		LOG_ASSERT(input == in);
		LOG_ASSERT(index == ind);
		LOG_ASSERT(parent == id);
	}

	// test input output flow values correct
	void testing_node_fn_2()
	{
		auto node = test_node::create_node("testing2");
		LOG_ASSERT(node->get_num_input_flows() == 1);
		LOG_ASSERT(node->get_num_output_flows() == 1);

		LOG_ASSERT(node->get_num_input_values() == 3);
		LOG_ASSERT(node->get_num_output_values() == 3);
	}

	// test input output flow values correct
	void testing_node_fn_3()
	{
		auto node = test_node::create_node("testing2");
		LOG_ASSERT(
			node->get_num_input_flows() + node->get_num_input_values() 
			== node->get_inputs().size());
		LOG_ASSERT(node->get_num_output_flows() + node->get_num_output_values()
			== node->get_outputs().size());
	}

	// test type correctness of input/output (TRIVIAL)
	void testing_node_fn_4()
	{
		auto node = test_node::create_node("testing2");

		for (auto& pin : node->get_inputs())
			static_assert(pin.is_input == true);
		for (auto& pin : node->get_inputs())
			static_assert(pin.is_output == false);

		for (auto& pin : node->get_outputs())
			static_assert(pin.is_output == true);
		for (auto& pin : node->get_outputs())
			static_assert(pin.is_input == false);

	}

	// test id correctness of pins
	void testing_node_fn_5()
	{
		auto node = test_node::create_node("testing2");

		auto& inputs = node->get_inputs();

		for (auto i = 0; i < inputs.size(); ++i)
		{
			auto [input, index, parent] = pin_to_component(inputs[i]);
			LOG_ASSERT(input == inputs[i].is_input);
			LOG_ASSERT(index == i);
			LOG_ASSERT(parent == node->get_id());
		}
	}


	//node.add_input(pin_type::Flow, "");
	//node.add_input(pin_type::Float, "float input");
	//node.add_input(pin_type::Int, "int input");
	//node.add_input(pin_type::String, "string input");

	//node.add_output(pin_type::Flow, "");
	//node.add_output(pin_type::Int, "int output");
	//node.add_output(pin_type::Float, "float output");
	//node.add_output(pin_type::String, "string output");

	// test type correctness of pins
	void testing_node_fn_6()
	{
		auto node = test_node::create_node("testing2");

		auto& inputs = node->get_inputs();
		auto& outputs = node->get_outputs();

		LOG_ASSERT(inputs[0].get_type() == pin_type::Flow);
		LOG_ASSERT(inputs[1].get_type() == pin_type::Float);
		LOG_ASSERT(inputs[2].get_type() == pin_type::Int);
		LOG_ASSERT(inputs[3].get_type() == pin_type::String);

		LOG_ASSERT(outputs[0].get_type() == pin_type::Flow);
		LOG_ASSERT(outputs[1].get_type() == pin_type::Int);
		LOG_ASSERT(outputs[2].get_type() == pin_type::Float);
		LOG_ASSERT(outputs[3].get_type() == pin_type::String);
	}

	// test factory
	void testing_node_fn_7()
	{
		auto node = CreateNode("", "");
		LOG_ASSERT(node == nullptr);
	}
	void testing_node_fn_8()
	{
		auto node = CreateNode("test", "testing2");

		LOG_ASSERT(node->get_num_input_flows() == 1);
		LOG_ASSERT(node->get_num_output_flows() == 1);

		LOG_ASSERT(node->get_num_input_values() == 3);
		LOG_ASSERT(node->get_num_output_values() == 3);

		auto& inputs = node->get_inputs();
		auto& outputs = node->get_outputs();

		for (auto i = 0; i < inputs.size(); ++i)
		{
			auto [input, index, parent] = pin_to_component(inputs[i]);
			LOG_ASSERT(input == inputs[i].is_input);
			LOG_ASSERT(index == i);
			LOG_ASSERT(parent == node->get_id());
		}

		LOG_ASSERT(inputs[0].get_type() == pin_type::Flow);
		LOG_ASSERT(inputs[1].get_type() == pin_type::Float);
		LOG_ASSERT(inputs[2].get_type() == pin_type::Int);
		LOG_ASSERT(inputs[3].get_type() == pin_type::String);

		LOG_ASSERT(outputs[0].get_type() == pin_type::Flow);
		LOG_ASSERT(outputs[1].get_type() == pin_type::Int);
		LOG_ASSERT(outputs[2].get_type() == pin_type::Float);
		LOG_ASSERT(outputs[3].get_type() == pin_type::String);
	}
	void testing_node_fn_9()
	{
		auto node = CreateNode("test", "testing2");

		auto& inputs = node->get_inputs();
		auto& outputs = node->get_outputs();

		for (auto i = 0; i < inputs.size(); ++i)
		{
			auto [input, index, parent] = pin_to_component(inputs[i]);
			LOG_ASSERT(inputs[i] != parent);
		}
		for (auto i = 0; i < outputs.size(); ++i)
		{
			auto [input, index, parent] = pin_to_component(outputs[i]);
			LOG_ASSERT(outputs[i] != parent);
		}
	}

	void testing_node_fn()
	{
		testing_node_fn_1();
		testing_node_fn_2();
		testing_node_fn_3();
		testing_node_fn_4();
		testing_node_fn_5();
		testing_node_fn_6();
		//testing_node_fn_7();
		testing_node_fn_8();
		testing_node_fn_9();
	}
}