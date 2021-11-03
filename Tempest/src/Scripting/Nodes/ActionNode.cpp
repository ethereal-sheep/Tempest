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
    std::string ActionNode::get_type_string()
    {
        // assuming no problem
        std::string toReturn = magic_enum::enum_name(type).data();

        return std::move(toReturn);
    }

    node_ptr ActionNode::create_node(const std::string& typeinfo)
    {
        // parse type info
        auto type = magic_enum::enum_cast<ActionNode::inner_type>(typeinfo);

        if (!type.has_value())
        {
            throw node_exception("CreateNode: Bad inner type!");
        }

        ActionNode node(type.value());

        switch (type.value())
        {
        case Tempest::ActionNode::inner_type::Input:
            node.set_name("Input");
            node.add_output(pin_type::Flow, "");
            node.add_output(pin_type::Int, "");
            break;
        case Tempest::ActionNode::inner_type::Output:
            node.set_name("Output");
            node.add_input(pin_type::Flow, "");
            node.add_input(pin_type::Int, "");
            break;
        case Tempest::ActionNode::inner_type::Roll:
            node.set_name("Roll");
            node.add_output(pin_type::Flow, "");
            node.add_output(pin_type::Int, "");
            break;
        case Tempest::ActionNode::inner_type::Resolve:
            node.set_name("Resolve");
            node.add_output(pin_type::Flow, "");
            node.add_output(pin_type::Int, "");
            break;
        default:
            return nullptr;
            break;
        }

        return make_sptr<ActionNode>(std::move(node));
    }

    script* ActionNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]] )
    {

        switch (get_type())
        {
        case Tempest::ActionNode::inner_type::Input:
        {
            auto script = instance.srm.add_script(
                CreateEventScript([&instance, entity](const Event& e) {
                    auto a = event_cast<Input>(e);

                    if (auto var = instance.srm.get_variable_to_id(entity, "Attacker"))
                    {
                        var->get<int64_t>() = static_cast<int64_t>(a.attacker);
                    }

                    if (auto var = instance.srm.get_variable_to_id(entity, "Defender"))
                    {
                        var->get<int64_t>() = static_cast<int64_t>(a.defender);
                    }

                    if (auto var = instance.srm.get_variable_to_id(entity, "Output"))
                    {
                        var->get<int>() = static_cast<int64_t>(a.input);
                    }

                    return std::make_tuple(a.input);
                }
            ));

            instance.srm.register_listener<Input>(entity, script);

            return script;
        }
        break;
        case Tempest::ActionNode::inner_type::Output:
        {
            return instance.srm.add_script(
                CreateRuntimeScript<void(int)>([&instance, entity](int x) {
                    
                    if (auto var = instance.srm.get_variable_to_id(entity, "Output"))
                    {
                        var->get<int>() = x;
                    }

                }, std::placeholders::_1));
        }
        break;
        case Tempest::ActionNode::inner_type::Roll:
        {
            auto script = instance.srm.add_script(
                CreateEventScript([&instance, entity](const Event& e) {
                    auto a = event_cast<Roll>(e);

                    // must have owner, enemy, and output 
                    LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Owner"));
                    LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Enemy"));
                    LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Output"));

                    if (auto var = instance.srm.get_variable_to_id(entity, "Owner"))
                    {
                        var->get<int64_t>() = static_cast<int64_t>(a.owner);
                    }

                    if (auto var = instance.srm.get_variable_to_id(entity, "Enemy"))
                    {
                        var->get<int64_t>() = static_cast<int64_t>(a.enemy);
                    }

                    if (auto var = instance.srm.get_variable_to_id(entity, "Output"))
                    {
                        var->get<int>() = static_cast<int64_t>(a.input);
                    }

                    return std::make_tuple(a.input);
                }
            ));

            instance.srm.register_listener<Roll>(entity, script);

            return script;
        }
        break;
        case Tempest::ActionNode::inner_type::Resolve:
        {
            auto script = instance.srm.add_script(
                CreateEventScript([&instance, entity](const Event& e) {
                    auto a = event_cast<Resolve>(e);

                    // must have owner, enemy, and output 
                    LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Owner"));
                    LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Enemy"));
                    LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Output"));

                    if (auto var = instance.srm.get_variable_to_id(entity, "Owner"))
                    {
                        var->get<int64_t>() = static_cast<int64_t>(a.owner);
                    }

                    if (auto var = instance.srm.get_variable_to_id(entity, "Enemy"))
                    {
                        var->get<int64_t>() = static_cast<int64_t>(a.enemy);
                    }

                    if (auto var = instance.srm.get_variable_to_id(entity, "Output"))
                    {
                        var->get<int>() = static_cast<int64_t>(a.input);
                    }

                    return std::make_tuple(a.input);
                }
            ));

            instance.srm.register_listener<Resolve>(entity, script);

            return script;
        }
        break;
        default:
            break;
        }

        // default throw
        return nullptr;
    }

}