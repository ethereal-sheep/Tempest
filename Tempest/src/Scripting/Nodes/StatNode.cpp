
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
        case Tempest::GetStatNode::inner_type::Attacker:
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(void)>(
                    [&]() {

                        auto atker = instance.srm.get_variable_to_id(entity, "Attacker"); // attacking entity

                        Entity atk_e = (Entity)atker->get<int64_t>();

                        if (auto character = instance.ecs.get_if<tc::Character>(atk_e))
                        {
                            auto& stat = character->get_stat(stat_id);

                            if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                            {
                                return stat + weapon->get_stat(stat_id);
                            }
                            return stat;
                        }
                        return 0;
                    }));
            break;
        case Tempest::GetStatNode::inner_type::Defender:
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(void)>(
                    [&]() {
                        auto defer = instance.srm.get_variable_to_id(entity, "Defender"); // defending entity

                        Entity def_e = (Entity)defer->get<int64_t>();

                        if (auto character = instance.ecs.get_if<tc::Character>(def_e))
                        {
                            auto& stat = character->get_stat(id);

                            if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                            {
                                return stat + weapon->get_stat(id);
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
        std::string toReturn = get_name();

        return std::move(toReturn);
    }

    node_ptr SetStatNode::create_node(const std::string& typeinfo)
    {
        // we cannot check if stat exist here so we assume it does
        SetStatNode node(SetStatNode::inner_type::_cannot_be_empty);
        node.set_name(typeinfo);
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
        if (!statline || !statline->exist(get_name())) return nullptr; // might want reasonable msg here

        // get index of stat in statline
        auto index = statline->index_of_stat(get_name());

        // if have character sheet

        return instance.srm.add_script(
            CreateRuntimeScript<std::tuple<int>(int)>(
                [&](int x) {

                    if (auto character = instance.ecs.get_if<tc::Character>(entity))
                    {
                        auto& stat = character->get_stat(index);
                        return std::make_tuple(stat = x);
                    }
                    return std::make_tuple(0);

                }, std::placeholders::_1));
        

    }
    std::string GetMainStatNode::get_type_string()
    {
        // assuming no problem
        std::string toReturn = get_name();

        return std::move(toReturn);
    }

    node_ptr GetMainStatNode::create_node(const std::string& )
    {
        // we cannot check if stat exist here so we assume it does
        GetMainStatNode node(GetMainStatNode::inner_type::_cannot_be_empty);
        node.set_name("Main");
        node.add_output(pin_type::Int, "");

        return make_sptr<GetMainStatNode>(std::move(node));
    }

    script* GetMainStatNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]] )
    {
        /* steps:
        1. find character sheet

            if cannot find stat name, throw
        */

        // if have character sheet
        return instance.srm.add_script(
            CreateRuntimeScript<std::tuple<int>()>(
                [&]() {

                    if (auto character = instance.ecs.get_if<tc::Character>(entity))
                    {
                        auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon);

                        if (!weapon)
                            return std::make_tuple(character->get_stat(1));

                        return  std::make_tuple(
                            character->get_stat(weapon->get_main_stat()) + 
                            weapon->get_stat(weapon->get_main_stat()));
                    }
                    return std::make_tuple(0);
                }));

    }
}