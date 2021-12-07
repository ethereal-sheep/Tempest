/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
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
    std::string GroupNode::get_type_string()
    {
        return name;
    }

    node_ptr GroupNode::create_node(const std::string& typeinfo)
    {
        GroupNode node(GroupNode::inner_type::Default);
        node.set_name(typeinfo);
        return std::make_unique<GroupNode>(std::move(node));
    }

    script* GroupNode::create_script(Entity entity [[maybe_unused]], Instance& instance [[maybe_unused]] )
    {
        return nullptr;
    }
}