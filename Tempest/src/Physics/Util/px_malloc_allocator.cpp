/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "px_malloc_allocator.h"

namespace Tempest
{
	px_malloc_allocator_t& px_malloc_allocator()
	{
		static px_malloc_allocator_t alloc;
		return alloc;
	}
}