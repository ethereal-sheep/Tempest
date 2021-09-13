#include "RuntimeInstance.h"
#include "ECS/Components/Components.h"

namespace Tempest
{
	void RuntimeInstance::_init()
	{

		auto view = ecs.view<Components::Rigidbody, tc::Transform>();
		for (auto id : view)
		{
			auto& rb = ecs.get<Components::Rigidbody>(id);
			auto& transform = ecs.get<Components::Transform>(id);

			rb.internal_rb = po.create_actor(rb.rb_config, rb.shape_data, transform.position, transform.rotation, id);
			po.AddActorToScene(rb.internal_rb.get());
			//Edit Time is setting the physx properties
			//auto& position = ecs.get<Components::Transform>(id).position;
			
			//if (!rb.rb_config.is_static)
			//{
			//	auto dynamicRb = static_cast<physx::PxRigidBody*>(rb.internal_rb.get());
			//	//Setting up Dynamic RigidBody
			//	dynamicRb->setLinearVelocity({ rb.rb_config.linear_velocity.x,rb.rb_config.linear_velocity.y, rb.rb_config.linear_velocity.z });
			//	//rb.rb_config.linear_velocity = { dynamicRb->getLinearVelocity().x,  dynamicRb->getLinearVelocity().y, dynamicRb->getLinearVelocity().z };
			//	dynamicRb->setAngularVelocity({ rb.rb_config.angular_velocity.x,rb.rb_config.angular_velocity.y, rb.rb_config.angular_velocity.z });
			//	dynamicRb->setLinearDamping(rb.rb_config.linear_damping);
			//	dynamicRb->setAngularDamping(rb.rb_config.angular_damping);
			//	dynamicRb->setMass(rb.rb_config.mass);
			//	dynamicRb->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !rb.rb_config.gravity);
			//	physx::PxRigidBodyExt::updateMassAndInertia(*dynamicRb, rb.rb_config.density);

			//	
			//	LOG("{0} Current Velocity [{1}, {2}, {3}]", id, dynamicRb->getLinearVelocity().x, dynamicRb->getLinearVelocity().y, dynamicRb->getLinearVelocity().z);
			//	LOG("{0} Current Position [{1}, {2}, {3}]", id, dynamicRb->getGlobalPose().p.x, dynamicRb->getGlobalPose().p.y, dynamicRb->getGlobalPose().p.z);
			//}
			
		}
	}
	void RuntimeInstance::_update(float dt)
	{
		po.advance(dt);

		// we can do someother shit here

		po.fetch();
		auto view = ecs.view<Components::Rigidbody, tc::Transform>();
		for (auto id : view)
		{
			auto& rb = ecs.get<Components::Rigidbody>(id);
			auto& transform = ecs.get<Components::Transform>(id);
			if (!rb.rb_config.is_static)
			{
				auto dynamicRb = static_cast<physx::PxRigidBody*>(rb.internal_rb.get());
				auto& rbConfig = rb.rb_config;

				rbConfig.linear_velocity = { dynamicRb->getLinearVelocity().x,  dynamicRb->getLinearVelocity().y, dynamicRb->getLinearVelocity().z };
				rbConfig.angular_velocity = { dynamicRb->getAngularVelocity().x,  dynamicRb->getAngularVelocity().y, dynamicRb->getAngularVelocity().z };
				rbConfig.linear_damping = dynamicRb->getLinearDamping();
				rbConfig.angular_damping = dynamicRb->getAngularDamping();
				
			}
			
			
			transform.position = { rb.internal_rb->getGlobalPose().p.x, rb.internal_rb->getGlobalPose().p.y, rb.internal_rb->getGlobalPose().p.z };
			transform.rotation = math_cast(rb.internal_rb->getGlobalPose().q);  //{ rb.internal_rb->getGlobalPose().q., rb.internal_rb->getGlobalPose().p.y, rb.internal_rb->getGlobalPose().p.z };
			/*LOG("{0} Current Velocity [{1}, {2}, {3}]", id, dynamicRb->getLinearVelocity().x, dynamicRb->getLinearVelocity().y, dynamicRb->getLinearVelocity().z);
			LOG("{0} Current Position [{1}, {2}, {3}]", id, dynamicRb->getGlobalPose().p.x, dynamicRb->getGlobalPose().p.y, dynamicRb->getGlobalPose().p.z);*/
		}
	}
	void RuntimeInstance::_render()
	{
	}
	void RuntimeInstance::_exit()
	{
	}
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

