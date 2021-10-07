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
#include "Instance.h"
#include "Scripting/SRM.h"
#include "Scripting/GMS.h"
#include "Physics/Physics.h"


namespace Tempest
{
	

	/**
	 * @brief Type of Instance that is loaded for runtime
	 * implementation requires a path to the root folder
	 * of the loading project/scene/map etc.
	 */
	class RuntimeInstance : public Instance
	{

	public:
		/**
		 * @brief Requires root directory
		 */
		RuntimeInstance(const tpath& project_path, MemoryStrategy strategy = {}) :
			Instance(project_path, strategy)
		{

			build_scripts(root);
		}

		void _init() override;
		void _update(float dt) override;
		void _render() override;
		void _exit() override;

		
	private:
		void build_scripts(const tpath& root_directory);

	public:
	};
}

