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
		RuntimeInstance(const tpath& project_path, MemoryStrategy strategy = {}) :
			Instance(project_path, strategy), srm{memory_object.get()}
		{
			RunPhysxUpdate();
			build_scripts(root);
		}

		void _init() override
		{
			
		}
		void _update([[maybe_unused]] float dt) override
		{
			po.advance(dt);

			// we can do someother shit here

			po.fetch();
			auto view = ecs.view<Components::Rigidbody, tc::Transform>();
			for (auto id : view)
			{
				auto& rb = ecs.get<Components::Rigidbody>(id);
				//auto& position = ecs.get<Components::Transform>(id).position;
				auto dynamicRb = static_cast<physx::PxRigidBody*>(rb.internal_rb.get());
				
				LOG("{0} Current Velocity [{1}, {2}, {3}]",id, dynamicRb->getLinearVelocity().x, dynamicRb->getLinearVelocity().y, dynamicRb->getLinearVelocity().z);
				LOG("{0} Current Position [{1}, {2}, {3}]",id, dynamicRb->getGlobalPose().p.x, dynamicRb->getGlobalPose().p.y, dynamicRb->getGlobalPose().p.z);
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
		void RunPhysxUpdate();
	public:
		SRM srm;
	};
}

