
#include "../Util/node.h"
#include "Instance/Instance.h"
#include "Events/Events/ScriptEvents.h"

namespace Tempest
{
	std::string ConflictNode::get_type_string()
	{
		// assuming no problem
		std::string toReturn = magic_enum::enum_name(type).data();

		return std::move(toReturn);
	}

	node_ptr ConflictNode::create_node(const std::string& typeinfo)
	{
		// parse type info
		auto type = magic_enum::enum_cast<ConflictNode::inner_type>(typeinfo);

		if (!type.has_value())
		{
			throw node_exception("CreateNode: Bad inner type!");
		}

		ConflictNode node(type.value());

		switch (type.value())
		{
		case Tempest::ConflictNode::inner_type::Start:
			node.set_name("Start");
			node.add_output(pin_type::Flow, "");
			break;
		case Tempest::ConflictNode::inner_type::Win:
			node.set_name("Win");
			node.add_input(pin_type::Flow, "");
			break;
		case Tempest::ConflictNode::inner_type::Lose:
			node.set_name("Lose");
			node.add_input(pin_type::Flow, "");
			break;
		default:
			break;
		}

		return make_sptr<ConflictNode>(std::move(node));
	}

	script* ConflictNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]] )
	{
		switch (get_type())
		{
		case Tempest::ConflictNode::inner_type::Start:
		{
			auto script = instance.srm.add_script(
				CreateEventScript([&instance, entity](const Event& e) {
					auto a = event_cast<Simulate>(e);

					if (auto var = instance.srm.get_variable_to_id(entity, "Attacker"))
					{
						var->get<int64_t>() = static_cast<int64_t>(a.attacker);
					}

					if (auto var = instance.srm.get_variable_to_id(entity, "Defender"))
					{
						var->get<int64_t>() = static_cast<int64_t>(a.defender);
					}

					}));

			instance.srm.register_listener<Simulate>(entity, script);

			return script;
		}
		break;
		case Tempest::ConflictNode::inner_type::Win:
		{
			return instance.srm.add_script(
				CreateRuntimeScript<void()>([&instance, entity]() {

					// variable name is "Win"
					if (auto var = instance.srm.get_variable_to_id(entity, "Win"))
					{
						var->get<int>() = 1;
					}

				}));
		}
		case Tempest::ConflictNode::inner_type::Lose:
		{
			return instance.srm.add_script(
				CreateRuntimeScript<void()>([&instance, entity]() {

					// variable name is "Win"
					if (auto var = instance.srm.get_variable_to_id(entity, "Win"))
					{
						var->get<int>() = 0;
					}

				}));
		}
		break;
		default:
			break;
		}

		// default throw
		return nullptr;
	}

}
