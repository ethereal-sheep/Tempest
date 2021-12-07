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
#include "Instance/Instance.h"

namespace Tempest
{
	/**
	 * @brief Type of Instance that is loaded for edit time
	 * implementation requires a path to the root folder
	 * of the loading project/scene/map etc.
	 */
	class CreatorInstance : public Instance
	{
	public:
		/**
		 * @brief Requires root directory
		 */
		CreatorInstance(MemoryStrategy strategy = {}) :
			Instance("", "", "", strategy)
		{
			// 
		}

		void _init() override;

		void _update([[maybe_unused]] float dt) override;

		void _render() override;

		void _exit() override;

		void retarget(tpath path)
		{
			full_path = path;
		}

	private:


	public:
	};
}

