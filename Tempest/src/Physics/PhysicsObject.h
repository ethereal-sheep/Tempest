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

	/* three main type for collision i think
	*
	* 1. trigger (no collision at all, only triggers events)
	*		actually might not need this since everything is grid based in software
	*		and for 300 we are just doing random demos
	*		lmk what yall think
	* 2. static
	*		doesn't move, collides with all dynamic types
	* 3. dynamic
	*		is simulated by physx, collides with everything
	* 4. particles (dk if this is here anot) //
			might be some special type
	*		that we define outside
	*
	*
	*/
	struct rigidbody_config // for creation of rigidbody object
	{
		float mass;
		float density;				// Only for dynamic
		float linear_damping;		// Rate of decay overtime for linear velocty
		float angular_damping;		// Rate of decay overtime for angular velocty
		bool is_static;				// Static or Dynamic rigidbody
		bool gravity;				// explicit or implicit with the static/dynamic type

		vec3 linear_velocity;		// Velocity in a straight line
		vec3 angular_velocity;		// Rotational velocity
		vec3 material;				// staticFriction, dynamicFriction, restitution (typedef float PxReal)

		// just reference for you
		/*
		* PxMaterial needs this three things to be created
			PxReal staticFriction, PxReal dynamicFriction, PxReal restitution
		*/
	};



	struct sample_rigidbody
	{
		// maybe? implementation

		/**
		 * @brief MUST BE CREATED WITH px_make!
		 * We can possibly make it only initializable with PhysicsObject.
		 */

		 // gravity yes:no
		 // masss
		 // density 
		 // material -> default/preset donnid to be adjusted by designer
		 // resolve collision yes:no () 

		 // shape (capsule, sphere, box, )
		// mentioned earlier, complex shapes need cooking and we said we don't need
		// however, if we want to simulate dice roll (we need complex shapes)
		// so just do all the standard dice first ()
		// d4 (tetrahedron, tetrapyramid, 4 triangles glued-tgt)
		// d6 (cube)
		// d8 (octahedron (2 tetrahedron))
		// d10 (no fking idea what shape is this but is
		// d12 (dodecahedron)
		// d20 icosahedron
		/*
		*
		*	[0, 2, 3],
			[0, 3, 4],
			[0, 4, 5],
			[0, 5, 6],
			[0, 6, 7],
			[0, 7, 8],
			[0, 8, 9],
			[0, 9, 10],
			[0, 10, 11],
			[0, 11, 2],
			[1, 3, 2],
			[1, 4, 3],
			[1, 5, 4],
			[1, 6, 5],
			[1, 7, 6],
			[1, 8, 7],
			[1, 9, 8],
			[1, 10, 9],
			[1, 11, 10],
			[1, 2, 11]

		*/

		rigidbody_config rb_config;

		tsptr<physx::PxRigidActor> internal_rb;
	};

	// TODO: create shape factory so can reuse shape

	class PhysicsObject
	{
	public:
		// create plane in constructor
		PhysicsObject(m_resource* mem_res = aligned_malloc_resource());

		// testing
		bool advance(float dt);
		bool fetch();

		tsptr<sample_rigidbody> createRigidbody(rigidbody_config rb_config, vec3 pos, tsptr<physx::PxShape> shape) const;
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
	};
}