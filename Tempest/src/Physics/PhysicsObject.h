#pragma once

#include "TMath.h"
#include "Core.h"
#include "Memory.h"
#include "Util/px_include.h"

namespace Tempest
{
	struct PhysicsConfig
	{
		vec3 gravity = { 0.0f, -9.81f, 0.0f };
		bool pvd_enabled = false;
		// bunch of other shit
	};

	struct sample_rigidbody
	{
		// maybe? implementation

		/**
		 * @brief MUST BE CREATED WITH px_make!
		 * We can possibly make it only initializable with PhysicsObject.
		 */
		tsptr<physx::PxActor> actor;

	};


	class PhysicsObject
	{
	public:
		PhysicsObject(m_resource* mem_res = aligned_malloc_resource());

		// testing
		bool advance(float dt);
		bool fetch();

		// what other functions here?

		/**
		 * @brief What can we add here to add functionality?
		 *
		 * #notes
		 * PxTriangleMesh, PxHeightField, PxConvexMesh, PxMaterial, and PxShape can auto deleted
		 * so no need px_make, unless no references (actually not sure need db check)
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
		 *		or we can readd it to the scene later if we want, doesnt matter
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
