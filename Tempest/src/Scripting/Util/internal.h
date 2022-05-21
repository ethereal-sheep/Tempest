/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Core.h"
#include "static_id_generator.h"

namespace Tempest
{
	using pin_id_t = id_t;
	using node_id_t = id_t;

	class node;
	using node_ptr = tsptr<node>;


	enum struct pin_type
	{
		Flow,
		Bool, Byte, Int, Int64, Float, String, Vec2, Vec3, Vec4,
		END,
		Entity, Vector
	};

	/*
	* pin_id_t
	* 0b 0 000 0...0
	*    ^ ^   ^
	*    1 2   3
	* 
	* 1. 1 bit:		INPUT/OUTPUT flag
	* 2. 7 bits:	INDEX of pin
	* 3. 24 bits:	NODE id of parent
	*/

	[[nodiscard]] inline constexpr std::tuple<bool, size_t, node_id_t> pin_to_component(pin_id_t pin)
	{
		return std::make_tuple(
			static_cast<bool>((pin >> 31) & 0x01),
			static_cast<size_t>(((pin >> 24) & 0x7F) - 1),
			static_cast<node_id_t>(pin & 0xFFFFFF));
	}

	[[nodiscard]] inline constexpr pin_id_t create_pin_id(bool input, size_t index, node_id_t parent)
	{
		pin_id_t id = 0;
		id |= parent;
		id |= (index+1) << 24;
		id |= input << 31;

		return id;
	}

	[[nodiscard]] inline constexpr uint64_t concatenate_id_t(id_t id1, id_t id2)
	{
		return (static_cast<uint64_t>(id1) << 32) + id2;
	}
	[[nodiscard]] inline constexpr tpair<id_t, id_t> split_uint64_t(uint64_t id)
	{
		return std::make_pair(
			static_cast<id_t>((id & 0xFFFFFFFF00000000LL) >> 32),
			static_cast<id_t>(id & 0xFFFFFFFFLL));
	}
}

