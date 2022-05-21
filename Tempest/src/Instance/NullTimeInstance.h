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
#include "Instance.h"
#include "Scripting/SRM.h"
#include "Scripting/GMS.h"

namespace Tempest
{
	/**
	 * @brief Type of Instance that is loaded for runtime
	 * implementation requires a path to the root folder
	 * of the loading project/scene/map etc.
	 */
	class NullTimeInstance : public Instance
	{
	public:
		/**
		 * @brief Requires root directory
		 */
		NullTimeInstance(MemoryStrategy strategy = {}) :
			Instance("", "", "", strategy)
		{
		}

		void _init() override {}
		void _update([[maybe_unused]] float dt) override {}
		void _render() override	{}
		void _exit() override {}

	};
}

