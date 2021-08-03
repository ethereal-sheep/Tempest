#pragma once

#include "Instance.h"

namespace Tempest 
{
	class PhysicsInstance : public Instance
	{
	public:
		/**
		 * @brief Requires root directory
		 */
		PhysicsInstance(const tpath& root_directory, MemoryStrategy strategy = {}) :
			Instance(root_directory, strategy)
		{
			// 
		}

		void _init() override;

		void _update([[maybe_unused]] float dt) override;

		void _render() override;

		void _exit() override;

		void save()
		{
			ecs.save(root);
			//graph_manager.save_all_to(root);
		}

		void save_as(const tpath&)
		{

		}
	};
};