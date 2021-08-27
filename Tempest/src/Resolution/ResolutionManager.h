#pragma once

#include "ECS/Entity.h"
#include "Statline.h"

namespace Tempest
{
	class ResolutionManager
	{

	public:

		Entity attacking = INVALID;
		Entity defending = INVALID;
		Statline statline;
	};
}