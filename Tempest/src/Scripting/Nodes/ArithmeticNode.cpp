
#include "../Util/node.h"
#include "Instance/Instance.h"

namespace Tempest
{
std::string ArithmeticNode::get_type_string()
    {
        // assuming no problem
        std::string toReturn = magic_enum::enum_name(type).data();

        return std::move(toReturn);
    }

    node_ptr ArithmeticNode::create_node(const std::string& typeinfo)
    {
        // parse type info
        auto type = magic_enum::enum_cast<ArithmeticNode::inner_type>(typeinfo);

        if (!type.has_value())
        {
            throw node_exception("CreateNode: Bad inner type!");
        }

        ArithmeticNode node(type.value());

        switch (type.value())
        {
        case Tempest::ArithmeticNode::inner_type::Plus:
            node.set_name("Plus");
            node.add_input(pin_type::Int, "");
            node.add_input(pin_type::Int, "");
            node.add_output(pin_type::Int, "");
        break;
        case Tempest::ArithmeticNode::inner_type::Minus:
            node.set_name("Minus");
            node.add_input(pin_type::Int, "");
            node.add_input(pin_type::Int, "");
            node.add_output(pin_type::Int, "");
        break;
        default:
            break;
        }

        return make_uptr<ArithmeticNode>(std::move(node));
    }

    script* ArithmeticNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]] )
    {
        
        switch (type)
        {
        case Tempest::ArithmeticNode::inner_type::Plus:
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(int, int)>(
                    [](int x, int y) {
                        return x + y;
                    }, std::placeholders::_1, std::placeholders::_2));
            break;
        case Tempest::ArithmeticNode::inner_type::Minus:
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(int, int)>(
                    [](int x, int y) {
                        return x - y;
                    }, std::placeholders::_1, std::placeholders::_2));
            break;
        default:
            break;
        }


        // default throw
        return nullptr;
    }

}
