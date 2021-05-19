#pragma once

#include "../Core.h"

namespace Tempest
{
	using Entity = std::uint32_t;
	using EntityType = std::uint32_t;
	using EnVector = tvector<Entity>;

	static constexpr size_t MAX_ENTITY = 10001;
}