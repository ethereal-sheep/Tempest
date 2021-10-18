/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "TMath.h"
#include "Core.h"
#include "Memory.h"
#include "Util/px_include.h"
#include "Util/px_allocator.h"
#include "Util/px_malloc_allocator.h"
#include "Util/px_cpu_dispatcher.h"



namespace Tempest
{
	struct PhysicsConfig
	{
		vec3 gravity = { 0.0f, -9.81f, 0.0f };
		bool pvd_enabled = false;
		// bunch of other shit
	};

	enum struct SHAPE_TYPE
	{
		NONE,
		SPHERE,
		BOX,
		CAPSULE
	};

	struct shape
	{
		SHAPE_TYPE type = SHAPE_TYPE::BOX;
		vec3 shapeData = { 1.f, 1.f, 1.f };

		shape(SHAPE_TYPE shape_type = SHAPE_TYPE::NONE, float x = 0.f, float y = 0.f, float z = 0.f) : type{ shape_type }, shapeData{ x,y,z } {}
	
		template <typename Archiver>
		friend Archiver& operator&(Archiver& ar, shape& component)
		{
			ar.StartObject();
			ar.Member("ShapeType", component.type);
			ar.Member("ShapeData", component.shapeData);
			return ar.EndObject();
		}
		
	};


	/* three main type for collision i think
	*
	* 1. trigger (no collision at all, only triggers events)
	*		actually might not need this since everything is grid based in software
	*		and for 300 we are just doing random demos
	*		lmk what yall think
	* 2. static
	*		doesn't move, collides with all dynamic types
	* 3. dynamicasdsada
	*		is simulated by physx, collides with everything
	* 4. particles (dk if this is here anot) //
			might be some special type
	*		that we define outside
	*
	*
	*/
	struct rigidbody_config // for creation of rigidbody object
	{
		float mass = 1.f;
		float density = 1.f;				// Only for dynamic
		float linear_damping = 0.5f;		// Rate of decay overtime for linear velocty
		float angular_damping = 0.5f;		// Rate of decay overtime for angular velocty
		bool is_static = false;				// Static or Dynamic rigidbody
		bool gravity = false;				// explicit or implicit with the static/dynamic type

		vec3 linear_velocity = { 0.f, 0.f, 0.f };		// Velocity in a straight line
		vec3 angular_velocity = { 0.f, 0.f, 0.f };;		// Rotational velocity
		vec3 material = { 0.f, 0.f, 0.f };;				// staticFriction, dynamicFriction, restitution (typedef float PxReal)

		template <typename Archiver>
		friend Archiver& operator&(Archiver& ar, rigidbody_config& component)
		{
			ar.StartObject();
			ar.Member("Mass", component.mass);
			ar.Member("Density", component.density);
			ar.Member("Linear_Damping", component.linear_damping);
			ar.Member("Angular_Damping", component.angular_damping);
			ar.Member("Is_Static", component.is_static);
			ar.Member("Gravity", component.gravity);
			ar.Member("Linear_Velocity", component.linear_velocity);
			ar.Member("Angular_Velocity", component.angular_velocity);
			ar.Member("Physics Material", component.material);
			return ar.EndObject();
		}
		// just reference for you
		/*
		* PxMaterial needs this three things to be created
			PxReal staticFriction, PxReal dynamicFriction, PxReal restitution
		*/
	};
	
	class PhysicsObject
	{
	public:
		// create plane in constructor
		PhysicsObject(m_resource* mem_res = aligned_malloc_resource());

		// testing
		bool advance(float dt);
		bool fetch();

		// uesd for loading 
		tsptr<physx::PxRigidActor> createRigidbody(rigidbody_config rb_config, shape shape_data, vec3 pos, quat rot) const;
		// what other functions here?

		/**
		 * @brief What can we add here to add functionality?
		 *
		 * #notes
		 * PxTriangleMesh, PxHeightField, PxConvexMesh, PxMaterial, and PxShape can auto deleted
		 * so no need px_make, unless no references (actually not sure need db check)
		 *
		 * 0. do we want dedicate system to handle shapes? reusing shapes are good way of
		 *	saving memory but because we cannot change the size of a shape after creation time
		 *  this may be a bad idea. this is the same for materials.
		 *		since we are using grids for many software, we can configure shapes to be exactly
		 *		1x1. But this defeats the purpose of having physics simulation in the first place.
		 *
		 *
		 * 1. add actors to the scene?
		 *		can we use shared_ptr?
		 *		what actors can we add to the scene?
		 *			rigidbody?
		 *				what if we don't want to collide?
		 *					Collision filtering is possible using filter shaders // see filters
		 *				what collision types we looking at
		 *					probably static and dynamic and trigger shapes
		 *					kinematic means we move them ourselves (maybe player? for trigger testing)
		 *			complex mesh? like a person or an object by vertex?
		 *				probably too expensive?
		 *		improve scene insertion for broad phase?
		 *			amortize insertion by adding the debris to an aggregate and then add it to
		 *			the scene -> means we should have a helper "aggregate" that is not inserted
		 *			in the scene and periodically over 3+ frames (set by user?) inserted into scene
		 *
		 * 2. remove actors?
		 *		using share pointer means we can easily remove stuff
		 *		just call scene->removeActor(shared_p.get())
		 *			den we just let the shared_ptr destructor take care of the rest
		 *		or we can re-add it to the scene later if we want, doesnt matter
		 *			cos it only dies after sharePtr goes out of scope
		 *
		 * 3. physics use cases
		 *		what do we want to simulate?
		 *			triggers ->
		 *				player step on trapped tile, den we enable a trap event
		 *			particles?
		 *				we can do custom filtering for particles but might need to be careful if there is too
		 *				many (must figure out how to optimize) (physx flex)
		 *			???
		 *
		 */

		 //tsptr<physx::PxRigidBody> createActor(vec3 pos, quat rotation, rigidbody_config config) // some other stuff for configuration)
		 //{
		 //	/*float h_extents = 0.5f;
		 //	auto material = physics->createMaterial(0.5f, 0.5f, 0.6f);
		 //	PxBoxGeometry a(h_extents, h_extents, h_extents);
		 //	auto shape = physics->createShape(a, *material);*/
		 //	return nullptr;
		 //}

		 //tvector<tsptr<physx::PxRigidBody>> createParticles() // some stuff for configuration)
		 //{
		 //	return tvector<tsptr<physx::PxRigidBody>>{};
		 //}


		/* sample of creating and adding rigidbody to scene
		* tsptr<sample_rigidbody> rb = createRigidbody(config, pos, shape);
		* scene->addActor(*rb->internal_rb);
		*/

		void AddActorToScene(physx::PxRigidActor* actor) {scene->addActor(*actor); }
		void RemoveActorFromScene(physx::PxRigidActor* actor) { scene->removeActor(*actor); }
		physx::PxShape* CreateActorShape(rigidbody_config rb_config, shape shape_data);
		// testing
		tsptr<physx::PxRigidActor> create_actor(rigidbody_config rb_config, shape shape_data, vec3 pos, quat rot, id_t id);
		tpair<id_t, bool> raycast(vec3 origin, vec3 dir);
	private:
		px_allocator allocator;
		px_cpu_dispatcher pcd;

		tsptr<physx::PxFoundation> foundation;
		tsptr<physx::PxPvd> pvd;
		tsptr<physx::PxPvdTransport> transport;
		tsptr<physx::PxPhysics> physics;
		tsptr<physx::PxCooking> cooking;
		tsptr<physx::PxScene> scene;

		// testing
		float accumulator = 0.0f;
		float step_size = 1.0f / 60.0f;

		tmap<physx::PxRigidActor*, id_t> lookup;
	};
}