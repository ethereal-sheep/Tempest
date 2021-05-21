#pragma once

#include "..\..\TMath.h"
#include "..\ECS.h"

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


	struct test_system
	{
		template <typename Func>
		void operator()(Tempest::runtime_view view, Func func)
		{
			view.each(func);
		}
	};

}

namespace ts = Tempest::Systems;