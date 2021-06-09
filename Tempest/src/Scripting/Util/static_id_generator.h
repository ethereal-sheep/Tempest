#pragma once
#include "Core.h"

namespace Tempest
{
	struct static_id_generator
	{
		static id_t generate()
		{
			static id_t gen = 1;
			return gen++;
		}
	};

	using idgen = static_id_generator;

}