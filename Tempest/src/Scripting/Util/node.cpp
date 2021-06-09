#include "node.h"

namespace Tempest
{
	Writer& node::serialize(Writer& writer) const
	{
		serialize_parent(writer);
		return writer;
	}

	Writer& node::serialize_parent(Writer& writer) const
	{
		writer.StartObject("Node");
		writer.Member("Id", id);
		// safe because writer.Member is guaranteed to not modify the value
		writer.Member("Size", const_cast<vec2&>(size));
		writer.Member("Position", const_cast<vec2&>(position));
		return writer.EndObject();
	}

	input_pin* node::get_input_pin(size_t index)
	{
		if (get_num_inputs() <= index)
			return nullptr;
		return &inputs[index];
	}

	output_pin* node::get_output_pin(size_t index)
	{
		if (get_num_outputs() <= index)
			return nullptr;
		return &outputs[index];
	}

	pin_id_t node::add_input(pin_type _type, const string& _name)
	{
		// check if flow
		if (_type == pin_type::Flow)
		{
			if (flow_inputs) // if flow exist already, do not add more
				return inputs[0].get_id();
			if (non_flow_inputs) // if flow does not exist, but there are inputs
				LOG_CRITICAL("Flow input cannot come after value inputs!"); 

			// else we can add the flow
			auto new_pin_id = create_pin_id(true, 0, id);

			inputs.emplace_back(new_pin_id, _type, _name);
			++flow_inputs; // increment no. of flows

			return new_pin_id;
		}
		else
		{
			auto new_pin_id = create_pin_id(true, inputs.size(), id);

			inputs.emplace_back(new_pin_id, _type, _name);
			++non_flow_inputs;

			return new_pin_id;
		}
	}
	pin_id_t node::add_output(pin_type _type, const string& _name)
	{
		// check if flow
		if (_type == pin_type::Flow)
		{
			if (non_flow_outputs) // flows can only come at the start
				LOG_CRITICAL("Flow outputs cannot come after value outputs!");

			// else we can add the flow
			auto new_pin_id = create_pin_id(false, outputs.size(), id);

			outputs.emplace_back(new_pin_id, _type, _name);
			++flow_outputs; // increment no. of flows

			return new_pin_id;
		}
		else
		{
			auto new_pin_id = create_pin_id(true, outputs.size(), id);

			outputs.emplace_back(new_pin_id, _type, _name);
			++non_flow_outputs;

			return new_pin_id;
		}
	}
	std::unique_ptr<node> CreateNode(const string& category, const string& type)
	{
		auto category_value = magic_enum::enum_cast<category_type>(category);

		if (!category_value.has_value())
			return nullptr;

		return create_helper(category_value.value(), type);
	}
}