#pragma once

#include "px_include.h"

namespace Tempest
{
	template <typename Tpx>
	struct px_deleter
	{
		void operator()(Tpx* px_item)
		{
			if(px_item)
				px_item->release();
		}
	};
}