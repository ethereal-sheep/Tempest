/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "node.h"
#include "Instance/Instance.h"

namespace Tempest
{
	script* node::create_script_pack(Entity entity, Instance& instance)
	{
		auto s = create_script(entity, instance);

		for (int i = 0; i < inputs.size(); ++i)
		{
			auto& pin = inputs[i];
			if (pin.is_linked() == false)
			{
				script* data = nullptr;

				switch (pin.get_type())
				{
				case Tempest::pin_type::Bool:
					data = instance.srm.add_script(CreateDataScript<bool>(pin.default_var.get<bool>()));
					break;
				case Tempest::pin_type::Byte:
					data = instance.srm.add_script(CreateDataScript<uint8_t>(pin.default_var.get<uint8_t>()));
					break;
				case Tempest::pin_type::Int:
					data = instance.srm.add_script(CreateDataScript<int>(pin.default_var.get<int>()));
					break;
				case Tempest::pin_type::Int64:
					data = instance.srm.add_script(CreateDataScript<int64_t>(pin.default_var.get<int64_t>()));
					break;
				case Tempest::pin_type::Float:
					data = instance.srm.add_script(CreateDataScript<float>(pin.default_var.get<float>()));
					break;
				case Tempest::pin_type::String:
					data = instance.srm.add_script(CreateDataScript<string>(pin.default_var.get<string>()));
					break;
				case Tempest::pin_type::Vec2:
					data = instance.srm.add_script(CreateDataScript<vec2>(pin.default_var.get<vec2>()));
					break;
				case Tempest::pin_type::Vec3:
					data = instance.srm.add_script(CreateDataScript<vec3>(pin.default_var.get<vec3>()));
					break;
				case Tempest::pin_type::Vec4:
					data = instance.srm.add_script(CreateDataScript<vec4>(pin.default_var.get<vec4>()));
					break;
				case Tempest::pin_type::Entity:
					data = instance.srm.add_script(CreateDataScript<id_t>(entity)); // self
					break;
				case Tempest::pin_type::Vector: [[fallthrough]];
				case Tempest::pin_type::END: [[fallthrough]];
				case Tempest::pin_type::Flow: [[fallthrough]];
				default:
					continue;
				}

				s->set_input(i - flow_inputs, data, 0);

			}
		}
		return s;
	}
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

		writer.StartArray("PinInfo");
		for (int i = 0; i < inputs.size(); ++i)
		{
			if (!inputs[i].is_linked() &&
				inputs[i].get_type() != pin_type::Flow &&
				inputs[i].get_type() != pin_type::Vector)
			{
				LOG_ASSERT(inputs[i].default_var.get_type() == inputs[i].get_type());
				writer.StartObject();

				writer.Member("Index", i);
				writer.Member("Var", inputs[i].default_var);

				writer.EndObject();
			}
		}
		writer.EndArray();

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
			auto new_pin_id = create_pin_id(false, outputs.size(), id);

			outputs.emplace_back(new_pin_id, _type, _name);
			++non_flow_outputs;

			return new_pin_id;
		}
	}
	node_ptr CreateNode(const string& category, const string& type)
	{
		auto category_value = magic_enum::enum_cast<category_type>(category);

		if (!category_value.has_value())
			throw node_exception("CreateNode: Bad node category!");

		auto node = create_helper(category_value.value(), type);

		if(!node)
			throw node_exception(
				"CreateNode: Failed to create node! category_type: " + 
				category + 
				", inner_type: " + type);

		return std::move(node);
	}
	node_ptr CreateNode(category_type category, const string& type)
	{
		auto node = create_helper(category, type);
		if (!node)
			throw node_exception(
				"CreateNode: Failed to create node! category_type: " + 
				string(magic_enum::enum_name(category).data()) + 
				", inner_type: " + type);

		return std::move(node);
	}
}