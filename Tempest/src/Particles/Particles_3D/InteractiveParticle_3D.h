


#ifndef INTERACTIVE_PARTICLE_3D_H
#define INTERACTIVE_PARTICLE_3D_H

#include "Emitter_3D.h"

#include <gtx/transform.hpp>

struct InteractiveParticle_3D : public Emitter_3D
{
	//InteractiveParticle_3D(glm::vec3 minPos, glm::vec3);

	void Emit(const int partcielAmount) override;

	// Min and Max range of the spawn pos
	glm::vec3 minPos;
	glm::vec3 maxPos;
};


#endif // !INTERACTIVE_PARTICLE_3D_H