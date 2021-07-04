#include "EditTimeInstance.h"
#include "Graphics/OpenGL/RenderSystem.h"

namespace Tempest
{
	void EditTimeInstance::_init()
	{
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
			
			//Edit Time is setting the physx properties
			//auto& position = ecs.get<Components::Transform>(id).position;
			if (!rb.rb_config.is_static)
			{
				auto dynamicRb = static_cast<physx::PxRigidBody*>(rb.internal_rb.get());
				//Setting up Dynamic RigidBody
				dynamicRb->setLinearVelocity({ rb.rb_config.linear_velocity.x,rb.rb_config.linear_velocity.y, rb.rb_config.linear_velocity.z });
				dynamicRb->setAngularVelocity({ rb.rb_config.angular_velocity.x,rb.rb_config.angular_velocity.y, rb.rb_config.angular_velocity.z });
				dynamicRb->setLinearDamping(rb.rb_config.linear_damping);
				dynamicRb->setAngularDamping(rb.rb_config.angular_damping);
				dynamicRb->setMass(rb.rb_config.mass);
				dynamicRb->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !rb.rb_config.gravity);
				physx::PxRigidBodyExt::updateMassAndInertia(*dynamicRb, rb.rb_config.density);

				transform.position = { dynamicRb->getGlobalPose().p.x, dynamicRb->getGlobalPose().p.y, dynamicRb->getGlobalPose().p.z };
				LOG("{0} Current Velocity [{1}, {2}, {3}]", id, dynamicRb->getLinearVelocity().x, dynamicRb->getLinearVelocity().y, dynamicRb->getLinearVelocity().z);
				LOG("{0} Current Position [{1}, {2}, {3}]", id, dynamicRb->getGlobalPose().p.x, dynamicRb->getGlobalPose().p.y, dynamicRb->getGlobalPose().p.z);
			}
			
		}
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