
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
		case Tempest::ConflictNode::inner_type::Resolve:
			node.set_name("Resolve");
			node.add_input(pin_type::Flow, "");
			node.add_input(pin_type::Int, "Attacking");
			node.add_input(pin_type::Int, "Defending");
			break;
		case Tempest::ConflictNode::inner_type::Attacking:
			node.set_name("Attacking");
			node.add_input(pin_type::Flow, "");
			node.add_input(pin_type::Int, "");
			node.add_output(pin_type::Flow, "");
			node.add_output(pin_type::Int, "");
			break;
		case Tempest::ConflictNode::inner_type::Defending:
			node.set_name("Defending");
			node.add_input(pin_type::Flow, "");
			node.add_input(pin_type::Int, "");
			node.add_output(pin_type::Flow, "");
			node.add_output(pin_type::Int, "");
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

					if (auto var = instance.srm.get_variable_to_id(entity, "Attacking"))
					{
						var->get<int64_t>() = static_cast<int64_t>(a.attacking);
					}

					if (auto var = instance.srm.get_variable_to_id(entity, "Defending"))
					{
						var->get<int64_t>() = static_cast<int64_t>(a.defending);
					}
					if (auto var = instance.srm.get_variable_to_id(entity, "Attacking_System"))
					{
						var->get<int64_t>() = static_cast<int64_t>(a.atk_sys);
					}
					if (auto var = instance.srm.get_variable_to_id(entity, "Defending_System"))
					{
						var->get<int64_t>() = static_cast<int64_t>(a.def_sys);
					}
					if (auto var = instance.srm.get_variable_to_id(entity, "Resolving_System"))
					{
						var->get<int64_t>() = static_cast<int64_t>(a.res_sys);
					}

					}));

			instance.srm.register_listener<Simulate>(entity, script);

			return script;
		}
		break;
		case Tempest::ConflictNode::inner_type::Resolve:
		{
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<int>(int, int)>([&instance, entity](int x, int y) {

					return std::make_tuple(x + y);

					}, std::placeholders::_1, std::placeholders::_2));
		}
		break;
		case Tempest::ConflictNode::inner_type::Attacking:
		{
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<int>(int)>([&instance, entity](int x) {

					// attacking entity

					auto atk = instance.srm.get_variable_to_id(entity, "Attacking");
					auto atk_sys = instance.srm.get_variable_to_id(entity, "Attacking_System");

					if (atk && atk_sys)
					{
						Entity atk_e = (Entity)atk->get<int64_t>();
						Entity atk_sys_e = (Entity)atk_sys->get<int64_t>();

						instance.srm.instant_dispatch_to_id<Input>(atk_sys_e, atk_e, x);
						if (auto var = instance.srm.get_variable_to_id(atk_sys_e, "Output"))
						{
							LOG_ASSERT(var->get_type() == pin_type::Int);
							return std::make_tuple(var->get<int>());
						}
					}

					return std::make_tuple(x);

					}, std::placeholders::_1));
		}
		break;
		case Tempest::ConflictNode::inner_type::Defending:
		{
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<int>(int)>([&instance, entity](int x) {

					// defending entity
					auto def = instance.srm.get_variable_to_id(entity, "Defending");
					auto def_sys = instance.srm.get_variable_to_id(entity, "Defending_System");

					if (def && def_sys)
					{
						Entity def_e = (Entity)def->get<int64_t>();
						Entity def_sys_e = (Entity)def_sys->get<int64_t>();

						instance.srm.instant_dispatch_to_id<Input>(def_sys_e, def_e, x);
						if (auto var = instance.srm.get_variable_to_id(def_sys_e, "Output"))
						{
							LOG_ASSERT(var->get_type() == pin_type::Int);
							return std::make_tuple(var->get<int>());
						}
					}

					return std::make_tuple(x);

					}, std::placeholders::_1));
		}
		break;
		default:
			break;
		}

		// default throw
		return nullptr;
	}

}
