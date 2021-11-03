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

namespace Tempest
{
    std::string GetStatNode::get_type_string()
    {
        // assuming no problem
        std::string toReturn = magic_enum::enum_name(type).data();
        toReturn += ":";
        toReturn += get_name();

        return std::move(toReturn);
    }

    node_ptr GetStatNode::create_node(const std::string& typeinfo)
    {
        // parse type info
        auto v = algo::delimited_split(typeinfo, ":");
        if (v.size() != 2)
        {
            throw node_exception("GetStatNode: Bad inner type!");
        }
        // attacker/defender | stat_id
        auto type = magic_enum::enum_cast<GetStatNode::inner_type>(v[0]);

        if (!type.has_value())
        {
            throw node_exception("GetStatNode: Bad inner type!");
        }

        // we cannot check if stat exist here so we assume it does
        GetStatNode node(type.value());
        node.set_name(v[1]);
        node.add_output(pin_type::Int, "");

        return make_sptr<GetStatNode>(std::move(node));
    }

    script* GetStatNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]] )
    {
        /* steps:
        1. from name derive
            stat name

            if cannot find stat name, throw
        */
        // get the statline
        tc::Statline* statline = nullptr;
        for (auto i : instance.ecs.view<tc::Statline>())
            statline = instance.ecs.get_if<tc::Statline>(i);

        // no statline or no stat, throw
        if (!statline) throw script_exception("Bad Script! No Statline exists!");

        // get index of stat in statline
        auto stat_id = std::stoi(get_name());
        if (stat_id >= statline->size()) throw script_exception("Bad Script! No Stat exists!");
        
        switch (type)
        {
        case Tempest::GetStatNode::inner_type::Owner:
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(void)>(
                    [&instance, entity, sid = stat_id]() {

                        LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Owner"));
                        LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Enemy"));

                        auto atker = instance.srm.get_variable_to_id(entity, "Owner"); // attacking entity

                        Entity atk_e = (Entity)atker->get<int64_t>();

                        if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                        {
                            auto& stat = character->get_stat(sid);

                            if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                            {
                                return stat + weapon->get_stat(sid);
                            }
                            return stat;
                        }
                        return 0;
                    }));
            break;
        case Tempest::GetStatNode::inner_type::Enemy:
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(void)>(
                    [&instance, entity, sid = stat_id]() {

                        LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Owner"));
                        LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Enemy"));

                        auto defer = instance.srm.get_variable_to_id(entity, "Enemy"); // defending entity

                        Entity def_e = (Entity)defer->get<int64_t>();

                        if (auto character = instance.ecs.get_if<tc::Character>(def_e))
                        {
                            auto& stat = character->get_stat(sid);

                            if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                            {
                                return stat + weapon->get_stat(sid);
                            }
                            return stat;
                        }
                        return 0;
                    }));
            break;
        default:
            break;
        }

        return nullptr;
            
    }

    std::string SetStatNode::get_type_string()
    {
        // assuming no problem
        std::string toReturn = magic_enum::enum_name(type).data();
        toReturn += ":";
        toReturn += get_name();

        return std::move(toReturn);
    }

    node_ptr SetStatNode::create_node(const std::string& typeinfo)
    {
        auto v = algo::delimited_split(typeinfo, ":");
        if (v.size() != 2)
        {
            throw node_exception("SetStatNode: Bad inner type!");
        }
        // attacker/defender | stat_id
        auto type = magic_enum::enum_cast<SetStatNode::inner_type>(v[0]);

        if (!type.has_value())
        {
            throw node_exception("SetStatNode: Bad inner type!");
        }

        // we cannot check if stat exist here so we assume it does
        SetStatNode node(type.value());
        node.set_name(v[1]);
        node.add_input(pin_type::Flow, "");
        node.add_input(pin_type::Int, "");
        node.add_output(pin_type::Flow, "");
        node.add_output(pin_type::Int, "");

        return make_sptr<SetStatNode>(std::move(node));
    }

    script* SetStatNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]] )
    {
        /* steps:
        1. from name derive
            stat name

            if cannot find stat name, throw
        */
        // get the statline
        tc::Statline* statline = nullptr;
        for (auto i : instance.ecs.view<tc::Statline>())
            statline = instance.ecs.get_if<tc::Statline>(i);

        // no statline or no stat, throw
        if (!statline) throw script_exception("Bad Script! No Statline exists!");

        // get index of stat in statline
        auto stat_id = std::stoi(get_name());
        if (stat_id >= statline->size()) throw script_exception("Bad Script! No Stat exists!");

        switch (type)
        {
        case Tempest::SetStatNode::inner_type::Owner:
            return instance.srm.add_script(
                CreateRuntimeScript<std::tuple<int>(int)>(
                    [&instance, entity, sid = stat_id](int x) {

                        LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Owner"));
                        LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Enemy"));

                        auto atker = instance.srm.get_variable_to_id(entity, "Owner"); // attacking entity

                        Entity atk_e = (Entity)atker->get<int64_t>();

                        if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                        {
                            auto& stat = character->get_stat(sid);
                            return stat = x;
                        }
                        return 0;
                    }, std::placeholders::_1));
            break;
        case Tempest::SetStatNode::inner_type::Enemy:
            return instance.srm.add_script(
                CreateRuntimeScript<std::tuple<int>(int)>(
                    [&instance, entity, sid = stat_id](int x) {


                        LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Owner"));
                        LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Enemy"));

                        auto defer = instance.srm.get_variable_to_id(entity, "Enemy"); // defending entity

                        Entity def_e = (Entity)defer->get<int64_t>();

                        if (auto character = instance.ecs.get_if<tc::Character>(def_e))
                        {
                            auto& stat = character->get_stat(sid);
                            return stat = x;
                        }
                        return 0;
                    }, std::placeholders::_1));
            break;
        default:
            break;
        }

        return nullptr;
    }
    std::string GetMainStatNode::get_type_string()
    {
        // assuming no problem
        std::string toReturn = get_name();

        return std::move(toReturn);
    }

    node_ptr GetMainStatNode::create_node(const std::string& typeinfo)
    {
        // we cannot check if stat exist here so we assume it does
        auto type = magic_enum::enum_cast<GetMainStatNode::inner_type>(typeinfo);

        if (!type.has_value())
        {
            throw node_exception("GetMainStatNode: Bad inner type!");
        }

        GetMainStatNode node(type.value());
        switch (type.value())
        {
        case Tempest::GetMainStatNode::inner_type::Owner:
            node.set_name("Get Owner MAIN");
            node.add_output(pin_type::Int, "");
            break;
        case Tempest::GetMainStatNode::inner_type::Enemy:
            node.set_name("Get Enemy MAIN");
            node.add_output(pin_type::Int, "");
            break;
        default:
            break;
        }

        return make_sptr<GetMainStatNode>(std::move(node));
    }

    script* GetMainStatNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]] )
    {
        /* steps:
        1. find character sheet

            if cannot find stat name, throw
        */
        tc::Statline* statline = nullptr;
        for (auto i : instance.ecs.view<tc::Statline>())
            statline = instance.ecs.get_if<tc::Statline>(i);

        // no statline or no stat, throw
        if (!statline) throw script_exception("Bad Script! No Statline exists!");

        switch (type)
        {
        case Tempest::GetMainStatNode::inner_type::Owner:
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(void)>(
                    [&instance, entity]() {
                        LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Owner"));
                        LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Enemy"));

                        auto atker = instance.srm.get_variable_to_id(entity, "Owner"); // attacking entity

                        Entity atk_e = (Entity)atker->get<int64_t>();

                        if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                        {
                            if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                            {
                                auto sid = weapon->get_main_stat();
                                return  weapon->get_stat(sid) + character->get_stat(sid);
                            }
                        }
                        return 0;
                    }));
            break;
        case Tempest::GetMainStatNode::inner_type::Enemy:
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(void)>(
                    [&instance, entity]() {

                        LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Owner"));
                        LOG_ASSERT(instance.srm.get_variable_to_id(entity, "Enemy"));

                        auto defer = instance.srm.get_variable_to_id(entity, "Enemy"); // defending entity

                        Entity def_e = (Entity)defer->get<int64_t>();

                        if (auto character = instance.ecs.get_if<tc::Character>(def_e))
                        {
                            if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                            {
                                auto sid = weapon->get_main_stat();
                                return  weapon->get_stat(sid) + character->get_stat(sid);
                            }
                        }
                        return 0;
                    }));
            break;
        default:
            break;
        }

        return nullptr;
    }



    std::string StatNode::get_type_string()
    {
        // assuming no problem
        std::string toReturn = magic_enum::enum_name(type).data();
        toReturn += ":";
        toReturn += get_name();

        return std::move(toReturn);
    }

    node_ptr StatNode::create_node(const std::string& typeinfo)
    {
        // parse type info
        auto v = algo::delimited_split(typeinfo, ":");
        if (v.size() != 2)
        {
            throw node_exception("StatNode: Bad inner type!");
        }
        // attacker/defender | stat_id
        auto type = magic_enum::enum_cast<StatNode::inner_type>(v[0]);

        if (!type.has_value())
        {
            throw node_exception("StatNode: Bad inner type!");
        }

        // we cannot check if stat exist here so we assume it does

        StatNode node(type.value());
        switch (type.value())
        {
        case Tempest::StatNode::inner_type::GetStat:
            node.set_name(v[1]);
            node.add_input(pin_type::Byte, "");
            node.add_output(pin_type::Int, "");
            break;
        case Tempest::StatNode::inner_type::SetStat:
            node.set_name(v[1]);
            node.add_input(pin_type::Flow, "");
            node.add_input(pin_type::Byte, "");
            node.add_input(pin_type::Int, "");
            node.add_output(pin_type::Flow, "");
            node.add_output(pin_type::Int, "");
            break;
        case Tempest::StatNode::inner_type::GetMain:
            node.set_name(v[1]);
            node.add_input(pin_type::Byte, "");
            node.add_output(pin_type::Int, "");
            break;
        case Tempest::StatNode::inner_type::SetMain:
            node.set_name(v[1]);
            node.add_input(pin_type::Flow, "");
            node.add_input(pin_type::Byte, "");
            node.add_input(pin_type::Int, "");
            node.add_output(pin_type::Flow, "");
            node.add_output(pin_type::Int, "");
            break;
        case Tempest::StatNode::inner_type::END:
        default:
            break;
        }

        return make_sptr<StatNode>(std::move(node));
    }

    script* StatNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]] )
    {
        /* steps:
        1. from name derive
            stat name

            if cannot find stat name, throw
        */
        // get the statline
        tc::Statline* statline = nullptr;
        for (auto i : instance.ecs.view<tc::Statline>())
            statline = instance.ecs.get_if<tc::Statline>(i);

        // no statline or no stat, throw
        if (!statline) throw script_exception("Bad Script! No Statline exists!");

        // get index of stat in statline
        auto stat_id = std::stoi(get_name());
        if (stat_id >= statline->size()) throw script_exception("Bad Script! No Stat exists!");

        switch (type)
        {
        case Tempest::StatNode::inner_type::GetStat:
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(uint8_t)>(
                    [&instance, entity, sid = stat_id](uint8_t index) {

                        if (index == 0)
                        {
                            // owner + attacker
                            if (auto owner = instance.srm.get_variable_to_id(entity, "Owner"))
                            {
                                LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                Entity own_e = (Entity)owner->get<int64_t>();

                                if (auto character = instance.ecs.get_if<tc::Character>(own_e))
                                {
                                    auto& stat = character->get_stat(sid);
                                    if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                                    {
                                        return stat + weapon->get_stat(sid);
                                    }
                                    return stat;
                                }
                            }
                            // attacker
                            if (auto owner = instance.srm.get_variable_to_id(entity, "Attacker"))
                            {
                                LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                Entity own_e = (Entity)owner->get<int64_t>();

                                if (auto character = instance.ecs.get_if<tc::Character>(own_e))
                                {
                                    auto& stat = character->get_stat(sid);
                                    if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                                    {
                                        return stat + weapon->get_stat(sid);
                                    }
                                    return stat;
                                }
                            }
                        }
                        else if(index == 1)
                        {
                            // owner + attacker
                            if (auto owner = instance.srm.get_variable_to_id(entity, "Enemy"))
                            {
                                LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                Entity own_e = (Entity)owner->get<int64_t>();

                                if (auto character = instance.ecs.get_if<tc::Character>(own_e))
                                {
                                    auto& stat = character->get_stat(sid);
                                    if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                                    {
                                        return stat + weapon->get_stat(sid);
                                    }
                                    return stat;
                                }
                            }
                            // attacker
                            if (auto owner = instance.srm.get_variable_to_id(entity, "Defender"))
                            {
                                LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                Entity own_e = (Entity)owner->get<int64_t>();

                                if (auto character = instance.ecs.get_if<tc::Character>(own_e))
                                {
                                    auto& stat = character->get_stat(sid);
                                    if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                                    {
                                        return stat + weapon->get_stat(sid);
                                    }
                                    return stat;
                                }
                            }
                        }

                        return 0;
                    }, std::placeholders::_1));
            break;
        case Tempest::StatNode::inner_type::SetStat:
            return instance.srm.add_script(
                CreateRuntimeScript<std::tuple<int>(uint8_t, int)>(
                    [&instance, entity, sid = stat_id](uint8_t index, int x) {

                        if (index == 0)
                        {
                            // owner + attacker
                            if (auto owner = instance.srm.get_variable_to_id(entity, "Owner"))
                            {
                                LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                Entity atk_e = (Entity)owner->get<int64_t>();

                                if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                                {
                                    auto& stat = character->get_stat(sid);
                                    return stat = x;
                                }
                            }
                            // attacker
                            if (auto owner = instance.srm.get_variable_to_id(entity, "Attacker"))
                            {
                                LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                Entity atk_e = (Entity)owner->get<int64_t>();

                                if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                                {
                                    auto& stat = character->get_stat(sid);
                                    return stat = x;
                                }
                            }
                        }
                        else if (index == 1)
                        {
                            // owner + attacker
                            if (auto owner = instance.srm.get_variable_to_id(entity, "Enemy"))
                            {
                                LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                Entity atk_e = (Entity)owner->get<int64_t>();

                                if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                                {
                                    auto& stat = character->get_stat(sid);
                                    return stat = x;
                                }
                            }
                            // attacker
                            if (auto owner = instance.srm.get_variable_to_id(entity, "Defender"))
                            {
                                LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                Entity atk_e = (Entity)owner->get<int64_t>();

                                if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                                {
                                    auto& stat = character->get_stat(sid);
                                    return stat = x;
                                }
                            }
                        }

                        return 0;

                    }, std::placeholders::_1, std::placeholders::_2));
            break; 
        case Tempest::StatNode::inner_type::GetMain:
                return instance.srm.add_script(
                    CreateConstexprScript<std::tuple<int>(uint8_t)>(
                        [&instance, entity](uint8_t index) {

                            if (index == 0)
                            {
                                // owner + attacker
                                if (auto owner = instance.srm.get_variable_to_id(entity, "Owner"))
                                {
                                    LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                    Entity atk_e = (Entity)owner->get<int64_t>();

                                    if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                                    {
                                        if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                                        {
                                            auto sid = weapon->get_main_stat();
                                            return  weapon->get_stat(sid) + character->get_stat(sid);
                                        }
                                    }
                                }
                                // attacker
                                if (auto owner = instance.srm.get_variable_to_id(entity, "Attacker"))
                                {
                                    LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                    Entity atk_e = (Entity)owner->get<int64_t>();

                                    if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                                    {
                                        if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                                        {
                                            auto sid = weapon->get_main_stat();
                                            return  weapon->get_stat(sid) + character->get_stat(sid);
                                        }
                                    }
                                }
                            }
                            else if (index == 1)
                            {
                                // owner + attacker
                                if (auto owner = instance.srm.get_variable_to_id(entity, "Enemy"))
                                {
                                    LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                    Entity atk_e = (Entity)owner->get<int64_t>();

                                    if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                                    {
                                        if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                                        {
                                            auto sid = weapon->get_main_stat();
                                            return  weapon->get_stat(sid) + character->get_stat(sid);
                                        }
                                    }
                                }
                                // attacker
                                if (auto owner = instance.srm.get_variable_to_id(entity, "Defender"))
                                {
                                    LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                    Entity atk_e = (Entity)owner->get<int64_t>();

                                    if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                                    {
                                        if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                                        {
                                            auto sid = weapon->get_main_stat();
                                            return  weapon->get_stat(sid) + character->get_stat(sid);
                                        }
                                    }
                                }
                            }
                            return 0;
                        }, std::placeholders::_1));
                break;
            case Tempest::StatNode::inner_type::SetMain:
                return instance.srm.add_script(
                    CreateRuntimeScript<std::tuple<int>(uint8_t, int)>(
                        [&instance, entity](uint8_t index, int x) {

                            if (index == 0)
                            {
                                // owner + attacker
                                if (auto owner = instance.srm.get_variable_to_id(entity, "Owner"))
                                {
                                    LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                    Entity atk_e = (Entity)owner->get<int64_t>();

                                    if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                                    {
                                        if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                                        {
                                            auto sid = weapon->get_main_stat();
                                            auto& stat = character->get_stat(sid);
                                            return stat = x;
                                        }
                                    }
                                }
                                // attacker
                                if (auto owner = instance.srm.get_variable_to_id(entity, "Attacker"))
                                {
                                    LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                    Entity atk_e = (Entity)owner->get<int64_t>();

                                    if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                                    {
                                        if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                                        {
                                            auto sid = weapon->get_main_stat();
                                            auto& stat = character->get_stat(sid);
                                            return stat = x;
                                        }
                                    }
                                }
                            }
                            else if (index == 1)
                            {
                                // owner + attacker
                                if (auto owner = instance.srm.get_variable_to_id(entity, "Enemy"))
                                {
                                    LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                    Entity atk_e = (Entity)owner->get<int64_t>();

                                    if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                                    {
                                        if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                                        {
                                            auto sid = weapon->get_main_stat();
                                            auto& stat = character->get_stat(sid);
                                            return stat = x;
                                        }
                                    }
                                }
                                // attacker
                                if (auto owner = instance.srm.get_variable_to_id(entity, "Defender"))
                                {
                                    LOG_ASSERT(owner->get_type() == pin_type::Int64);
                                    Entity atk_e = (Entity)owner->get<int64_t>();

                                    if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                                    {
                                        if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                                        {
                                            auto sid = weapon->get_main_stat();
                                            auto& stat = character->get_stat(sid);
                                            return stat = x;
                                        }
                                    }
                                }
                            }

                            return 0;

                        }, std::placeholders::_1, std::placeholders::_2));
                break;
        default:
            break;
        }

        return nullptr;

    }

}