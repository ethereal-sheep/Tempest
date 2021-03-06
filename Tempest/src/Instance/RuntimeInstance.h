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
#include "Physics/Physics.h"

struct Unit_MovementTrailEmitter_3D;

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

		}

		void _init() override;
		void _update(float dt) override;
		void _render() override;
		void _exit() override;

		
	private:
		// Not suppose to be here.... @Jun Hao
		std::weak_ptr<Unit_MovementTrailEmitter_3D> m_Unit_MovementTrailEmitter_3D;

	public:
		std::vector<Entity> sequences;
		tmap<int, tmap<int, id_t>> collision_map;
		tmap<int, tmap<int, id_t>> character_map;
		tmap<int, tmap<int, tmap<int, tmap<int, id_t>>>> wall_map;
		tmap<int, tmap<int, tmap<int, tmap<int, id_t>>>> door_map;
	};
}

