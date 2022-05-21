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