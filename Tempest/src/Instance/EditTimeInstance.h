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
			Instance(root_directory, strategy)
		{
			// 
		}

		void _init() override;

		void _update([[maybe_unused]] float dt) override;

		void _render() override;

		void _exit() override;

		void save();

		void save_as(const tpath&);

		void save_current_scene();
		void save_current_conflict_resolution();

		bool delete_scene(const string& scene_name);
		bool delete_conflict_resolution(int i);

		string create_new_scene(const string& scene_name = "Map");
		void create_new_conflict_resolution(int i, const string& res_name);

		


	public:
	};
}

