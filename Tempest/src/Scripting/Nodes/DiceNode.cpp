
#include "../Util/node.h"
#include "Instance/Instance.h"

namespace Tempest
{
    std::string DiceNode::get_type_string()
    {
        // assuming no problem
        std::string toReturn = magic_enum::enum_name(type).data();

        return std::move(toReturn);
    }

    node_ptr DiceNode::create_node(const std::string& typeinfo)
    {
        // parse type info
        auto type = magic_enum::enum_cast<DiceNode::inner_type>(typeinfo);

        if (!type.has_value())
        {
            throw node_exception("CreateNode: Bad inner type!");
        }

        DiceNode node(type.value());

        switch (type.value())
        {
        case Tempest::DiceNode::inner_type::D4:
            node.set_name("D4");
            node.add_input(pin_type::Int, "");
            node.add_output(pin_type::Int, "");
            break;
        case Tempest::DiceNode::inner_type::D6:
            node.set_name("D6");
            node.add_input(pin_type::Int, "");
            node.add_output(pin_type::Int, "");
            break;
        case Tempest::DiceNode::inner_type::D8:
            node.set_name("D8");
            node.add_input(pin_type::Int, "");
            node.add_output(pin_type::Int, "");
            break;
        case Tempest::DiceNode::inner_type::D10:
            node.set_name("D10");
            node.add_input(pin_type::Int, "");
            node.add_output(pin_type::Int, "");
            break;
        case Tempest::DiceNode::inner_type::D12:
            node.set_name("D12");
            node.add_input(pin_type::Int, "");
            node.add_output(pin_type::Int, "");
            break;
        case Tempest::DiceNode::inner_type::D20:
            node.set_name("D20");
            node.add_input(pin_type::Int, "");
            node.add_output(pin_type::Int, "");
            break;
        case Tempest::DiceNode::inner_type::END:
            node.set_name("END");
            node.add_input(pin_type::Int, "");
            node.add_output(pin_type::Int, "");
            break;
        default:
            break;
        }

        return make_uptr<DiceNode>(std::move(node));
    }

    script* DiceNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]] )
    {

        switch (type)
        {
        case Tempest::DiceNode::inner_type::D4:
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(int)>(
                    [](int x) {
                        return x + els::random::uniform_rand(1,4);
                    }, std::placeholders::_1));
            break;
        case Tempest::DiceNode::inner_type::D6:
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(int)>(
                    [](int x) {
                        return x + els::random::uniform_rand(1,6);
                    }, std::placeholders::_1));
            break;
        case Tempest::DiceNode::inner_type::D8:
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(int)>(
                    [](int x) {
                        return x + els::random::uniform_rand(1,8);
                    }, std::placeholders::_1));
            break;
        case Tempest::DiceNode::inner_type::D10:
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(int)>(
                    [](int x) {
                        return x + els::random::uniform_rand(1,10);
                    }, std::placeholders::_1));
            break;
        case Tempest::DiceNode::inner_type::D12:
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(int)>(
                    [](int x) {
                        return x + els::random::uniform_rand(1,12);
                    }, std::placeholders::_1));
            break;
        case Tempest::DiceNode::inner_type::D20:
             return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(int)>(
                    [](int x) {
                        return x + els::random::uniform_rand(1,20);
                    }, std::placeholders::_1));
            break;
        case Tempest::DiceNode::inner_type::END:
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(int)>(
                    [](int x) {
                        return x + els::random::uniform_rand(1,4);
                    }, std::placeholders::_1));
            break;
        default:
            break;
        }


        // default throw
        return nullptr;
    }

}