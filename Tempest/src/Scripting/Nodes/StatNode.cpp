
#include "../Util/node.h"
#include "Instance/Instance.h"

namespace Tempest
{
    std::string GetStatNode::get_type_string()
    {
        // assuming no problem
        std::string toReturn = get_name();

        return std::move(toReturn);
    }

    node_ptr GetStatNode::create_node(const std::string& typeinfo)
    {
        // we cannot check if stat exist here so we assume it does
        GetStatNode node(GetStatNode::inner_type::_cannot_be_empty);
        node.set_name(typeinfo);
        node.add_output(pin_type::Int, "");

        return make_uptr<GetStatNode>(std::move(node));
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
        if (!statline || !statline->exist(get_name())) return nullptr; // might want reasonable msg here

        // get index of stat in statline
        auto index = statline->index_of_stat(get_name());
        
        // if have character sheet
            return instance.srm.add_script(
                CreateConstexprScript<std::tuple<int>(void)>(
                    [&]() {

                        if (auto character = instance.ecs.get_if<tc::Character>(entity))
                        {
                            auto& stat = character->get_stat(index);

                            if (auto weapon = instance.ecs.get_if<tc::Weapon>(character->weapon))
                            {
                                return stat + weapon->get_stat(index);
                            }
                            return stat;
                        }
                        return 0;
                    }));
        

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

        return make_uptr<SetStatNode>(std::move(node));
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

        return make_uptr<GetMainStatNode>(std::move(node));
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