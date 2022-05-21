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
#include "Instance/Instance.h"

namespace Tempest
{
	string test_node::get_type_string()
	{
		return magic_enum::enum_name(type).data();
	}

	node_ptr test_node::create_node(const std::string& typeinfo)
	{
		// parse type info
		auto type = magic_enum::enum_cast<test_node::inner_type>(typeinfo);
		if (!type.has_value())
		{
			return nullptr;
		}

		test_node node(type.value());

		switch (type.value())
		{
		case Tempest::test_node::inner_type::testing1:
			node.set_name("testing1");
			node.add_input(pin_type::Flow, "");
			node.add_output(pin_type::Flow, "");
			break;

		case Tempest::test_node::inner_type::testing2:
			node.set_name("testing2");
			node.add_input(pin_type::Flow, "");
			node.add_input(pin_type::Float, "float input");
			node.add_input(pin_type::Int, "int input");
			node.add_input(pin_type::String, "string input");

			node.add_output(pin_type::Flow, "");
			node.add_output(pin_type::Int, "int output");
			node.add_output(pin_type::Float, "float output");
			node.add_output(pin_type::String, "string output");
			break;

		case Tempest::test_node::inner_type::all:
			node.set_name("all");
			node.add_input(pin_type::Flow, "");
			node.add_input(pin_type::Bool, "Bool input");
			node.add_input(pin_type::Byte, "Byte input");
			node.add_input(pin_type::Int, "Int input");
			node.add_input(pin_type::Int64, "Int64 input");
			node.add_input(pin_type::Float, "Float input");
			node.add_input(pin_type::String, "String input");
			node.add_input(pin_type::Vec2, "Vec2 input");
			node.add_input(pin_type::Vec3, "Vec3 input");
			node.add_input(pin_type::Vec4, "Vec4 input");

			node.add_output(pin_type::Flow, "");
			node.add_output(pin_type::Byte, "Byte output");
			node.add_output(pin_type::Int, "Int output");
			node.add_output(pin_type::Int64, "Int64 output");
			node.add_output(pin_type::Float, "Float output");
			node.add_output(pin_type::String, "String output");
			node.add_output(pin_type::Vec2, "Vec2 output");
			node.add_output(pin_type::Vec3, "Vec3 output");
			node.add_output(pin_type::Vec4, "Vec4 output");
			break;
		default:
			return nullptr;
			break;
		}

		return make_uptr<test_node>(std::move(node));
	}

	script* test_node::create_script(Entity entity [[maybe_unused]], Instance& srm [[maybe_unused]] )
	{
		return nullptr;
	}

}