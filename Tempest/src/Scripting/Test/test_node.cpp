
#include "../Util/node.h"

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
		case Tempest::test_node::inner_type::END:
			break;
		default:
			break;
		}

		return make_uptr<test_node>(std::move(node));
	}

	script* test_node::create_script(Entity entity [[maybe_unused]])
	{
		return nullptr;
	}

}