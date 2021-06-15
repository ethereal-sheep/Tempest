#include "RuntimeInstance.h"
#include "ECS/Components/Components.h"

namespace Tempest
{
	void RuntimeInstance::build_scripts(const tpath& root_directory)
	{
		// for every file in graphs folder
		tpath folder = root_directory / graphs_folder;

		// if directory doesn't exist, warn and return
		if (!std::filesystem::exists(folder))
		{
			// no exception as its not malformed
			LOG_WARN("{} does not exist!", folder.string());
			return;
		}
		
		//for (auto id : ecs.view<tc::Script>())
		{
			//auto& s = ecs.get<tc::Script>(id);
			// build scripts here

		}
	}
}

