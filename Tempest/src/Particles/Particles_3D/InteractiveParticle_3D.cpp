


#include "InteractiveParticle_3D.h"

#include "../Random.h"

#include <algorithm> //std::clamp

#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

void InteractiveParticle_3D::Emit(const int particleAmount)
{
	//srand(time(NULL));

	// Emit only if there is enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initialisation of the particle
			Particle_3D particle;

			// Spawn Position of the particle - To be between min and max of range
			int rangeX = static_cast<int>(maxPos.x - minPos.x) > 0 ? static_cast<int>(maxPos.x - minPos.x) : 1;
			int rangeY = static_cast<int>(maxPos.y - minPos.y) > 0 ? static_cast<int>(maxPos.y - minPos.y) : 1;
			int rangeZ = static_cast<int>(maxPos.z - minPos.z) > 0 ? static_cast<int>(maxPos.z - minPos.z) : 1;

			particle.m_position.x = std::rand() % rangeX + minPos.x;
			particle.m_position.y = std::rand() % rangeY + minPos.y;
			particle.m_position.z = std::rand() % rangeZ + minPos.z;

			// Velocity - Should be 0
			particle.m_velocity = m_PAM.m_startVelocity;

			// Rotation
			//particle.m_rotation = Random::Float() * 2.0f * std::numbers::pi;

			// Active bool
			particle.m_isActive = true;

			// Color
			particle.m_colour.r = (Random::Float() - 0.5f);
			particle.m_colour.g = (Random::Float() - 0.5f);
			particle.m_colour.b = (Random::Float() - 0.5f);

			// Lifetime
			particle.m_lifeTime = m_PAM.m_lifeTime;
			particle.m_lifeRemaining = m_PAM.m_lifeTime;

			// Size Variation
			int sizeVariation = 0;

			if(m_PAM.m_sizeVariation > 0)
				sizeVariation = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / m_PAM.m_sizeVariation));

			particle.m_size = m_PAM.m_sizeBegin + sizeVariation;// * (Random::Float() - 0.5f);

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}