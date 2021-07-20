#include "Instance.h"
#include "Graphics/Basics/RenderSystem.h"

namespace Tempest
{
	void Instance::internal_init()
	{
		window_manager.init();
	}
	void Instance::internal_update()
	{

	}
	void Instance::internal_render()
	{
		window_manager.show(*this);

		// move this to instance call when test finish
		auto view = ecs.view<tc::Mesh>();
		for (auto id : view)
		{
			auto& mesh = ecs.get<tc::Mesh>(id);
			auto& transform = ecs.get<tc::Transform>(id);
			Service<RenderSystem>::Get().Submit(mesh.code, transform);
		}

	}
	void Instance::internal_exit()
	{
		window_manager.exit();
	}
}