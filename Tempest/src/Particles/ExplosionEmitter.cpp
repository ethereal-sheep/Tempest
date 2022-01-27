

#include "ExplosionEmitter.h"

// Additional Includes
#include "Random.h"

void ExplosionEmitter::Emit(const int particleAmount)
{
	// Emit only if enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initailisation of the particle
			Particle particle;

			particle.m_position = m_GM.m_position;
			particle.m_isActive = true;
			//particle.m_rotation = Random::Float() * 2.0f * std::numbers::pi;

			// Velocity - RNG
			particle.m_velocity = m_PAM.m_startVelocity;

			short spawnSector = std::rand() % 5;
			short directionX = (std::rand() % 1000);
			short directionY = (std::rand() % 1000);

			switch (spawnSector)
			{
			case 1:
				directionX = -directionX;
				directionY = directionY;
				break;
			case 2:
				directionX = directionX;
				directionY = directionY;
				break;
			case 3:
				directionX = -directionX;
				directionY = -directionY;
				break;
			case 4:
				directionX = directionX;
				directionY = -directionY;
				break;

			default:
				break;
			}

			particle.m_velocity.x += m_PAM.m_velocityVariation.x + directionX;
			particle.m_velocity.y += m_PAM.m_velocityVariation.y + directionY;

			//particle.m_velocity.x += m_PAM.m_velocityVariation.x - (Random::Float() - 50.f);
			//particle.m_velocity.y += m_PAM.m_velocityVariation.y - (Random::Float() - 50.f);

			// Color
			particle.m_colour.r = (Random::Float() - 0.5f);
			particle.m_colour.g = (Random::Float() - 0.5f);
			particle.m_colour.b = (Random::Float() - 0.5f);

			particle.m_type = m_RM.m_type;

			// Lifetime
			particle.m_lifeTime = m_PAM.m_lifeTime;
			particle.m_lifeRemaining = m_PAM.m_lifeTime;
			particle.m_size = m_PAM.m_sizeBegin + m_PAM.m_sizeVariation * (Random::Float() - 0.5f);

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}