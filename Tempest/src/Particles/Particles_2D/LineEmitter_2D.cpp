


#include "LineEmitter_2D.h"

#include "../Random.h"

glm::vec2 LineEmitter_2D::RandomPoint()
{
	glm::vec2 distanceLine = m_endPoint - m_startPoint;

	glm::vec2 randomPoint = m_startPoint;

	// Perform twice to prevent same values of x and y and ensure more randomness
	randomPoint += Random::Float() * distanceLine;

	return randomPoint;
}

void LineEmitter_2D::Emit(const int particleAmount)
{
	// Emit only if enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initailisation of the particle
			Particle_2D particle;
			ParticleSetUp(particle);

			// Set position in between the points to spawn
			particle.m_position = RandomPoint();

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}

