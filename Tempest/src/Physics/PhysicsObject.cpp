 #include "PhysicsObject.h"

/* might want to clean this up*/

#include "Util/thread_pool.h"
#include "Util/Service.h"
#include "Util/px_err_callback.h"
#include "Util/px_deleter.h"
#include "Util/px_make.h"
#include "Util/px_math.h"


namespace Tempest
{
	using namespace physx;

	PhysicsObject::PhysicsObject(m_resource* mem_res)
		: allocator{ mem_res }, pcd{ Service<thread_pool>::Get() }
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
			transport = px_make(physx::PxDefaultPvdSocketTransportCreate(pvd_host_ip, port, timeout));
			if (!transport)
				LOG_CRITICAL("PxPvdTransport failed!");
			pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
		}

		// init physics
		{
			physx::PxTolerancesScale scale;
			scale.length = 100;        // typical length of an object
			scale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice

			bool recordMemoryAllocations = true;
			physics = px_make(PxCreatePhysics(PX_PHYSICS_VERSION, *foundation,
				scale, recordMemoryAllocations));
			if (!physics)
				LOG_CRITICAL("PxCreatePhysics failed!");
		}

		// init cooking
		{
			cooking = px_make(PxCreateCooking(PX_PHYSICS_VERSION, *foundation, physx::PxCookingParams(physics->getTolerancesScale())));
			if (!cooking)
				LOG_CRITICAL("PxCreateCooking failed!");
		}

		// init scene
		{
			physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
			sceneDesc.cpuDispatcher = &pcd;
			sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
			sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

			scene = px_make(physics->createScene(sceneDesc));
			if (!scene)
				LOG_CRITICAL("createScene failed!");
		}
		// creates an aggregate with no collision, obviously
		auto agg = px_make(physics->createAggregate(128, false));
	}

	bool PhysicsObject::advance(float dt)
	{
		accumulator += dt;
		if (accumulator < step_size)
			return false;

		accumulator -= step_size;
		// this is threaded
		scene->simulate(step_size);
		// no writes to scene after this
		return true;
	}

	bool PhysicsObject::fetch()
	{
		return scene->fetchResults(true);
	}

	tsptr<sample_rigidbody> PhysicsObject::createRigidbody(rigidbody_config rb_config, shape shape_data, vec3 pos) const
	{
		tsptr<sample_rigidbody> rb;
		rb->rb_config = rb_config;

		PxShape* newShape = nullptr;
		PxMaterial* material = physics->createMaterial(rb_config.material.x, rb_config.material.y, rb_config.material.z);

		switch (shape_data.type)
		{
		case SHAPE_TYPE::SPHERE:
			newShape = physics->createShape(physx::PxSphereGeometry(dynamic_cast<sphere*>(&shape_data)->radius), *material);
			break;
		case SHAPE_TYPE::BOX:	
		{
			box* pxBox = dynamic_cast<box*>(&shape_data);
			newShape = physics->createShape(physx::PxBoxGeometry(pxBox->halfX, pxBox->halfY, pxBox->halfZ), *material);
		}
			break;
		case SHAPE_TYPE::CAPSULE:
		{
			capsule* cap = dynamic_cast<capsule*>(&shape_data);
			newShape = physics->createShape(physx::PxCapsuleGeometry(cap->radius, cap->halfHeight), *material);
		}
			break;

		case SHAPE_TYPE::NONE:
			LOG_ERROR("No shape data");
			break;
		};

		if (rb->rb_config.is_static)
			rb->internal_rb = px_make(physx::PxCreateStatic(*physics, PxTransform(PxVec3{ pos }), *newShape));
		else
		{
			tsptr<PxRigidBody> dynamicBody = px_make(physx::PxCreateDynamic(*physics, PxTransform(PxVec3{ pos }), *newShape, rb->rb_config.density));
			dynamicBody->setLinearDamping(rb_config.linear_damping);
			dynamicBody->setAngularDamping(rb_config.angular_damping);
			dynamicBody->setLinearVelocity(PxVec3{rb_config.linear_velocity });
			dynamicBody->setAngularVelocity(PxVec3{rb_config.angular_velocity });
			rb->internal_rb = dynamicBody;
		}
		
		LOG_ERROR("Rigidbody cannot be created");

		rb->internal_rb->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, rb->rb_config.gravity);
		
		return rb;
	}

}


