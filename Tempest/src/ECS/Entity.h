/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "../Core.h"

namespace Tempest
{
	using Entity = uint32_t;
	using EnVector = tvector<Entity>;

	static constexpr uint32_t MAX_ENTITY = 65536u;
	static constexpr Entity INVALID = NULL;
}