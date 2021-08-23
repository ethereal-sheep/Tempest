
#include "../Util/node.h"
#include "Instance/Instance.h"

namespace Tempest
{
    std::string UtilNode::get_type_string()
    {
        // assuming no problem
        pin_type Startpin_type = node::get_inputs()[0].get_type();
        pin_type Endpin_type = node::get_outputs()[0].get_type();

        std::string toReturn = magic_enum::enum_name(Startpin_type).data();
        toReturn += ":";
        toReturn += magic_enum::enum_name(Endpin_type).data();

        return std::move(toReturn);
    }

    node_ptr UtilNode::create_node(const std::string& typeinfo)
    {
        // parse type info
        auto type = magic_enum::enum_cast<UtilNode::inner_type>(typeinfo);

        if (!type.has_value())
        {
            throw node_exception("CreateNode: Bad inner type!");
        }

        UtilNode node(type.value());
        switch (type.value())
        {
        case Tempest::UtilNode::inner_type::Print:
            node.set_name("Print");
            node.add_input(pin_type::Flow, "");
            node.add_input(pin_type::Float, "");
            node.add_output(pin_type::Flow, "");
            break;
        case Tempest::UtilNode::inner_type::In:
            node.set_name("In");
            node.add_output(pin_type::Flow, "");
            node.add_output(pin_type::Float, "In");
            break;
        default:
            return nullptr;
            break;
        }

        return make_uptr<UtilNode>(std::move(node));
    }

    script* UtilNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]] )
    {
       


        // default throw
        return nullptr;
    }

}