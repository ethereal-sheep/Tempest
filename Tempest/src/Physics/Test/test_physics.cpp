#include "test_physics.h"
#include "../Physics.h"
#include "Memory.h"
#include "TMath.h"

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

	void testing_physics_5()
	{

	}

	void testing_physics()
	{
		testing_physics_1();
		testing_physics_2();
		testing_physics_3();
		testing_physics_4();
	}
}