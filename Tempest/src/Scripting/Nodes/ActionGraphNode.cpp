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
    std::string ActionGraphNode::get_type_string()
    {
        // assuming no problem
        std::string toReturn = std::to_string(graph_entity);

        return std::move(toReturn);
    }

    node_ptr ActionGraphNode::create_node(const std::string& typeinfo)
    {
        ActionGraphNode node(ActionGraphNode::inner_type::_cannot_be_empty);
        node.graph_entity = std::stoi(typeinfo);

		node.set_name("If you see this, drawing of action graph nodes is wrong");
		node.add_input(pin_type::Flow, "");
		node.add_input(pin_type::Int, "");
		node.add_output(pin_type::Flow, "");
		node.add_output(pin_type::Int, "");

        return make_sptr<ActionGraphNode>(std::move(node));
    }

    script* ActionGraphNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]] )
    {
		// check if graph exists first

		if (!instance.ecs.has<tc::ActionGraph>(graph_entity))
			throw script_exception("Entity has no action graph!");

		Entity g_e = graph_entity;

		return instance.srm.add_script(
			CreateRuntimeScript<std::tuple<int>(int)>([&instance, entity, g_e](int x) {

				auto atker = instance.srm.get_variable_to_id(entity, "Attacker"); // attacking entity
				auto defer = instance.srm.get_variable_to_id(entity, "Defender"); // defending entity

				Entity atk_e = (Entity)atker->get<int64_t>();
				Entity def_e = (Entity)defer->get<int64_t>();

				instance.srm.instant_dispatch_to_id<Input>(g_e, atk_e, def_e, x);
				if (auto var = instance.srm.get_variable_to_id(g_e, "Output"))
				{
					LOG_ASSERT(var->get_type() == pin_type::Int);
					return std::make_tuple(var->get<int>());
				}

				return std::make_tuple(x);

		}, std::placeholders::_1));
    }

}