#include "test_physics.h"
#include "../Physics.h"
#include "Memory.h"

namespace Tempest
{
	using namespace physx;

	void testing_physics_1()
	{
		debug_mr dg("DEBUG PHYSICS", aligned_malloc_resource());
		dg.set_strict(true);
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

		thread_pool tp;
		px_cpu_dispatcher pcd(tp);

		PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
		sceneDesc.cpuDispatcher = &pcd;
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;

		auto mScene = px_make(mPhysics->createScene(sceneDesc));
		if (!mScene)
			LOG_CRITICAL("createScene failed!");
	}


	void testing_physics()
	{
		testing_physics_1();
	}
}