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
#include "px_deleter.h"

namespace Tempest
{
	/**
	 * @brief Used for PhysX. Smart pointer management for
	 * created pointers from PhysX.
	 */
	template <typename T>
	tsptr<T> px_make(T* s)
	{
		return tsptr<T>(s, px_deleter<T>());
	}
}