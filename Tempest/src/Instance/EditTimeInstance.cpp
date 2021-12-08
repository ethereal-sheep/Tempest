/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "EditTimeInstance.h"
#include "Graphics/Basics/RenderSystem.h"
#include "ECS/Components/Components.h"
#include <Physics/Util/px_math.h>

namespace Tempest
{
	void EditTimeInstance::_init()
	{
		auto view = ecs.view<Components::Rigidbody, tc::Transform>(exclude_t<tc::Destroyed>());


		auto sl = ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
		if (sl.size_hint() == 0)
		{
			auto StatsLine = ecs.create();
			auto meta = ecs.emplace<tc::Meta>(StatsLine);
			meta->name = "StatsData";
			ecs.emplace<tc::Statline>(StatsLine);
		}
		

		/*for (auto id : view)
		{
			auto& rb = ecs.get<Components::Rigidbody>(id);
			auto& transform = ecs.get<Components::Transform>(id);
			rigidbody_config staticBody;
			staticBody.is_static = true;
			rb.internal_rb = po.create_actor(staticBody, rb.shape_data, transform.position, transform.rotation, id);
			po.AddActorToScene(rb.internal_rb.get());

		}*/
	}
	void EditTimeInstance::_update(float)
	{
		scene.get_map().update();

		for (auto& [id, pf] : scene.get_map())
		{
			
			if (auto model = pf.get_if<tc::Model>())
			{
				auto transform = pf.get_if<tc::Transform>();
				auto local = pf.get_if<tc::Local>();

				auto test = glm::translate(transform->position)
					* glm::mat4(transform->rotation)
					* glm::translate(local->local_position)
					* glm::mat4(local->local_rotation)
					* glm::scale(local->local_scale)
					* glm::scale(transform->scale);

				std::filesystem::path p{ model->path };
				if (strcmp(p.extension().string().c_str(), ".a"))
				{
					p.replace_extension(".a");
				}
				Service<RenderSystem>::Get().SubmitModel(p.string(), test);
			}
		}

		// destroy all deleted stuff in ecs
		auto view = ecs.view<Components::Destroyed>();
		tvector<Entity> entities{ view.begin(), view.end() };
		std::for_each(entities.begin(), entities.end(), std::bind(&ECS::destroy, &ecs, std::placeholders::_1));
	}
	void EditTimeInstance::_render()
	{

	}
	void EditTimeInstance::_exit()
	{
		/*auto view = ecs.view<Components::Rigidbody, tc::Transform>();
		for (auto id : view)
		{
			auto& rb = ecs.get<Components::Rigidbody>(id);
			po.RemoveActorFromScene(rb.internal_rb.get());
			rb.internal_rb = nullptr;

		}*/
	}
}