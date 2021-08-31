
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
            node.add_input(pin_type::Int, "lhs");
            node.add_input(pin_type::Int, "rhs");
            node.add_output(pin_type::Flow, u8"\uf531");
            node.add_output(pin_type::Flow, u8"\uf52c");
            node.add_output(pin_type::Flow, u8"\uf536");
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
                CreateBranchScript<std::tuple<int>(int, int), 3>(
                    [](int lhs, int rhs) { 
                        if (lhs > rhs)
                            return std::make_tuple(0);
                        if (lhs == rhs)
                            return std::make_tuple(1);
                        return std::make_tuple(2);
                    },
                    std::placeholders::_1, std::placeholders::_2));
        default:
            break;
        }

        // default throw
        return nullptr;
    }

}
