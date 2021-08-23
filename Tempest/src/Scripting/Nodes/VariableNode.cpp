
#include "../Util/node.h"
#include "Instance/Instance.h"

namespace Tempest
{
    std::string VariableNode::get_type_string()
    {
        // assuming no problem
        const bool isSetter = (type == VariableNode::inner_type::GlobalSet) 
                     || (type == VariableNode::inner_type::LocalSet);
        const auto index = isSetter ? 1 : 0;

        pin_type end = get_outputs()[index].get_type();
        
        std::string toReturn = magic_enum::enum_name(type).data();
        toReturn += ":";
        toReturn += magic_enum::enum_name(end).data();
        toReturn += ":";

        auto v = algo::delimited_split(get_name(), ":");

        if (v.size() != 2)
        {
            //CDN_CORE_WARN("get_typeInString {0} : name ({1}) corrupted", typeid(VariableNode).name(), Node::GetName());
        }

        toReturn += v[1];

        return std::move(toReturn);
    }

    node_ptr VariableNode::create_node(const std::string& typeinfo)
    {
        // parse type info
        auto v = algo::delimited_split(typeinfo, ":");
        if(v.size() != 3)
        {
            throw node_exception("VariableNode: Bad inner type!");
        }

        auto type = magic_enum::enum_cast<VariableNode::inner_type>(v[0]);
        auto end = magic_enum::enum_cast<pin_type>(v[1]);

        if(!type.has_value() || !end.has_value())
        {
            throw node_exception("VariableNode: Bad inner type!");
        }

        // assume variable exist
        VariableNode node(type.value());
        std::string name;

        switch (type.value())
        {
        case VariableNode::inner_type::LocalGet:
            name = "Local:";
            name += v[2];
            node.set_name(name);
            node.add_output(end.value(), "");
            break;
        case VariableNode::inner_type::GlobalGet:
            name = "Global:";
            name += v[2];
            node.set_name(name);
            node.add_output(end.value(), "");
            break;
        case VariableNode::inner_type::LocalSet:
            name = "Local:";
            name += v[2];
            node.set_name(name);
            node.add_input(pin_type::Flow, "");
            node.add_input(end.value(), "Set");
            node.add_output(pin_type::Flow, "");
            node.add_output(end.value(), "");
            break;
        case VariableNode::inner_type::GlobalSet:
            name = "Global:";
            name += v[2];
            node.set_name(name);
            node.add_input(pin_type::Flow, "");
            node.add_input(end.value(), "Set");
            node.add_output(pin_type::Flow, "");
            node.add_output(end.value(), "");
            break;
        default: return nullptr; break;
        }
        return std::make_unique<VariableNode>(std::move(node));
    }

    script* create_getter(pin_type pin_t, var_data* var, Instance& instance);
    script* create_setter(pin_type pin_t, var_data* var, Instance& instance);
    
    script* VariableNode::create_script(Entity entity, Instance& instance)
    {
        auto v = algo::delimited_split(get_name(), ":");
        if (v.size() != 2)
            throw script_exception("VariableScript: Bad inner type!");

        switch (type)
        {
        case VariableNode::inner_type::LocalGet:
        {
            pin_type pin_t = get_outputs()[0].get_type();
            var_data* var = instance.srm.get_variable_to_id(entity, v[1]);
            return create_getter(pin_t, var, instance);
        }
        case VariableNode::inner_type::GlobalGet:
        {
            pin_type pin_t = get_outputs()[0].get_type();
            var_data* var = instance.srm.get_variable(v[1]);
            return create_getter(pin_t, var, instance);
        }
        case VariableNode::inner_type::LocalSet:
        {
            pin_type pin_t = get_outputs()[1].get_type();
            var_data* var = instance.srm.get_variable_to_id(entity, v[1]);
            return create_setter(pin_t, var, instance);
        }
        case VariableNode::inner_type::GlobalSet:
        {
            pin_type pin_t = get_outputs()[1].get_type();
            var_data* var = instance.srm.get_variable(v[1]);
            return create_setter(pin_t, var, instance);
        }

        }


        return nullptr;
    }

    script* create_getter(pin_type pin_t, var_data* var, Instance& instance)
    {
        if (!var)
            throw script_exception("VariableScript: Cannot find variable!");

        if (var->get_type() != pin_t)
            throw script_exception("VariableScript: Badly formed variable!");

        switch (pin_t)
        {
        case pin_type::Bool:
            return instance.srm.add_script(
                CreateDataScript<bool&>(var->get<bool>()));

        case pin_type::Byte:
            return instance.srm.add_script(
                CreateDataScript<uint8_t&>(var->get<uint8_t>()));

        case pin_type::Int:
            return instance.srm.add_script(
                CreateDataScript<int&>(var->get<int>()));

        case pin_type::Entity:
            return instance.srm.add_script(
                CreateDataScript<uint32_t&>(var->get<uint32_t>()));

        case pin_type::Int64:
            return instance.srm.add_script(
                CreateDataScript<int64_t&>(var->get<int64_t>()));

        case pin_type::Float:
            return instance.srm.add_script(
                CreateDataScript<float&>(var->get<float>()));

        case pin_type::String:
            return instance.srm.add_script(
                CreateDataScript<std::string&>(var->get<string>()));

        case pin_type::Vec2:
            return instance.srm.add_script(
                CreateDataScript<vec2&>(var->get<vec2>()));

        case pin_type::Vec3:
            return instance.srm.add_script(
                CreateDataScript<vec3&>(var->get<vec3>()));

        case pin_type::Vec4:
            return instance.srm.add_script(
                CreateDataScript<vec4&>(var->get<vec4>()));
        }

        return nullptr;
    }

    script* create_setter(pin_type pin_t, var_data* var, Instance& instance)
    {
        if (!var)
            throw script_exception("VariableScript: Cannot find variable!");

        if (var->get_type() != pin_t)
            throw script_exception("VariableScript: Badly formed variable!");

        switch (pin_t)
        {
		case pin_type::Bool:
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<bool>(bool)>(
					[var](const auto& x) {
						return var->get<bool>() = x;
					}, std::placeholders::_1));
			
		case pin_type::Byte:
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<uint8_t>(uint8_t)>(
					[var](const auto& x) {
						return var->get<uint8_t>() = x;
					}, std::placeholders::_1));
			
		case pin_type::Int:
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<int>(int)>(
					[var](const auto& x) {
						return var->get<int>() = x;
					}, std::placeholders::_1));

		case pin_type::Int64:
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<int64_t>(int64_t)>(
					[var](const auto& x) {
						return var->get<int64_t>() = x;
					}, std::placeholders::_1));
			
		case pin_type::Float:
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<float>(float)>(
					[var](const auto& x) {
						return var->get<float>() = x;
					}, std::placeholders::_1));
			
		case pin_type::String:
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<string>(string)>(
					[var](const auto& x) {
						return var->get<string>() = x;
					}, std::placeholders::_1));
			
		case pin_type::Vec2:
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<vec2>(vec2)>(
					[var](const auto& x) {
						return var->get<vec2>() = x;
					}, std::placeholders::_1));
			
		case pin_type::Vec3:
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<vec3>(vec3)>(
					[var](const auto& x) {
						return var->get<vec3>() = x;
					}, std::placeholders::_1));
			
		case pin_type::Vec4:
			return instance.srm.add_script(
				CreateRuntimeScript<std::tuple<vec4>(vec4)>(
					[var](const auto& x) {
						return var->get<vec4>() = x;
					}, std::placeholders::_1));
		}

        return nullptr;
    }
}