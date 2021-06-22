#include "test_physics.h"
#include "../Physics.h"
#include "../PhysicsObject.h"
#include "Memory.h"
#include "TMath.h"
#include "ECS/ECS.h"
#include "Instance/RuntimeInstance.h"

namespace Tempest
{
	using namespace physx;

	void testing_physics_1()
	{
		debug_mr dg("testing_physics_1", aligned_malloc_resource());
		px_allocator alloc(&dg);
		auto mFoundation = px_make(PxCreateFoundation(PX_PHYSICS_VERSION, alloc, px_err_callback()));

		if (!mFoundation)
			LOG_CRITICAL("PxCreateFoundation failed!");

		bool recordMemoryAllocations = true;

		auto mPvd = px_make(PxCreatePvd(*mFoundation));
		if (!mPvd)
			LOG_CRITICAL("PxCreatePvd failed!");
		auto transport = px_make(PxDefaultPvdSocketTransportCreate(pvd_host_ip, port, timeout));
		mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

		auto mPhysics = px_make(PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
			PxTolerancesScale(), recordMemoryAllocations));
		if (!mPhysics)
			LOG_CRITICAL("PxCreatePhysics failed!");

		auto mCooking = px_make(PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams(mPhysics->getTolerancesScale())));
		if (!mCooking)
			LOG_CRITICAL("PxCreateCooking failed!");

		thread_pool& tp = Service<thread_pool>::Get();
		px_cpu_dispatcher pcd(tp);

		PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
		sceneDesc.cpuDispatcher = &pcd;
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;

		auto mScene = px_make(mPhysics->createScene(sceneDesc));
		if (!mScene)
			LOG_CRITICAL("createScene failed!");
	}


	void testing_physics_2()
	{
		debug_mr dg("testing_physics_2", aligned_malloc_resource());
		dg.set_verbose(true);

		PhysicsObject po(&dg);

	}
	void testing_physics_3()
	{
		// fking works
		{
			/*
			physx::PxMat44 a(1.f);
			glm::mat4& b = *reinterpret_cast<glm::mat4*>(&a);
			physx::PxQuat c(els::pi<>, PxVec3{ 0, 0, 1 });
			glm::quat& d = *reinterpret_cast<glm::quat*>(&c);
			*/

		}

		//actual tests
		auto test = [](auto& a)
		{
			auto& b = math_cast(a);
			auto& c = math_cast(b);
			LOG_ASSERT(a == c);
		};
		auto test_const = [](const auto& a)
		{
			auto& b = math_cast(a);
			auto& c = math_cast(b);
			LOG_ASSERT(a == c);
		};

		{
			physx::PxVec3 a(els::random::uniform_rand(0.f, 1.f));
			test(a);
			test_const(a);
		}
		{
			physx::PxQuat a(els::random::uniform_rand(0.f, 1.f));
			test(a);
			test_const(a);
		}
		{
			physx::PxMat44 a(els::random::uniform_rand(0.f, 1.f));
			test(a);
			test_const(a);
		}
		{
			vec3 a(els::random::uniform_rand(0.f, 1.f));
			test(a);
			test_const(a);
		}
		{

			quat a{
				els::random::uniform_rand(0.f, 1.f),
				els::random::uniform_rand(0.f, 1.f),
				els::random::uniform_rand(0.f, 1.f),
				els::random::uniform_rand(0.f, 1.f) };
			test(a);
			test_const(a);
		}
		{
			mat4 a(els::random::uniform_rand(0.f, 1.f));
			test(a);
			test_const(a);
		}

	}
	void testing_physics_4()
	{
		auto test = [](auto& a, auto& b)
		{
			auto& c = math_cast(a);
			LOG_ASSERT(b == c);
		};
		auto test_const = [](const auto& a, const auto& b)
		{
			auto& c = math_cast(a);
			LOG_ASSERT(b == c);
		};

		{
			auto rand = els::random::uniform_rand(0.f, 1.f);
			physx::PxVec3 a(rand);
			vec3 b(rand);
			test(a, b);
			test_const(a, b);
			test(b, a);
			test_const(b, a);
		}
		{
			auto r1 = els::random::uniform_rand(0.f, 1.f);
			auto r2 = els::random::uniform_rand(0.f, 1.f);
			auto r3 = els::random::uniform_rand(0.f, 1.f);
			auto r4 = els::random::uniform_rand(0.f, 1.f);
			physx::PxQuat a(r1,r2,r3,r4);
			quat b(r1, r2, r3, r4);

			test(a, b);
			test_const(a, b);
			test(b, a);
			test_const(b, a);
		}
		{
			auto rand = els::random::uniform_rand(0.f, 1.f);
			physx::PxMat44 a(rand);
			mat4 b(rand);
			test(a, b);
			test_const(a, b);
			test(b, a);
			test_const(b, a);
		}
		
	}

	void testing_physics_5() // Serialization Test
	{
		//edit
		{
			debug_mr dg("testing_ecs_2.1");
			dg.set_strict(true);

			ECS ecs(&dg);

			auto t = 100;
			for (auto i = 0; i < t; ++i)
			{
				auto entity = ecs.create();
				auto* rb = ecs.emplace<Components::Rigidbody>(entity);
				
				if(entity % 3 == 1)
					rb->shape_data = shape(SHAPE_TYPE::SPHERE, 1);
				if (entity % 3 == 2)
					rb->shape_data = shape(SHAPE_TYPE::BOX, 1,2,3 );
				if (entity % 3 == 0)
					rb->shape_data = shape(SHAPE_TYPE::CAPSULE, 4,5 );
				
				
			}

			ecs.save("C:\\Users\\h_ron\\source\\repos\\Tempest\\Build");
		}

		// load
		{
			debug_mr dg("testing_ecs_2.2");
			dg.set_strict(true);

			ECS ecs(&dg);

			ecs.load("C:\\Users\\h_ron\\source\\repos\\Tempest\\Build");

			auto view = ecs.view<Components::Rigidbody>();
			LOG_ASSERT(view.size_hint() == 100);

			for (auto id : view)
			{
				auto rb = ecs.get<Components::Rigidbody>(id);
				
				if (id % 3 == 1)
					LOG_ASSERT(ecs.get<Components::Rigidbody>(id).shape_data.type == SHAPE_TYPE::SPHERE);
				if (id % 3 == 2)
					LOG_ASSERT(ecs.get<Components::Rigidbody>(id).shape_data.type == SHAPE_TYPE::BOX);
				if (id % 3 == 0)
					LOG_ASSERT(ecs.get<Components::Rigidbody>(id).shape_data.type == SHAPE_TYPE::CAPSULE);
			
			}

		}
	}

	void testing_physics_6() // creation of rigidbody and adding to scene
	{
		//edit, Saving
		
		{
			debug_mr dg("testing_ecs_2.1");
			dg.set_strict(true);
			ECS ecs(&dg);
			auto t = 1;
			for (auto i = 0; i < t; ++i)
			{
				auto entity = ecs.create();
				auto* rb = ecs.emplace<Components::Rigidbody>(entity);
				
				rb->shape_data = shape(SHAPE_TYPE::SPHERE, 1);
				auto* transform = ecs.emplace<Components::Transform>(entity);
				transform->position = { 0,1,0 };
			}
			ecs.save("C:\\Users\\h_ron\\source\\repos\\Tempest\\Build");
		}

		{
			debug_mr dg("testing_ecs_2.2", aligned_malloc_resource());
			dg.set_strict(true);
			PhysicsObject po = Service<PhysicsObject>::Get();
			ECS ecs(&dg);

			ecs.load("C:\\Users\\h_ron\\source\\repos\\Tempest\\Build");

			auto view = ecs.view<Components::Rigidbody>();
			LOG_ASSERT(view.size_hint() == 1);

			
		//	PxShape* shapes = nullptr;
			for (auto id : view)
			{
				auto rb = ecs.get<Components::Rigidbody>(id);
				auto position = ecs.get<Components::Transform>(id).position;
				vec3 testpos{ 0,1,0 };

				LOG_ASSERT(rb.shape_data.type == SHAPE_TYPE::SPHERE);
				LOG_ASSERT(position == testpos);
				rb.internal_rb = po.createRigidbody(rb.rb_config, rb.shape_data, position);
				po.AddActorToScene(rb.internal_rb.get());
				
				//need to check if rigidbody is dynamic then cast
				if (!rb.rb_config.is_static)
				{
					// the casting to body oso gives errors
					auto dynamicRb = static_cast<PxRigidBody*>(rb.internal_rb.get()); //try this?
					dynamicRb->setLinearVelocity({ 1,0,0 });
					LOG_TRACE("Current Velocity [{0}, {1}, {2}]", dynamicRb->getLinearVelocity().x, dynamicRb->getLinearVelocity().y, dynamicRb->getLinearVelocity().z);

					LOG_ASSERT(dynamicRb->getLinearVelocity() == physx::PxVec3( 1, 0, 0 ));
				}

				
				//rb.internal_rb->getShapes(&shapes, 1);
				//shapes->release();
			}
		}
	}

	void testing_physics_7_1() // initing of physics object
	{ 
		RuntimeInstance runtime("C:\\Users\\h_ron\\source\\repos\\Tempest\\Build");
		for(int i = 0; i < 10; i++)
		{
			runtime._update(1.f);
		}
	}

	void testing_physics_Saving() // Saving object

	{
		{
			debug_mr dg("testing_ecs_2.1");
			dg.set_strict(true);
			ECS ecs(&dg);
			auto t = 1;
			for (auto i = 0; i < t; ++i)
			{
				auto entity = ecs.create();
				auto* rb = ecs.emplace<Components::Rigidbody>(entity);

				rb->shape_data = shape(SHAPE_TYPE::SPHERE, 1);
				auto* transform = ecs.emplace<Components::Transform>(entity);
				transform->position = { 0,0,0 };
			}
			ecs.save("C:\\Users\\h_ron\\source\\repos\\Tempest\\Build");
		}
	}

	void testing_physics()
	{
		/*testing_physics_1();
		testing_physics_2();
		testing_physics_3();
		testing_physics_4();*/
		//testing_physics_5(); // Serialization Test
		//testing_physics_6();
		testing_physics_Saving();
		testing_physics_7_1();
	}
}