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
	class RuntimeInstance : public Instance
	{
	public:
		/**
		 * @brief Requires root directory
		 */
		RuntimeInstance(const tpath& root_directory, MemoryStrategy strategy = {}) :
			Instance(root_directory, strategy), srm{memory_object.get()}
		{
			build_scripts(root_directory);
		}

		void _init() override
		{


		}
		void _update([[maybe_unused]] float dt) override
		{

		}
		void _render() override
		{

		}
		void _exit() override
		{

		}

	private:
		void build_scripts(const tpath& root_directory);



	public:
		SRM srm;
	};
}
