/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "ExplosionEmitter_2D.h"

// Additional Includes
#include "../Random.h"

void ExplosionEmitter_2D::Emit(const int particleAmount)
{
	// Emit only if enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initailisation of the particle
			Particle_2D particle;
			Emitter_2D::ParticleSetUp(particle);

			short spawnSector = std::rand() % 4;
			short directionX = (std::rand() % 500);
			short directionY = (std::rand() % 500);

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

			particle.m_velocity.x += m_PAM.m_velocityVariation.x + directionX;
			particle.m_velocity.y += m_PAM.m_velocityVariation.y + directionY;

			//particle.m_velocity.x += m_PAM.m_velocityVariation.x - (Random::Float() - 50.f);
			//particle.m_velocity.y += m_PAM.m_velocityVariation.y - (Random::Float() - 50.f);

			//// Color
			//particle.m_colour.r = (Random::Float() - 0.5f);
			//particle.m_colour.g = (Random::Float() - 0.5f);
			//particle.m_colour.b = (Random::Float() - 0.5f);

			//particle.m_type = m_RM.m_type;

			//// Lifetime
			//particle.m_lifeTime = m_PAM.m_lifeTime;
			//particle.m_lifeRemaining = m_PAM.m_lifeTime;
			//particle.m_size = m_PAM.m_scaleBegin + m_PAM.m_scaleVariation * (Random::Float() - 0.5f);

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}