


#include "Interactive_DoorParticle_3D.h"

#include "../Random.h"

#include <algorithm> //std::clamp

#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <numbers>		/* numbers */

void InteractiveParticle_3D::Emit(const int particleAmount)
{
	// Emit only if there is enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initialisation of the particle
			Particle_3D particle;
			ParticleSetUp(particle);

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}

void InteractiveParticle_3D::ParticleSetUp(Particle_3D& particle)
{
	Emitter_3D::ParticleSetUp(particle);

	// Spawn Position of the particle - To be between min and max of range
	float rangeX = static_cast<float>(maxPos.x - minPos.x) > 0 ? static_cast<float>(maxPos.x - minPos.x) : 0;
	float rangeY = static_cast<float>(maxPos.y - minPos.y) > 0 ? static_cast<float>(maxPos.y - minPos.y) : 0;
	float rangeZ = static_cast<float>(maxPos.z - minPos.z) > 0 ? static_cast<float>(maxPos.z - minPos.z) : 0;

	if(rangeX)
		particle.m_position.x = Random::Float() * rangeX + minPos.x;

	if(rangeY)
		particle.m_position.y = Random::Float() * rangeY + minPos.y;

	if(rangeZ)
		particle.m_position.z = Random::Float() * rangeZ + minPos.z;
}