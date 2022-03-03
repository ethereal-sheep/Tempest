


#include "InteractiveParticle_3D.h"

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
	int rangeX = static_cast<int>(maxPos.x - minPos.x) > 0 ? static_cast<int>(maxPos.x - minPos.x) : 1;
	int rangeY = static_cast<int>(maxPos.y - minPos.y) > 0 ? static_cast<int>(maxPos.y - minPos.y) : 1;
	int rangeZ = static_cast<int>(maxPos.z - minPos.z) > 0 ? static_cast<int>(maxPos.z - minPos.z) : 1;

	particle.m_position.x = std::rand() % rangeX + minPos.x;
	particle.m_position.y = std::rand() % rangeY + minPos.y;
	particle.m_position.z = std::rand() % rangeZ + minPos.z;
}