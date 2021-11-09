/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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

		// for each prefab, send it to hell
		for (auto& [id, pf] : scene.get_map())
		{
			auto entity = ecs.create(pf);
		}

	}

	void collision_helper(id_t id, const tc::Transform& t, const tc::Shape& s, tmap<int, tmap<int, id_t>>& m)
	{
		const auto transform = &t;
		const auto shape = &s;


		const int& x = shape->x;
		const int& y = shape->y;

		int a_x = x, a_y = y, e_x = 0, e_y = 0;

		if (a_x % 2 != a_y % 2)
		{
			a_x = a_y = std::min(x, y);
			e_x = x - a_x;
			e_y = y - a_y;
		}

		vec3 min, max;

		min.x = -.5f - (a_x - 1) / 2.f;
		min.z = -.5f - (a_y - 1) / 2.f;
		min.y = 0;

		max.x = .5f + (a_x - 1) / 2.f + e_x;
		max.z = .5f + (a_y - 1) / 2.f + e_y;
		max.y = 0;

		auto rot = transform->rotation;
		min = rot * min;
		max = rot * max;

		if (max.x < min.x) std::swap(min.x, max.x);
		if (max.z < min.z) std::swap(min.z, max.z);

		min += transform->position;
		max += transform->position;

		for (int i = (int)std::floor(min.x); i < (int)std::floor(max.x); ++i)
			for (int j = (int)std::floor(min.z); j < (int)std::floor(max.z); ++j) {
				m[i][j] = id;
			}
	}

	void RuntimeInstance::_update(float dt)
	{
		collision_map.clear();
		character_map.clear();

		for (auto id : ecs.view<tc::Collision, tc::Transform, tc::Shape>())
		{
			auto& transform = ecs.get<tc::Transform>(id);
			auto& shape = ecs.get<tc::Shape>(id);

			collision_helper(id, transform, shape, collision_map);
		}

		for (auto id : ecs.view<tc::Unit, tc::Transform, tc::Shape>())
		{
			auto& transform = ecs.get<tc::Transform>(id);
			auto& shape = ecs.get<tc::Shape>(id);

			collision_helper(id, transform, shape, character_map);
		}



		//po.advance(dt);

		//// we can do someother shit here

		//po.fetch();
		//auto view = ecs.view<Components::Rigidbody, tc::Transform>();
		//for (auto id : view)
		//{
		//	auto& rb = ecs.get<Components::Rigidbody>(id);
		//	auto& transform = ecs.get<Components::Transform>(id);
		//	if (!rb.rb_config.is_static)
		//	{
		//		auto dynamicRb = static_cast<physx::PxRigidBody*>(rb.internal_rb.get());
		//		auto& rbConfig = rb.rb_config;

		//		rbConfig.linear_velocity = { dynamicRb->getLinearVelocity().x,  dynamicRb->getLinearVelocity().y, dynamicRb->getLinearVelocity().z };
		//		rbConfig.angular_velocity = { dynamicRb->getAngularVelocity().x,  dynamicRb->getAngularVelocity().y, dynamicRb->getAngularVelocity().z };
		//		rbConfig.linear_damping = dynamicRb->getLinearDamping();
		//		rbConfig.angular_damping = dynamicRb->getAngularDamping();
		//		
		//	}
		//	
		//	
		//	transform.position = { rb.internal_rb->getGlobalPose().p.x, rb.internal_rb->getGlobalPose().p.y, rb.internal_rb->getGlobalPose().p.z };
		//	transform.rotation = math_cast(rb.internal_rb->getGlobalPose().q);  //{ rb.internal_rb->getGlobalPose().q., rb.internal_rb->getGlobalPose().p.y, rb.internal_rb->getGlobalPose().p.z };
		//	/*LOG("{0} Current Velocity [{1}, {2}, {3}]", id, dynamicRb->getLinearVelocity().x, dynamicRb->getLinearVelocity().y, dynamicRb->getLinearVelocity().z);
		//	LOG("{0} Current Position [{1}, {2}, {3}]", id, dynamicRb->getGlobalPose().p.x, dynamicRb->getGlobalPose().p.y, dynamicRb->getGlobalPose().p.z);*/
		//}
	}
	void RuntimeInstance::_render()
	{
	}
	void RuntimeInstance::_exit()
	{
	}

}

