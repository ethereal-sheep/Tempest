/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
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
    std::string CompareNode::get_type_string()
    {
        // assuming no problem
        std::string toReturn = magic_enum::enum_name(type).data();

        return std::move(toReturn);
    }

    node_ptr CompareNode::create_node(const std::string& typeinfo)
    {
        // parse type info
        auto type = magic_enum::enum_cast<CompareNode::inner_type>(typeinfo);

        if (!type.has_value())
        {
            throw node_exception("CreateNode: Bad inner type!");
        }

        CompareNode node(type.value());

        switch (type.value())
        {
        case Tempest::CompareNode::inner_type::CompareFlow:
            node.set_name("Compare Flow");
            
            node.add_input(pin_type::Flow, "");
            node.add_input(pin_type::Int, "lhs ");
            node.add_input(pin_type::Int, "rhs ");
            
            node.add_output(pin_type::Flow, (const char*)u8"\uf52c");
            node.add_output(pin_type::Flow, (const char*)u8"\uf531");
            node.add_output(pin_type::Flow, (const char*)u8"\uf536");

            node.add_output(pin_type::Int, "result ");
            node.add_output(pin_type::Int, "lhs_out ");
            node.add_output(pin_type::Int, "rhs_out ");

            break;
        default:
            break;
        }

        return make_sptr<CompareNode>(std::move(node));
    }

    script* CompareNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]] )
    {
        switch (get_type())
        {
        case Tempest::CompareNode::inner_type::CompareFlow:
            return instance.srm.add_script(
                CreateBranchScript<std::tuple<int, int, int>(int, int), 3>(
                    [&instance, entity](int lhs, int rhs) {

                        if (auto var = instance.srm.get_variable_to_id(entity, "AttackRollOutput"))
                        {
                            LOG_ASSERT(var->get_type() == pin_type::Int);
                            var->get<int>() = lhs;
                        } // attack output
                        if (auto var = instance.srm.get_variable_to_id(entity, "DefendRollOutput"))
                        {
                            LOG_ASSERT(var->get_type() == pin_type::Int);
                            var->get<int>() = rhs;
                        } // defend output


                        if (lhs > rhs)
                            return std::make_tuple(0, lhs, rhs);
                        if (lhs == rhs)
                            return std::make_tuple(1, lhs, rhs);
                        return std::make_tuple(2, lhs, rhs);
                    },
                    std::placeholders::_1, std::placeholders::_2));
        default:
            break;
        }

        // default throw
        return nullptr;
    }

}
