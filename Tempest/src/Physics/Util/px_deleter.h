/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "px_include.h"

namespace Tempest
{
	/**
	 * @brief Used for PhysX. Custom deleter for physx objects.
	 */
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