#pragma once
#include "RuntimeInstance.h"
#include "EditTimeInstance.h"

namespace Tempest
{
	enum struct InstanceType
	{
		EDIT_TIME,
		RUN_TIME,
	};

	class InstanceConfig
	{
		MemoryStrategy memory_strategy;
		InstanceType instance_type = InstanceType::EDIT_TIME;
	};

	class InstanceManager
	{
		void update()
		{
			if (!instance) return;
		}
		void render()
		{
			if (!instance) return;
		}

		void load_new_instance([[maybe_unused]]const tpath& project_path, [[maybe_unused]] InstanceConfig config)
		{
		}

	private:
		void init_runtime()
		{
			// register windows here
			// assume instance is valid now

			// 
		}
		void init_edittime()
		{
			// register windows here
			// assume instance is valid now
		}


		tuptr<Instance> instance;
	};
}