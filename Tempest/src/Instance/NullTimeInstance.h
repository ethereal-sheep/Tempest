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

