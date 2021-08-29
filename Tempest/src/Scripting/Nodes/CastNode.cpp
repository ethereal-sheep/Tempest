
#include "../Util/node.h"
#include "Instance/Instance.h"

namespace Tempest
{
    std::string CastNode::get_type_string()
    {
        // assuming no problem
        pin_type Startpin_type = node::get_inputs()[0].get_type();
        pin_type Endpin_type = node::get_outputs()[0].get_type();
        
        std::string toReturn = magic_enum::enum_name(Startpin_type).data();
        toReturn += ":";
        toReturn += magic_enum::enum_name(Endpin_type).data();

        return std::move(toReturn);
    }

    node_ptr CastNode::create_node(const std::string& typeinfo)
    {
        // parse type info
        auto v = algo::delimited_split(typeinfo, ":");
        if(v.size() != 2)
        {
            throw node_exception("CreateNode: Bad inner type!");
        }

        auto start = magic_enum::enum_cast<pin_type>(v[0]);
        auto end = magic_enum::enum_cast<pin_type>(v[1]);

        if(!start.has_value() || !end.has_value())
        {
            throw node_exception("CreateNode: Bad inner type!");
        }

        CastNode node(CastNode::inner_type::_cannot_be_empty);
        node.add_input(start.value(), "");
		node.add_output(end.value(), "");
        
        return make_sptr<CastNode>(std::move(node));
    }

    script* CastNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]])
    {
        // assuming created correctly
        pin_type Startpin_type = get_inputs()[0].get_type();
        pin_type Endpin_type = get_outputs()[0].get_type();

        switch (Startpin_type)
        {
        case pin_type::Bool:
        {
            switch (Endpin_type)
            {
            case pin_type::Byte:
                return instance.srm.add_script(
                CreateConstexprScript<std::tuple<uint8_t>(bool)>(
                    [](const bool& x) {
                        return static_cast<uint8_t>(x);
                    }, std::placeholders::_1));

            case pin_type::Int:
                return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(bool)>(
                    [](const bool& x) {
                        return static_cast<int>(x);
                    }, std::placeholders::_1));

            case pin_type::Int64:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<int64_t>(bool)>(
                        [](const bool& x) {
                            return static_cast<int64_t>(x);
                        }, std::placeholders::_1)); 

            case pin_type::Float:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<float>(bool)>(
                        [](const bool& x) {
                            return static_cast<float>(x);
                        }, std::placeholders::_1)); 

            case pin_type::String:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<std::string>(bool)>(
                        [](const bool& x) {
                            return (x ? std::string{ "true" } : std::string{ "false" });
                        }, std::placeholders::_1)); 

            }
        }
        case pin_type::Byte:
        {
            switch (Endpin_type)
            {
            case pin_type::Bool:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<bool>(uint8_t)>(
                        [](const bool& x) {
                            return static_cast<bool>(x);
                        }, std::placeholders::_1)); 

            case pin_type::Int:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<int>(uint8_t)>(
                        [](const bool& x) {
                            return static_cast<int>(x);
                        }, std::placeholders::_1)); 

            case pin_type::Int64:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<int64_t>(uint8_t)>(
                        [](const bool& x) {
                            return static_cast<int64_t>(x);
                        }, std::placeholders::_1)); 

            case pin_type::Float:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<float>(uint8_t)>(
                        [](const bool& x) {
                            return static_cast<float>(x);
                        }, std::placeholders::_1)); 

            case pin_type::String:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<string>(uint8_t)>(
                        [](const bool& x) {
                            return std::to_string(x);
                        }, std::placeholders::_1)); 

            }
        }
        case pin_type::Int:
        {
            switch (Endpin_type)
            {
            case pin_type::Bool:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<bool>(int)>(
                        [](const int& x) {
                            return static_cast<bool>(x);
                        }, std::placeholders::_1)); 

            case pin_type::Byte:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<uint8_t>(int)>(
                        [](const int& x) {
                            return static_cast<uint8_t>(x);
                        }, std::placeholders::_1)); 

            case pin_type::Int64:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<int64_t>(int)>(
                        [](const int& x) {
                            return static_cast<int64_t>(x);
                        }, std::placeholders::_1)); 

            case pin_type::Float:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<float>(int)>(
                        [](const int& x) {
                            return static_cast<float>(x);
                        }, std::placeholders::_1)); 

            case pin_type::String:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<std::string>(int)>(
                        [](const int& x) {
                            return std::to_string(x);
                        }, std::placeholders::_1)); 
            }

        }
        case pin_type::Entity:
        {
            switch (Endpin_type)
            {
            case pin_type::String:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<string>(uint32_t)>(
                        [](const uint32_t& x) {
                            return std::to_string(x);
                        }, std::placeholders::_1)); 

            }
        }
        case pin_type::Int64:
        {
            switch (Endpin_type)
            {
            case pin_type::Bool:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<bool>(int64_t)>(
                        [](const int64_t& x) {
                            return static_cast<bool>(x);
                        }, std::placeholders::_1)); 

            case pin_type::Byte:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<uint8_t>(int64_t)>(
                        [](const int64_t& x) {
                            return static_cast<uint8_t>(x);
                        }, std::placeholders::_1)); 

            case pin_type::Int:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<int>(int64_t)>(
                        [](const int64_t& x) {
                            return static_cast<int>(x);
                        }, std::placeholders::_1)); 

            case pin_type::Float:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<float>(int64_t)>(
                        [](const int64_t& x) {
                            return static_cast<float>(x);
                        }, std::placeholders::_1)); 

            case pin_type::String:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<std::string>(int64_t)>(
                        [](const int64_t& x) {
                            return std::to_string(x);
                        }, std::placeholders::_1)); 
            }
        }
        case pin_type::Float:
        {
            switch (Endpin_type)
            {
            case pin_type::Bool:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<bool>(float)>(
                        [](const float& x) {
                            return static_cast<bool>(x);
                        }, std::placeholders::_1)); 

            case pin_type::Byte:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<uint8_t>(float)>(
                        [](const float& x) {
                            return static_cast<uint8_t>(x);
                        }, std::placeholders::_1)); 

            case pin_type::Int:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<int>(float)>(
                        [](const float& x) {
                            return static_cast<int>(x);
                        }, std::placeholders::_1)); 

            case pin_type::Int64:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<int64_t>(float)>(
                        [](const float& x) {
                            return static_cast<int64_t>(x);
                        }, std::placeholders::_1)); 

            case pin_type::String:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<std::string>(float)>(
                        [](const float& x) {
                            return std::to_string(x);
                        }, std::placeholders::_1)); 

            }
        }
        case pin_type::Vec2:
        {
            switch (Endpin_type)
            {
            case pin_type::String:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<string>(vec2)>(
                        [](const vec2& vec) {
                            return ((std::string{ "x: " }
                                += std::to_string(vec.x))
                                += " y: ")
                                += std::to_string(vec.y);
                        }, std::placeholders::_1)); 
            case pin_type::Vec3:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<vec3>(vec2)>(
                        [](const vec2& vec) {
                            return std::make_tuple(vec3{ vec.x, vec.y, 0 });
                        }, std::placeholders::_1)); 
            case pin_type::Vec4:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<vec4>(vec2)>(
                        [](const vec2& vec) {
                            return std::make_tuple(vec4{ vec.x, vec.y, 0, 0 });
                        }, std::placeholders::_1)); 
            }
        }
        case pin_type::Vec3:
        {
            switch (Endpin_type)
            {
            case pin_type::String:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<string>(vec3)>(
                        [](const vec3& vec) {
                            return ((((std::string{ "x: " }
                                += std::to_string(vec.x))
                                += " y: ")
                                += std::to_string(vec.y))
                                += " z: ") += std::to_string(vec.z);
                        }, std::placeholders::_1)); 
            case pin_type::Vec2:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<vec2>(vec3)>(
                        [](const vec3& vec) {
                            return std::make_tuple(vec2{ vec.x, vec.y });
                        }, std::placeholders::_1)); 
            case pin_type::Vec4:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<vec4>(vec3)>(
                        [](const vec3& vec) {
                            return std::make_tuple(vec4{ vec.x, vec.y, vec.z, 0 });
                        }, std::placeholders::_1)); 
            }
        }
        case pin_type::Vec4:
        {
            switch (Endpin_type)
            {
            case pin_type::String:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<string>(vec4)>(
                        [](const vec4& vec) {
                            return ((((((std::string{ "x: " }
                                += std::to_string(vec.x))
                                += " y: ")
                                += std::to_string(vec.y))
                                += " z: ")
                                += std::to_string(vec.z))
                                += " w: ")
                                += std::to_string(vec.w);
                        }, std::placeholders::_1)); 
            case pin_type::Vec2:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<vec2>(vec4)>(
                        [](const vec4& vec) {
                            return std::make_tuple(vec2{ vec.x, vec.y });
                        }, std::placeholders::_1)); 
            case pin_type::Vec3:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<vec3>(vec4)>(
                        [](const vec4& vec) {
                            return std::make_tuple(vec3{ vec.x, vec.y, vec.z });
                        }, std::placeholders::_1)); 
            }
        }

        }
        // default throw
        return nullptr;
    }

}