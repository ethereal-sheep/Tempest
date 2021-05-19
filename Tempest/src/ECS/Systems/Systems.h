#pragma once

#include "..\..\Math.h"
#include "..\Entity.h"

namespace Tempest::Systems
{
	struct TestSystem
	{
		template <typename TData>
		static void update(float dt, TData& data)
		{
			data.for_entities([&](auto id)
				{
					// do something here
				});
		}
	};

}

namespace ts = Tempest::Systems;