#pragma once
#include "Instance.h"

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
		RuntimeInstance(const tpath& root_directory, MemoryStrategy strategy = {}) :
			Instance(root_directory, strategy) {}

		virtual void OnInit() override
		{

		}
		virtual void OnUpdate(float dt) override
		{

		}
		virtual void OnRender() override
		{

		}
		virtual void OnExit() override
		{

		}

	};
}

