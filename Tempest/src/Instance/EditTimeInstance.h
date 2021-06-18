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

		void _init() override
		{
			// init windows here
		}
		void _update([[maybe_unused]] float dt) override
		{
			// update windows here
		}
		void _render() override
		{
			// render extra stuff here
			// windows are auto rendered here
		}
		void _exit() override
		{

		}

	private:


	public:
		// WindowManager here
	};
}

