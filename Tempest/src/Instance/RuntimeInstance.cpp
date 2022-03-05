/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "RuntimeInstance.h"
#include "ECS/Components/Components.h"
#include "Audio/AudioEngine.h"

#include "Particles/Particles_3D/ParticleSystem_3D.h"
#include "Particles/Particles_3D/UnitTrailEmitter_3D.h"

namespace Tempest
{
	void RuntimeInstance::_init()
	{

		//auto view = ecs.view<Components::Rigidbody, tc::Transform>();
		//for (auto id : view)
		//{
		//	auto& rb = ecs.get<Components::Rigidbody>(id);
		//	auto& transform = ecs.get<Components::Transform>(id);

		//	rb.internal_rb = po.create_actor(rb.rb_config, rb.shape_data, transform.position, transform.rotation, id);
		//	po.AddActorToScene(rb.internal_rb.get());
		//	//Edit Time is setting the physx properties
		//	//auto& position = ecs.get<Components::Transform>(id).position;
		//	
		//	//if (!rb.rb_config.is_static)
		//	//{
		//	//	auto dynamicRb = static_cast<physx::PxRigidBody*>(rb.internal_rb.get());
		//	//	//Setting up Dynamic RigidBody
		//	//	dynamicRb->setLinearVelocity({ rb.rb_config.linear_velocity.x,rb.rb_config.linear_velocity.y, rb.rb_config.linear_velocity.z });
		//	//	//rb.rb_config.linear_velocity = { dynamicRb->getLinearVelocity().x,  dynamicRb->getLinearVelocity().y, dynamicRb->getLinearVelocity().z };
		//	//	dynamicRb->setAngularVelocity({ rb.rb_config.angular_velocity.x,rb.rb_config.angular_velocity.y, rb.rb_config.angular_velocity.z });
		//	//	dynamicRb->setLinearDamping(rb.rb_config.linear_damping);
		//	//	dynamicRb->setAngularDamping(rb.rb_config.angular_damping);
		//	//	dynamicRb->setMass(rb.rb_config.mass);
		//	//	dynamicRb->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !rb.rb_config.gravity);
		//	//	physx::PxRigidBodyExt::updateMassAndInertia(*dynamicRb, rb.rb_config.density);

		//	//	
		//	//	LOG("{0} Current Velocity [{1}, {2}, {3}]", id, dynamicRb->getLinearVelocity().x, dynamicRb->getLinearVelocity().y, dynamicRb->getLinearVelocity().z);
		//	//	LOG("{0} Current Position [{1}, {2}, {3}]", id, dynamicRb->getGlobalPose().p.x, dynamicRb->getGlobalPose().p.y, dynamicRb->getGlobalPose().p.z);
		//	//}
		//	
		//}

		// for each prefab, send it to hell
		for (auto& [id, pf] : scene.get_map())
		{
			[[maybe_unused]]auto entity = ecs.create(pf);
			if (auto xform = ecs.get_if<tc::Transform>(entity))
			{
				xform->position += scene.get_map().map_pos;
			}
		}

	}

	void collision_helper(uint32_t level, const tc::Transform& t, const tc::Shape& s, tmap<int, tmap<int, id_t>>& m)
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

		min += transform->position;
		max += transform->position;

		if (max.x < min.x) std::swap(min.x, max.x);
		if (max.z < min.z) std::swap(min.z, max.z);


		for (int i = (int)std::round(min.x); i < (int)std::round(max.x); ++i)
			for (int j = (int)std::round(min.z); j < (int)std::round(max.z); ++j) {
				m[i][j] = level;
			}
	}

	void collision_helper(uint32_t level, const tc::Transform& t, const tc::Shape& shape, tmap<int, tmap<int, tmap<int, tmap<int, id_t>>>>& m)
	{
		vec3 s, e;

		e.x = .5f;
		e.z = .5f;
		e.y = .0f;

		s.x = shape.y ? -.5f : .5f;
		s.z = shape.x ? -.5f : .5f;
		s.y = .0f;

		auto rot = t.rotation;
		s = rot * s;
		e = rot * e;

		int a_x = (int)std::floor(t.position.x + s.x);
		int a_y = (int)std::floor(t.position.z + s.z);
		int b_x = (int)std::floor(t.position.x + e.x);
		int b_y = (int)std::floor(t.position.z + e.z);

		m[a_x][a_y][b_x][b_y] = level;
		m[b_x][b_y][a_x][a_y] = level;
	}

	void RuntimeInstance::_update(float dt)
	{
		collision_map.clear();
		character_map.clear();
		wall_map.clear();
		door_map.clear();


		auto view1 = ecs.view<tc::Unit, tc::Transform>(exclude_t<tc::Destroyed>());
		for (auto id : view1)
		{
			auto& unit = ecs.get<tc::Unit>(id);
			auto& transform = ecs.get<tc::Transform>(id);

			unit.update(dt);
			if (unit.is_moving())
			{
				transform = unit.get_current_transform();
			}

			if (unit.is_end_frame())
			{
				AudioEngine ae;
				ae.Play("Sounds2D/PlayerMovement.wav", "SFX");
				// Shift emitter and calls it to emit particle

				if (!m_unitTrailEmitter_3D.expired())
				{
					m_unitTrailEmitter_3D.lock()->m_GM.m_position = transform.position;
					m_unitTrailEmitter_3D.lock()->Emit(1);
				}
				else
				{
					m_unitTrailEmitter_3D = ParticleSystem_3D::GetInstance().CreateUnitTrailEmitter(transform.position);
				}

			}
		}


		// 1 is full
		// 2 is half

		for (auto id : ecs.view<tc::Collision, tc::Transform, tc::Shape>())
		{
			auto& transform = ecs.get<tc::Transform>(id);
			auto& shape = ecs.get<tc::Shape>(id);
			auto& collider = ecs.get<tc::Collision>(id);

			auto level = collider.cover == tc::Collision::Cover::FULL ? 1u : 2u;

			collision_helper(level, transform, shape, collision_map);
		}

		for (auto id : ecs.view<tc::Unit, tc::Transform, tc::Shape>())
		{
			auto& transform = ecs.get<tc::Transform>(id);
			auto& shape = ecs.get<tc::Shape>(id);

			collision_helper(id, transform, shape, character_map);
		}

		for (auto id : ecs.view<tc::Wall, tc::Transform, tc::Shape>())
		{
			auto& transform = ecs.get<tc::Transform>(id);
			auto& shape = ecs.get<tc::Shape>(id);
			auto& wall = ecs.get<tc::Wall>(id);

			auto level = wall.cover == tc::Wall::Cover::FULL ? 1u : 2u;

			collision_helper(level, transform, shape, wall_map);
		}

		for (auto id : ecs.view<tc::Door, tc::Transform, tc::Shape>())
		{
			auto& transform = ecs.get<tc::Transform>(id);
			auto& shape = ecs.get<tc::Shape>(id);
			auto& door = ecs.get<tc::Door>(id);

			if(door.get_current_state() == tc::Door::State::CLOSE)
				collision_helper(1u, transform, shape, wall_map);

			collision_helper(id, transform, shape, door_map);

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

