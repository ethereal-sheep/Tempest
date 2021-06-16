#include "test_physics.h"
#include "../Physics.h"


namespace Tempest
{
	using namespace physx;

	void testing_physics_1()
	{
		px_allocator alloc;
		auto mFoundation = px_make(PxCreateFoundation(PX_PHYSICS_VERSION, px_malloc_allocator(), px_err_callback()));

		if (!mFoundation)
			LOG_CRITICAL("PxCreateFoundation failed!");

		bool recordMemoryAllocations = true;

		auto mPvd = px_make(PxCreatePvd(*mFoundation));
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(pvd_host_ip, port, timeout);
		mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

		auto mPhysics = px_make(PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
			PxTolerancesScale(), recordMemoryAllocations, mPvd.get()));
		if (!mPhysics)
			LOG_CRITICAL("PxCreatePhysics failed!");

	}


	void testing_physics()
	{
		testing_physics_1();
	}
}