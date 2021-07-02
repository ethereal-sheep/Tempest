#include "EditTimeInstance.h"
#include "Graphics/OpenGL/RenderSystem.h"

namespace Tempest
{
	void EditTimeInstance::_init()
	{
	}
	void EditTimeInstance::_update(float)
	{
	}
	void EditTimeInstance::_render()
	{
		// move this to instance call when test finish
		auto view = ecs.view<tc::Mesh>();
		for (auto id : view)
		{
			auto& mesh = ecs.get<tc::Mesh>(id);
			auto& transform = ecs.get<tc::Transform>(id);
			Service<RenderSystem>::Get().Submit(mesh.shape, transform);
		}

	}
	void EditTimeInstance::_exit()
	{
	}
}