/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu),
				Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "PhysicsObject.h"

/* might want to clean this up*/

#include "Util/thread_pool.h"
#include "Util/Service.h"
#include "Util/px_err_callback.h"
#include "Util/px_deleter.h"
#include "Util/px_make.h"
#include "Util/px_math.h"

#include <basetsd.h>

// TESTING
physx::PxRigidStatic* ground_plane = nullptr;

namespace Tempest
{
	using namespace physx;

	PxFilterFlags contactReportFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
		PxFilterObjectAttributes attributes1, PxFilterData filterData1,
		PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
	{
		PX_UNUSED(attributes0);
		PX_UNUSED(attributes1);
		PX_UNUSED(filterData0);
		PX_UNUSED(filterData1);
		PX_UNUSED(constantBlockSize);
		PX_UNUSED(constantBlock);

		// all initial and persisting reports for everything, with per-point data
		pairFlags = PxPairFlag::eSOLVE_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT
			| PxPairFlag::eNOTIFY_TOUCH_FOUND
			| PxPairFlag::eNOTIFY_TOUCH_PERSISTS
			| PxPairFlag::eNOTIFY_CONTACT_POINTS;
		return  PxFilterFlag::eDEFAULT;
	}

	PhysicsObject::PhysicsObject(m_resource* mem_res)
		: allocator{ mem_res }, pcd{ Service<thread_pool>::Get() }
	{
		//// init foundation
		//{
		//	//foundation = px_make(PxCreateFoundation(PX_PHYSICS_VERSION, allocator, px_err_callback()));
		//	foundation = px_make(PxCreateFoundation(PX_PHYSICS_VERSION, allocator, px_err_callback()));
		//	LOG_ASSERT_V(foundation, "PxCreateFoundation failed!");
		//}

		//// init pvd (OPTIONAL)
		//{
		//	pvd = px_make(PxCreatePvd(*foundation));
		//	if (!pvd)
		//		LOG_CRITICAL("PxCreatePvd failed!");
		//	transport = px_make(physx::PxDefaultPvdSocketTransportCreate(pvd_host_ip, port, timeout));
		//	LOG_ASSERT_V(transport, "PxPvdTransport failed!");
		//	pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
		//}

		//// init physics (SEE ME FOR GRAVITY SHIT)
		//{
		//	physx::PxTolerancesScale scale;


		//	bool recordMemoryAllocations = true;
		//	physics = px_make(PxCreatePhysics(PX_PHYSICS_VERSION, *foundation,
		//		scale, recordMemoryAllocations));
		//	LOG_ASSERT_V(physics, "PxCreatePhysics failed!");
		//}

		//// init cooking
		//{
		//	cooking = px_make(PxCreateCooking(PX_PHYSICS_VERSION, *foundation, physx::PxCookingParams(physics->getTolerancesScale())));
		//	LOG_ASSERT_V(cooking, "PxCreateCooking failed!");
		//}

		//// init scene
		//{
		//	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
		//	sceneDesc.cpuDispatcher = &pcd;
		//	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.f);
		//	//sceneDesc.filterShader = contactReportFilterShader;
		//	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		//	//sceneDesc.simulationEventCallback = &gContactReportCallback;

		//	scene = px_make(physics->createScene(sceneDesc));
		//	LOG_ASSERT_V(scene, "createScene failed!");
		//}

		//// create ground plane
		//{
		//	PxMaterial* material = physics->createMaterial(0.5f, 0.5f, 0.5f);
		//	ground_plane = PxCreatePlane(*physics, PxPlane(0,1,0,0), *material);
		//	scene->addActor(*ground_plane);
		//}

		//// creates an aggregate with no collision, obviously
		////auto agg = px_make(physics->createAggregate(128, false));

	}

	bool PhysicsObject::advance(float )
	{
		//accumulator += dt;
		//if (accumulator < step_size)
		//	return false;

		//accumulator -= step_size;
		////	LOG("DT: {0}", accumulator);
		//	// this is threaded
		//scene->simulate(step_size);


		// no writes to scene after this
		return true;
	}

	bool PhysicsObject::fetch()
	{
		//gContactPositions.clear();

		bool fetchResult = scene->fetchResults(true);

		//LOG(">>> {0} contact report ", physx::PxU32(gContactPositions.size()));

		return fetchResult;
	}

	tsptr<physx::PxRigidActor> PhysicsObject::createRigidbody(rigidbody_config rb_config, shape shape_data, vec3 pos, quat rot) const
	{

		PxShape* newShape = nullptr;
		PxMaterial* material = physics->createMaterial(rb_config.material.x, rb_config.material.y, rb_config.material.z);
		LOG("CREATE MATERIAL WITH {0}, {1}, {2}", rb_config.material.x, rb_config.material.y, rb_config.material.z);
		tsptr<physx::PxRigidActor> actor = nullptr;
		switch (shape_data.type)
		{
		case SHAPE_TYPE::SPHERE:
			newShape = physics->createShape(physx::PxSphereGeometry(shape_data.shapeData.x), *material);
			break;
		case SHAPE_TYPE::CAPSULE:
		{
			newShape = physics->createShape(physx::PxCapsuleGeometry(shape_data.shapeData.x, shape_data.shapeData.y), *material);
		}
		break;
		case SHAPE_TYPE::BOX:
		{
			newShape = physics->createShape(physx::PxBoxGeometry(shape_data.shapeData.x, shape_data.shapeData.y, shape_data.shapeData.z), *material);
		}
		break;
		case SHAPE_TYPE::NONE:
			LOG_ERROR("No shape data");
			break;
		};

		LOG_ASSERT_V(newShape != nullptr, "cannot create shape");

		if (rb_config.is_static)
		{
			actor = px_make(physx::PxCreateStatic(*physics, PxTransform(math_cast( pos )), *newShape));
			actor->setGlobalPose(PxTransform(math_cast(pos), math_cast(rot)));
		}
		else
		{
			tsptr<PxRigidBody> dynamicBody;
			dynamicBody = px_make(physx::PxCreateDynamic(*physics, PxTransform(math_cast(pos)), *newShape, rb_config.density));
			dynamicBody->setGlobalPose(PxTransform(math_cast(pos), math_cast(rot)));
			dynamicBody->setLinearDamping(rb_config.linear_damping);
			dynamicBody->setAngularDamping(rb_config.angular_damping);
			dynamicBody->setLinearVelocity(math_cast( rb_config.linear_velocity ));
			dynamicBody->setAngularVelocity(math_cast( rb_config.angular_velocity ));
			dynamicBody->setMass(rb_config.mass);
			dynamicBody->attachShape(*newShape);
			physx::PxRigidBodyExt::updateMassAndInertia(*dynamicBody, rb_config.density);


			actor = dynamicBody;
		}

		LOG_ASSERT_V(actor != nullptr, "cannot create actor");

		actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !rb_config.gravity);

		newShape->release();

		return actor;
	}
	tsptr<physx::PxRigidActor> PhysicsObject::create_actor(rigidbody_config rb_config, shape shape_data, vec3 pos, quat rot, id_t id )
	{
		auto actor = createRigidbody(rb_config, shape_data, pos, rot);
		actor->setName((char*)UIntToPtr(id));
		return actor;
	}

	tpair<id_t, bool> PhysicsObject::raycast(vec3 , vec3 )
	{
		//PxRaycastBuffer result;
		//auto max_dist = 100.f;
		//if (scene->raycast(math_cast( origin ), math_cast( dir ), max_dist, result))
		//{
		//	auto* actor = result.getAnyHit(0).actor;
		//	if (actor != static_cast<PxRigidActor*>(ground_plane))
		//		return make_tpair(PtrToUint(actor->getName()), true);
		//}
		return make_tpair(0, false);
	}

	physx::PxShape* PhysicsObject::CreateActorShape(rigidbody_config rb_config, shape shape_data)
	{
		PxShape* newShape = nullptr;
		PxMaterial* material = physics->createMaterial(rb_config.material.x, rb_config.material.y, rb_config.material.z);
		LOG("CREATE MATERIAL WITH {0}, {1}, {2}", rb_config.material.x, rb_config.material.y, rb_config.material.z);
		
		switch (shape_data.type)
		{
		case SHAPE_TYPE::SPHERE:
			newShape = physics->createShape(physx::PxSphereGeometry(shape_data.shapeData.x), *material);
			break;
		case SHAPE_TYPE::CAPSULE:
		{
			newShape = physics->createShape(physx::PxCapsuleGeometry(shape_data.shapeData.x, shape_data.shapeData.y), *material);
		}
		break;
		case SHAPE_TYPE::BOX:
		{
			newShape = physics->createShape(physx::PxBoxGeometry(shape_data.shapeData.x, shape_data.shapeData.y, shape_data.shapeData.z), *material);
		}
		break;
		case SHAPE_TYPE::NONE:
			LOG_ERROR("No shape data");
			break;
		};

		return newShape;
	}
}
