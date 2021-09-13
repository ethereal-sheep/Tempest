#include "Instance.h"
#include "Graphics/Basics/RenderSystem.h"

namespace Tempest
{
	void Instance::internal_init()
	{
		window_manager.init(*this);

		// clean up destroyed
		auto view = ecs.view<Components::Destroyed>();
		tvector<Entity> entities{ view.begin(), view.end() };
		std::for_each(entities.begin(), entities.end(), std::bind(&ECS::destroy, &ecs, std::placeholders::_1));
	}
	void Instance::internal_update()
	{

	}
	void Instance::internal_render()
	{
		window_manager.show(*this);

		// move this to instance call when test finish
		auto view = ecs.view<tc::Mesh>(exclude_t<tc::Destroyed>());
		for (auto id : view)
		{
			auto& mesh = ecs.get<tc::Mesh>(id);
			auto& transform = ecs.get<tc::Transform>(id);
			Service<RenderSystem>::Get().Submit(mesh.code, transform);
		}

	}
	void Instance::internal_exit()
	{
		window_manager.exit(*this);
	}
}