
#include "../Util/node.h"
#include "Instance/Instance.h"
#include "Events/Events/ScriptEvents.h"

namespace Tempest
{
    std::string SwitchNode::get_type_string()
    {
        // assuming no problem
        std::string toReturn = magic_enum::enum_name(type).data();

        return std::move(toReturn);
    }

    node_ptr SwitchNode::create_node(const std::string& typeinfo)
    {
        // parse type info
        auto type = magic_enum::enum_cast<SwitchNode::inner_type>(typeinfo);

        if (!type.has_value())
        {
            throw node_exception("CreateNode: Bad inner type!");
        }

        SwitchNode node(type.value());

        switch (type.value())
        {
        case Tempest::SwitchNode::inner_type::TwoSwitch:
            node.set_name("Two-Switch");
            node.add_input(pin_type::Flow, "");
            node.add_input(pin_type::Int, "choice");

            node.add_output(pin_type::Flow, "case 0");
            node.add_output(pin_type::Flow, "case 1");
            node.add_output(pin_type::Flow, "case 2");
            node.add_output(pin_type::Flow, "default");
            node.add_output(pin_type::Int, "out");

            break;
        case Tempest::SwitchNode::inner_type::ThreeSwitch:
            node.set_name("Three-Switch");
            node.add_input(pin_type::Flow, "");
            node.add_input(pin_type::Int, "choice");

            node.add_output(pin_type::Flow, "case 0");
            node.add_output(pin_type::Flow, "case 1");
            node.add_output(pin_type::Flow, "case 2");
            node.add_output(pin_type::Flow, "case 3");
            node.add_output(pin_type::Flow, "default");
            node.add_output(pin_type::Int, "out");
            break;
        case Tempest::SwitchNode::inner_type::FiveSwitch:
            node.set_name("Five-Switch");
            node.add_input(pin_type::Flow, "");
            node.add_input(pin_type::Int, "choice");

            node.add_output(pin_type::Flow, "case 0");
            node.add_output(pin_type::Flow, "case 1");
            node.add_output(pin_type::Flow, "case 2");
            node.add_output(pin_type::Flow, "case 3");
            node.add_output(pin_type::Flow, "case 4");
            node.add_output(pin_type::Flow, "case 5");
            node.add_output(pin_type::Flow, "default");
            node.add_output(pin_type::Int, "out");
            break;
        case Tempest::SwitchNode::inner_type::TenSwitch:
            node.set_name("Ten-Switch");
            node.add_input(pin_type::Flow, "");
            node.add_input(pin_type::Int, "choice");

            node.add_output(pin_type::Flow, "case 0");
            node.add_output(pin_type::Flow, "case 1");
            node.add_output(pin_type::Flow, "case 2");
            node.add_output(pin_type::Flow, "case 3");
            node.add_output(pin_type::Flow, "case 4");
            node.add_output(pin_type::Flow, "case 5");
            node.add_output(pin_type::Flow, "case 6");
            node.add_output(pin_type::Flow, "case 7");
            node.add_output(pin_type::Flow, "case 8");
            node.add_output(pin_type::Flow, "case 9");
            node.add_output(pin_type::Flow, "case 10");
            node.add_output(pin_type::Flow, "default");
            node.add_output(pin_type::Int, "out");
            break;

        case Tempest::SwitchNode::inner_type::TwentySwitch:
            node.set_name("Ten-Switch");
            node.add_input(pin_type::Flow, "");
            node.add_input(pin_type::Int, "choice");

            node.add_output(pin_type::Flow, "case 0");
            node.add_output(pin_type::Flow, "case 1");
            node.add_output(pin_type::Flow, "case 2");
            node.add_output(pin_type::Flow, "case 3");
            node.add_output(pin_type::Flow, "case 4");
            node.add_output(pin_type::Flow, "case 5");
            node.add_output(pin_type::Flow, "case 6");
            node.add_output(pin_type::Flow, "case 7");
            node.add_output(pin_type::Flow, "case 8");
            node.add_output(pin_type::Flow, "case 9");
            node.add_output(pin_type::Flow, "case 10");
            node.add_output(pin_type::Flow, "case 11");
            node.add_output(pin_type::Flow, "case 12");
            node.add_output(pin_type::Flow, "case 13");
            node.add_output(pin_type::Flow, "case 14");
            node.add_output(pin_type::Flow, "case 15");
            node.add_output(pin_type::Flow, "case 16");
            node.add_output(pin_type::Flow, "case 17");
            node.add_output(pin_type::Flow, "case 18");
            node.add_output(pin_type::Flow, "case 19");
            node.add_output(pin_type::Flow, "case 20");
            node.add_output(pin_type::Flow, "default");
            node.add_output(pin_type::Int, "out");
            break;
        case Tempest::SwitchNode::inner_type::ThirtySwitch:
            node.set_name("Ten-Switch");
            node.add_input(pin_type::Flow, "");
            node.add_input(pin_type::Int, "choice");

            node.add_output(pin_type::Flow, "case 0");
            node.add_output(pin_type::Flow, "case 1");
            node.add_output(pin_type::Flow, "case 2");
            node.add_output(pin_type::Flow, "case 3");
            node.add_output(pin_type::Flow, "case 4");
            node.add_output(pin_type::Flow, "case 5");
            node.add_output(pin_type::Flow, "case 6");
            node.add_output(pin_type::Flow, "case 7");
            node.add_output(pin_type::Flow, "case 8");
            node.add_output(pin_type::Flow, "case 9");
            node.add_output(pin_type::Flow, "case 10");
            node.add_output(pin_type::Flow, "case 11");
            node.add_output(pin_type::Flow, "case 12");
            node.add_output(pin_type::Flow, "case 13");
            node.add_output(pin_type::Flow, "case 14");
            node.add_output(pin_type::Flow, "case 15");
            node.add_output(pin_type::Flow, "case 16");
            node.add_output(pin_type::Flow, "case 17");
            node.add_output(pin_type::Flow, "case 18");
            node.add_output(pin_type::Flow, "case 19");
            node.add_output(pin_type::Flow, "case 20");
            node.add_output(pin_type::Flow, "case 21");
            node.add_output(pin_type::Flow, "case 22");
            node.add_output(pin_type::Flow, "case 23");
            node.add_output(pin_type::Flow, "case 24");
            node.add_output(pin_type::Flow, "case 25");
            node.add_output(pin_type::Flow, "case 26");
            node.add_output(pin_type::Flow, "case 27");
            node.add_output(pin_type::Flow, "case 28");
            node.add_output(pin_type::Flow, "case 29");
            node.add_output(pin_type::Flow, "case 30");
            node.add_output(pin_type::Flow, "default");
            node.add_output(pin_type::Int, "out");
            break;
        default:
            break;
        }

        return make_sptr<SwitchNode>(std::move(node));
    }

    script* SwitchNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]] )
    {
        switch (get_type())
        {
        case Tempest::SwitchNode::inner_type::TwoSwitch:
            return instance.srm.add_script(
                CreateBranchScript<int(int), 2 + 2>(
                    [](const auto& x) { return std::make_tuple(x); },
                    std::placeholders::_1));
        case Tempest::SwitchNode::inner_type::ThreeSwitch:
            return instance.srm.add_script(
                CreateBranchScript<int(int), 3 + 2>(
                    [](const auto& x) { return std::make_tuple(x); },
                    std::placeholders::_1));
        case Tempest::SwitchNode::inner_type::FiveSwitch:
            return instance.srm.add_script(
                CreateBranchScript<int(int), 5 + 2>(
                    [](const auto& x) { return std::make_tuple(x); },
                    std::placeholders::_1));
        case Tempest::SwitchNode::inner_type::TenSwitch:
            return instance.srm.add_script(
                CreateBranchScript<int(int), 10 + 2>(
                    [](const auto& x) { return std::make_tuple(x); },
                    std::placeholders::_1));
        case Tempest::SwitchNode::inner_type::TwentySwitch:
            return instance.srm.add_script(
                CreateBranchScript<int(int), 20 + 2>(
                    [](const auto& x) { return std::make_tuple(x); },
                    std::placeholders::_1));
        case Tempest::SwitchNode::inner_type::ThirtySwitch:
            return instance.srm.add_script(
                CreateBranchScript<int(int), 30 + 2>(
                    [](const auto& x) { return std::make_tuple(x); },
                    std::placeholders::_1));
        default:
            break;
        }

        // default throw
        return nullptr;
    }

}
