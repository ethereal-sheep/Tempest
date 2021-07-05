#include "RuntimeInstance.h"
#include "ECS/Components/Components.h"

namespace Tempest
{
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
	void RuntimeInstance::RunPhysxUpdate()
	{
		auto view = ecs.view<tc::Rigidbody, tc::Transform>();
		for (auto id : view)
		{
			auto& rb = ecs.get<Components::Rigidbody>(id);
			auto& position = ecs.get<Components::Transform>(id).position;
			rb.internal_rb = po.createRigidbody(rb.rb_config, rb.shape_data, position);
			po.AddActorToScene(rb.internal_rb.get());
			if (!rb.rb_config.is_static)
			{
				auto dynamicRb = static_cast<physx::PxRigidBody*>(rb.internal_rb.get());

				// both objects moving to each other
				if (id % 2 == 1)
					dynamicRb->setLinearVelocity({ 30.f,0,0 });
				else
					dynamicRb->setLinearVelocity({ -30.f,0,0 });

				LOG("{0} Starting Velocity [{1}, {2}, {3}]", id, dynamicRb->getLinearVelocity().x, dynamicRb->getLinearVelocity().y, dynamicRb->getLinearVelocity().z);
				LOG("{0} Starting Position [{1}, {2}, {3}]", id, position.x, position.y, position.z);

				//LOG_ASSERT(dynamicRb->getLinearVelocity() == physx::PxVec3(1, 0, 0));
			}
		}
	}
}

