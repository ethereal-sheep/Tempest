#include "Instance.h"

namespace Tempest
{
	void Instance::internal_init()
	{
		po.init();
		window_manager.init();
	}
	void Instance::internal_update()
	{

	}
	void Instance::internal_render()
	{
		window_manager.show(*this);
	}
	void Instance::internal_exit()
	{
		window_manager.init();
	}
}