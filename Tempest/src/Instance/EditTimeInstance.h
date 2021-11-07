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

namespace Tempest
{
	/**
	 * @brief Type of Instance that is loaded for edit time
	 * implementation requires a path to the root folder
	 * of the loading project/scene/map etc.
	 */
	class EditTimeInstance : public Instance
	{
	public:
		/**
		 * @brief Requires root directory
		 */
		EditTimeInstance(const tpath& root_directory, MemoryStrategy strategy = {}) :
			Instance(root_directory, strategy), graph_manager(memory_object.get())
		{
			// 
		}

		void _init() override;

		void _update([[maybe_unused]] float dt) override;

		void _render() override;

		void _exit() override;

		void save()
		{
			scene.save(root);
			ecs.save(root);
			//graph_manager.save_all_to(root);
		}

		void save_as(const tpath& )
		{

		}

	private:


	public:
		GMS graph_manager;
	};
}

