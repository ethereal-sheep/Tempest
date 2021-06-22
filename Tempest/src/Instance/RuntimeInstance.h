#pragma once
#include "Instance.h"
#include "Scripting/SRM.h"
#include "Scripting/GMS.h"
#include "Physics/Physics.h"


namespace Tempest
{
	/**
	 * @brief Type of Instance that is loaded for runtime
	 * implementation requires a path to the root folder
	 * of the loading project/scene/map etc.
	 */
	class RuntimeInstance : public Instance
	{
	public:
		/**
		 * @brief Requires root directory
		 */
		RuntimeInstance(const tpath& root_directory, MemoryStrategy strategy = {}) :
			Instance(root_directory, strategy), srm{memory_object.get()}
		{
			build_scripts(root_directory);
			auto view = ecs.view<Components::Rigidbody, tc::Transform>();
			for (auto id : view)
			{
				auto& rb = ecs.get<Components::Rigidbody>(id);
				auto& position = ecs.get<Components::Transform>(id).position;
				rb.internal_rb = po.createRigidbody(rb.rb_config, rb.shape_data, position);
				po.AddActorToScene(rb.internal_rb.get());
				if (!rb.rb_config.is_static)
				{
					auto dynamicRb = static_cast<physx::PxRigidBody*>(rb.internal_rb.get()); //try this?
					dynamicRb->setLinearVelocity({ 1,0,0 });
					
					LOG("Current Velocity [{0}, {1}, {2}]", dynamicRb->getLinearVelocity().x, dynamicRb->getLinearVelocity().y, dynamicRb->getLinearVelocity().z);
					LOG("Current Position [{0}, {1}, {2}]", position.x, position.y, position.z);

					//LOG_ASSERT(dynamicRb->getLinearVelocity() == physx::PxVec3(1, 0, 0));
				}
			}
		}

		void _init() override
		{
		}
		void _update([[maybe_unused]] float dt) override
		{
			po.advance(dt);
			po.fetch();
			auto view = ecs.view<Components::Rigidbody, tc::Transform>();
			for (auto id : view)
			{
				auto& rb = ecs.get<Components::Rigidbody>(id);
				auto& position = ecs.get<Components::Transform>(id).position;
				auto dynamicRb = static_cast<physx::PxRigidBody*>(rb.internal_rb.get()); //try this?
			//	auto pos = dynamicRb->getGlobalPose().
				LOG("Current Velocity [{0}, {1}, {2}]", dynamicRb->getLinearVelocity().x, dynamicRb->getLinearVelocity().y, dynamicRb->getLinearVelocity().z);
				LOG("Current Position [{0}, {1}, {2}]", dynamicRb->getGlobalPose().p.x, dynamicRb->getGlobalPose().p.y, dynamicRb->getGlobalPose().p.z);
			}

		}
		void _render() override
		{
		}
		void _exit() override
		{

		}

		
	private:
		void build_scripts(const tpath& root_directory);



	public:
		SRM srm;
	};
}

