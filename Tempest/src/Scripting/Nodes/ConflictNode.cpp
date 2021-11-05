/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


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
		case Tempest::ConflictNode::inner_type::AttackRoll:
			node.set_name("Attack Roll");
			node.add_input(pin_type::Flow, "");
			node.add_input(pin_type::Int, "");
			node.add_output(pin_type::Flow, "");
			node.add_output(pin_type::Int, "");
			break;
		case Tempest::ConflictNode::inner_type::DefendRoll:
			node.set_name("Defend Roll");
			node.add_input(pin_type::Flow, "");
			node.add_input(pin_type::Int, "");
			node.add_output(pin_type::Flow, "");
			node.add_output(pin_type::Int, "");
			break;
		case Tempest::ConflictNode::inner_type::AttackResolve:
			node.set_name("Resolve Attack");
			node.add_input(pin_type::Flow, "");
			node.add_input(pin_type::Int, "");
			node.add_output(pin_type::Flow, "");
			node.add_output(pin_type::Int, "");
			break;
		case Tempest::ConflictNode::inner_type::DefendResolve:
			node.set_name("Resolve Defend");
			node.add_input(pin_type::Flow, "");
			node.add_input(pin_type::Int, "");
			node.add_output(pin_type::Flow, "");
			node.add_output(pin_type::Int, "");
			break;
		case Tempest::ConflictNode::inner_type::END:
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

					if (auto var = instance.srm.get_variable_to_id(entity, "AttackAction"))
					{
						var->get<int64_t>() = static_cast<int64_t>(a.attack_action);
					}

					if (auto var = instance.srm.get_variable_to_id(entity, "DefendAction"))
					{
						var->get<int64_t>() = static_cast<int64_t>(a.defend_action);
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
		case Tempest::ConflictNode::inner_type::AttackRoll:
		{
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<int>(int)>([&instance, entity](int x) {

					auto atker = instance.srm.get_variable_to_id(entity, "Attacker"); // attacking entity
					auto defer = instance.srm.get_variable_to_id(entity, "Defender"); // defending entity
					auto atk_act = instance.srm.get_variable_to_id(entity, "AttackAction"); // attack action
					auto def_act = instance.srm.get_variable_to_id(entity, "DefendAction"); // attack action

					LOG_ASSERT(atker);
					LOG_ASSERT(defer);
					LOG_ASSERT(atk_act);
					LOG_ASSERT(def_act);

					Entity atk_e = (Entity)atker->get<int64_t>();
					Entity def_e = (Entity)defer->get<int64_t>();
					Entity atk_act_e = (Entity)atk_act->get<int64_t>();
					Entity def_act_e = (Entity)def_act->get<int64_t>();

					// dispatch to action graph
					instance.srm.instant_dispatch_to_id<Roll>(atk_act_e, atk_e, def_e, x);
					if (auto var = instance.srm.get_variable_to_id(atk_act_e, "Output"))
					{
						LOG_ASSERT(var->get_type() == pin_type::Int);
						// return the output of the graph
						return std::make_tuple(var->get<int>());
					}
					// if no output, just return x
					return std::make_tuple(x);

					}, std::placeholders::_1));
		}
		break;
		case Tempest::ConflictNode::inner_type::DefendRoll:
		{
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<int>(int)>([&instance, entity](int x) {

					auto atker = instance.srm.get_variable_to_id(entity, "Attacker"); // attacking entity
					auto defer = instance.srm.get_variable_to_id(entity, "Defender"); // defending entity
					auto atk_act = instance.srm.get_variable_to_id(entity, "AttackAction"); // attack action
					auto def_act = instance.srm.get_variable_to_id(entity, "DefendAction"); // attack action

					LOG_ASSERT(atker);
					LOG_ASSERT(defer);
					LOG_ASSERT(atk_act);
					LOG_ASSERT(def_act);

					Entity atk_e = (Entity)atker->get<int64_t>();
					Entity def_e = (Entity)defer->get<int64_t>();
					Entity atk_act_e = (Entity)atk_act->get<int64_t>();
					Entity def_act_e = (Entity)def_act->get<int64_t>();

					// dispatch to action graph
					instance.srm.instant_dispatch_to_id<Roll>(def_act_e, def_e, atk_e, x);
					if (auto var = instance.srm.get_variable_to_id(def_act_e, "Output"))
					{
						LOG_ASSERT(var->get_type() == pin_type::Int);
						// return the output of the graph
						return std::make_tuple(var->get<int>());
					}
					// if no output, just return x
					return std::make_tuple(x);

					}, std::placeholders::_1));
		}
		break;
		case Tempest::ConflictNode::inner_type::AttackResolve:
		{
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<int>(int)>([&instance, entity](int x) {

					auto atker = instance.srm.get_variable_to_id(entity, "Attacker"); // attacking entity
					auto defer = instance.srm.get_variable_to_id(entity, "Defender"); // defending entity
					auto atk_act = instance.srm.get_variable_to_id(entity, "AttackAction"); // attack action
					auto def_act = instance.srm.get_variable_to_id(entity, "DefendAction"); // attack action

					LOG_ASSERT(atker);
					LOG_ASSERT(defer);
					LOG_ASSERT(atk_act);
					LOG_ASSERT(def_act);

					Entity atk_e = (Entity)atker->get<int64_t>();
					Entity def_e = (Entity)defer->get<int64_t>();
					Entity atk_act_e = (Entity)atk_act->get<int64_t>();
					Entity def_act_e = (Entity)def_act->get<int64_t>();

					// dispatch to action graph
					instance.srm.instant_dispatch_to_id<Resolve>(atk_act_e, atk_e, def_e, x);
					if (auto var = instance.srm.get_variable_to_id(atk_act_e, "Output"))
					{
						LOG_ASSERT(var->get_type() == pin_type::Int);
						// return the output of the graph
						return std::make_tuple(var->get<int>());
					}
					// if no output, just return x
					return std::make_tuple(x);

					}, std::placeholders::_1));
		}
		break;
		case Tempest::ConflictNode::inner_type::DefendResolve:
		{
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<int>(int)>([&instance, entity](int x) {

					auto atker = instance.srm.get_variable_to_id(entity, "Attacker"); // attacking entity
					auto defer = instance.srm.get_variable_to_id(entity, "Defender"); // defending entity
					auto atk_act = instance.srm.get_variable_to_id(entity, "AttackAction"); // attack action
					auto def_act = instance.srm.get_variable_to_id(entity, "DefendAction"); // attack action

					LOG_ASSERT(atker);
					LOG_ASSERT(defer);
					LOG_ASSERT(atk_act);
					LOG_ASSERT(def_act);

					Entity atk_e = (Entity)atker->get<int64_t>();
					Entity def_e = (Entity)defer->get<int64_t>();
					Entity atk_act_e = (Entity)atk_act->get<int64_t>();
					Entity def_act_e = (Entity)def_act->get<int64_t>();

					// dispatch to action graph
					instance.srm.instant_dispatch_to_id<Resolve>(def_act_e, def_e, atk_e, x);
					if (auto var = instance.srm.get_variable_to_id(def_act_e, "Output"))
					{
						LOG_ASSERT(var->get_type() == pin_type::Int);
						// return the output of the graph
						return std::make_tuple(var->get<int>());
					}
					// if no output, just return x
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
