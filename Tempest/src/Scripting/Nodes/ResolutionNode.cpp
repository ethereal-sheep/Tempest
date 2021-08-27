
#include "../Util/node.h"
#include "Instance/Instance.h"
#include "Events/Events/ScriptEvents.h"

namespace Tempest
{
    std::string ResolutionNode::get_type_string()
    {
        // assuming no problem
        std::string toReturn = magic_enum::enum_name(type).data();

        return std::move(toReturn);
    }

    node_ptr ResolutionNode::create_node(const std::string& typeinfo)
    {
        // parse type info
        auto type = magic_enum::enum_cast<ResolutionNode::inner_type>(typeinfo);

        if (!type.has_value())
        {
            throw node_exception("CreateNode: Bad inner type!");
        }

        ResolutionNode node(type.value());

        switch (type.value())
        {
        case Tempest::ResolutionNode::inner_type::Start:
            node.set_name("Start");
            node.add_output(pin_type::Flow, "");
            break;
        case Tempest::ResolutionNode::inner_type::Resolve:
            node.set_name("Resolve");
            node.add_input(pin_type::Flow, "");
            node.add_input(pin_type::Int, "Attacking");
            node.add_input(pin_type::Int, "Defending");
            break;
        case Tempest::ResolutionNode::inner_type::Attacking:
            node.set_name("Attacking");
            node.add_input(pin_type::Flow, "");
            node.add_input(pin_type::Int, "");
            node.add_output(pin_type::Flow, "");
            node.add_output(pin_type::Int, "");
            break;
        case Tempest::ResolutionNode::inner_type::Defending:
            node.set_name("Defending");
            node.add_input(pin_type::Flow, "");
            node.add_input(pin_type::Int, "");
            node.add_output(pin_type::Flow, "");
            node.add_output(pin_type::Int, "");
            break;
        default:
            break;
        }

        return make_uptr<ResolutionNode>(std::move(node));
    }

    script* ResolutionNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]] )
    {
        switch (get_type())
        {
        case Tempest::ResolutionNode::inner_type::Start:
        {
            auto script = instance.srm.add_script(
                CreateEventScript([entity](const Event& e) {
                    auto a = event_cast<Simulate>(e);
                    LOG("Event Called: {0}", entity);
                    }));

            instance.srm.register_listener<Simulate>(entity, script);

            return script;
        }
            break;
        case Tempest::ResolutionNode::inner_type::Resolve:
        {
            auto script = instance.srm.add_script(
                CreateEventScript([entity](const Event& e) {
                    auto a = event_cast<test_event_with_member>(e);
                    LOG("Event Called: {0} called with {1}", entity, a.i);
                    }));

            instance.srm.register_listener<test_event_with_member>(entity, script);

            return script;
        }
            break;
        case Tempest::ResolutionNode::inner_type::Attacking:
        {
            auto script = instance.srm.add_script(
                CreateEventScript([entity](const Event& e) {
                    auto a = event_cast<test_event_with_member>(e);
                    LOG("Event Called: {0} called with {1}", entity, a.i);
                    }));

            instance.srm.register_listener<test_event_with_member>(entity, script);

            return script;
        }
            break;
        case Tempest::ResolutionNode::inner_type::Defending:
        {
            auto script = instance.srm.add_script(
                CreateEventScript([entity](const Event& e) {
                    auto a = event_cast<test_event_with_member>(e);
                    LOG("Event Called: {0} called with {1}", entity, a.i);
                    }));

            instance.srm.register_listener<test_event_with_member>(entity, script);

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
