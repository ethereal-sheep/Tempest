


#include "Interactive_DoorParticle_3D.h"

#include "../Random.h"

#include <algorithm> //std::clamp

#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <numbers>		/* numbers */

InteractiveParticle_3D::InteractiveParticle_3D()
{
	m_GM.m_active = true;

	m_MM.m_duration = 1000000.f;
	m_MM.m_preWarm = true;
	m_MM.m_maxParticles = 30;

	m_EM.m_spawnTimeInterval = 0.1f;
	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	m_EM.m_rateOverTime = 1;
	
	// Particle Architype values - without consideration for default ctor
	m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityVariation = glm::vec3{ 0.0f, 3.0f, 0.0f };

	m_PAM.m_scaleBegin = glm::vec3{ 0.03f, 0.03f, 0.03f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.02f, 0.02f, 0.02f };

	// Note - Values to be divided by 255.0f - Forgot the reason
	m_PAM.m_colourBegin = glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f };
	m_PAM.m_colourEnd = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };

	m_PAM.m_lifeTime = 1.0f;
	m_PAM.m_rebirth = false;

	m_RM.m_renderingPath = "Models/Cube.a";
}

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