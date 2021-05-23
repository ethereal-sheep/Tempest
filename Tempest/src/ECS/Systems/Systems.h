#pragma once

#include "..\..\TMath.h"
#include "..\ECS.h"

namespace Tempest::Systems
{
	struct dynamic_system
	{
		template <typename Func>
		void operator()(Tempest::runtime_view view, Func func)
		{
			view.each(func);
		}
	};

}

namespace ts = Tempest::Systems;