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

	struct PhysicsConfig
	{
		vec3 gravity = { 0.0f, -9.81f, 0.0f };
		bool pvd_enabled = false;
		// bunch of other shit
	};


	class PhysicsObject
	{
	public:
		PhysicsObject(m_resource* mem_res = aligned_malloc_resource()) :
			allocator{ mem_res }, pcd{ Service<thread_pool>::Get() }
		{
			// might want to change LOG_CRITICAL to exception
			// init foundation
			{
				foundation = px_make(PxCreateFoundation(PX_PHYSICS_VERSION, allocator, px_err_callback()));
				if (!foundation)
					LOG_CRITICAL("PxCreateFoundation failed!");
			}

			// init pvd (OPTIONAL)
			{
				pvd = px_make(PxCreatePvd(*foundation));
				if (!pvd)
					LOG_CRITICAL("PxCreatePvd failed!");
				transport = px_make(PxDefaultPvdSocketTransportCreate(pvd_host_ip, port, timeout));
				if (!transport)
					LOG_CRITICAL("PxPvdTransport failed!");
				pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
			}

			// init physics
			{
				bool recordMemoryAllocations = true;
				physics = px_make(PxCreatePhysics(PX_PHYSICS_VERSION, *foundation,
					PxTolerancesScale(), recordMemoryAllocations));
				if (!physics)
					LOG_CRITICAL("PxCreatePhysics failed!");
			}

			// init cooking
			{
				cooking = px_make(PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(physics->getTolerancesScale())));
				if (!cooking)
					LOG_CRITICAL("PxCreateCooking failed!");
			}

			// init scene
			{
				PxSceneDesc sceneDesc(physics->getTolerancesScale());
				sceneDesc.cpuDispatcher = &pcd;
				sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
				sceneDesc.filterShader = PxDefaultSimulationFilterShader;

				scene = px_make(physics->createScene(sceneDesc));
				if (!scene)
					LOG_CRITICAL("createScene failed!");
			}

		}
		// testing
		float accumulator = 0.0f;
		float step_size = 1.0f / 60.0f;
		bool advance(float dt)
		{
			accumulator += dt;
			if (accumulator < step_size)
				return false;

			accumulator -= step_size;

			scene->simulate(step_size);
		}


	private:
		px_allocator allocator;
		px_cpu_dispatcher pcd;

		tsptr<physx::PxFoundation> foundation;
		tsptr<physx::PxPvd> pvd;
		tsptr<physx::PxPvdTransport> transport;
		tsptr<physx::PxPhysics> physics;
		tsptr<physx::PxCooking> cooking;
		tsptr<physx::PxScene> scene;
	};

	void testing_physics_2()
	{
		debug_mr dg("testing_physics_2", aligned_malloc_resource());
		dg.set_verbose(true);

		PhysicsObject po(&dg);

	}
	void testing_physics_3()
	{
		// fking works
		physx::PxMat44 a(1.f);
		glm::mat4& b = *reinterpret_cast<glm::mat4*>(&a);
		physx::PxQuat c(els::pi<float>, PxVec3{0,0,1});
		glm::quat& d = *reinterpret_cast<glm::quat*>(&c);
	}

	void testing_physics()
	{
		testing_physics_1();
		testing_physics_2();
		testing_physics_3();
	}
}