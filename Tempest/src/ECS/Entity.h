#pragma once

#include "../Core.h"

namespace Tempest
{
	using Entity = uint32_t;
	using EnVector = tvector<Entity>;

	static constexpr uint32_t MAX_ENTITY = 16384u;
	static constexpr Entity INVALID = NULL;
}