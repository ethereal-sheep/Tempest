/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "PhysicsInstance.h"

namespace Tempest
{
	void PhysicsInstance::_init()
	{

		auto view = ecs.view<Components::Rigidbody, tc::Transform>(exclude_t<tc::Destroyed>());
		for (auto id : view)
		{
			auto& rb = ecs.get<Components::Rigidbody>(id);
			auto& transform = ecs.get<Components::Transform>(id);
			rigidbody_config staticBody;
			staticBody.is_static = true;
			rb.internal_rb = po.create_actor(staticBody, rb.shape_data, transform.position, transform.rotation, id);
			po.AddActorToScene(rb.internal_rb.get());

		}
	}
	void PhysicsInstance::_update(float dt)
	{

		po.advance(dt);

		// we can do someother shit here

		po.fetch();
		auto view = ecs.view<Components::Rigidbody, tc::Transform>(exclude_t<tc::Destroyed>());
		for (auto id : view)
		{
			auto& rb = ecs.get<Components::Rigidbody>(id);
			auto& transform = ecs.get<Components::Transform>(id);
			rb.internal_rb.get()->setGlobalPose({ transform.position.x, transform.position.y, transform.position.z });

		}
	}
	void PhysicsInstance::_render()
	{
	}
	void PhysicsInstance::_exit()
	{
		auto view = ecs.view<Components::Rigidbody, tc::Transform>();
		for (auto id : view)
		{
			auto& rb = ecs.get<Components::Rigidbody>(id);
			po.RemoveActorFromScene(rb.internal_rb.get());
			rb.internal_rb = nullptr;

		}
	}
}