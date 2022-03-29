

#include "RandomSpawnPointEmitter_2D.h"
#include "../Random.h"


RandomSpawnPointEmitter_2D::RandomSpawnPointEmitter_2D()
	: m_centre { 0.0f,0.0f }
	, m_radius { 0.0f }
{

}

glm::vec2 RandomSpawnPointEmitter_2D::RandomPoint()
{
	// A random point between the axis 
	float randomDistance = Random::Float() * m_radius;

	// Pick the random point
	glm::vec2 randomPoint_2D = m_centre + randomDistance;

	return randomPoint_2D;
}

void RandomSpawnPointEmitter_2D::Emit(const int particleAmount)
{
	// Emit only if enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initailisation of the particle
			Particle_2D particle;

			particle.m_position = RandomPoint();
			particle.m_isActive = true;
			//particle.m_rotation = Random::Float() * 2.0f * std::numbers::pi;

			// Velocity - RNG
			particle.m_velocity = m_PAM.m_velocityStart;

			short spawnSector = std::rand() % 4;
			short directionX = 1;
			short directionY = 1;

			switch (spawnSector)
			{
			case 0:
				directionX = -directionX;
				directionY = directionY;
				break;
			case 1:
				directionX = directionX;
				directionY = directionY;
				break;
			case 2:
				directionX = -directionX;
				directionY = -directionY;
				break;
			case 3:
				directionX = directionX;
				directionY = -directionY;
				break;

			default:
				break;
			}

			particle.m_velocity.x = Random::Float() * m_PAM.m_velocityVariation.x * directionX;
			particle.m_velocity.y = Random::Float() * m_PAM.m_velocityVariation.y * directionY;

			// Color
			particle.m_colour.r = (Random::Float() - 0.5f);
			particle.m_colour.g = (Random::Float() - 0.5f);
			particle.m_colour.b = (Random::Float() - 0.5f);

			particle.m_type = m_RM.m_type;

			// Lifetime
			particle.m_lifeTime = m_PAM.m_lifeTime;
			particle.m_lifeRemaining = m_PAM.m_lifeTime;
			particle.m_size = m_PAM.m_scaleBegin + m_PAM.m_scaleVariation * (Random::Float() - 0.5f);

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}