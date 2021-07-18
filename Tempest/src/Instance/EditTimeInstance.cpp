#include "EditTimeInstance.h"
#include "Graphics/OpenGL/RenderSystem.h"
#include "ECS/Components/Components.h"
#include <Physics/Util/px_math.h>

namespace Tempest
{
	void EditTimeInstance::_init()
	{
		auto view = ecs.view<Components::Rigidbody, tc::Transform>();
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
	void EditTimeInstance::_update(float dt)
	{
		po.advance(dt);

		// we can do someother shit here

		po.fetch();
		auto view = ecs.view<Components::Rigidbody, tc::Transform>();
		for (auto id : view)
		{
			auto& rb = ecs.get<Components::Rigidbody>(id);
			auto& transform = ecs.get<Components::Transform>(id);
			rb.internal_rb.get()->setGlobalPose({ transform.position.x, transform.position.y, transform.position.z });
			/*if (!rb.rb_config.is_static)
			{
				auto dynamicRb = static_cast<physx::PxRigidBody*>(rb.internal_rb.get());
				auto& rbConfig = rb.rb_config;

				rbConfig.linear_velocity = { dynamicRb->getLinearVelocity().x,  dynamicRb->getLinearVelocity().y, dynamicRb->getLinearVelocity().z };
				rbConfig.angular_velocity = { dynamicRb->getAngularVelocity().x,  dynamicRb->getAngularVelocity().y, dynamicRb->getAngularVelocity().z };
				rbConfig.linear_damping = dynamicRb->getLinearDamping();
				rbConfig.angular_damping = dynamicRb->getAngularDamping();

			}


			transform.position = { rb.internal_rb->getGlobalPose().p.x, rb.internal_rb->getGlobalPose().p.y, rb.internal_rb->getGlobalPose().p.z };
			transform.rotation = math_cast(rb.internal_rb->getGlobalPose().q);*/
		}
		
	}
	void EditTimeInstance::_render()
	{

	}
	void EditTimeInstance::_exit()
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