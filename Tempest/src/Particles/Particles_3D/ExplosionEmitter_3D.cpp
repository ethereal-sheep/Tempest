/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "ExplosionEmitter_3D.h"

// Additional Includes
#include "../Random.h"

void ExplosionEmitter_3D::Emit(const int particleAmount)
{
	// Emit only if enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initialisation of the particle
			Particle_3D particle;

			particle.m_position = m_GM.m_position;
			particle.m_isActive = true;
			//particle.m_rotation = Random::Float() * 2.0f * std::numbers::pi;

			// Velocity - RNG
			particle.m_velocity = m_PAM.m_startVelocity;

			short spawnSector = std::rand() % 7;
			short directionX = (std::rand() % 10);
			short directionY = (std::rand() % 10);
			short directionZ = (std::rand() % 10);

			switch (spawnSector)
			{
			case 0:
				directionX = directionX;
				directionY = directionY;
				directionZ = directionZ;
				break;
			case 1:
				directionX = -directionX;
				directionY = -directionY;
				directionZ = -directionZ;
				break;

			case 2:
				directionX = directionX;
				directionY = directionY;
				directionZ = -directionZ;
				break;
			case 3:
				directionX = directionX;
				directionY = -directionY;
				directionZ = directionZ;
				break;
			case 4:
				directionX = directionX;
				directionY = -directionY;
				directionZ = -directionZ;
				break;
			case 5:
				directionX = -directionX;
				directionY = directionY;
				directionZ = directionZ;
				break;
			case 6:
				directionX = -directionX;
				directionY = directionY;
				directionZ = -directionZ;
				break;

			default:
				break;
			}

			// Velocities
			particle.m_velocity.x += m_PAM.m_velocityVariation.x + directionX;
			particle.m_velocity.y += m_PAM.m_velocityVariation.y + directionY;
			particle.m_velocity.z += m_PAM.m_velocityVariation.z + directionZ;

			//particle.m_velocity.x += m_PAM.m_velocityVariation.x - (Random::Float() - 50.f);
			//particle.m_velocity.y += m_PAM.m_velocityVariation.y - (Random::Float() - 50.f);

			// Color
			particle.m_colourBegin = m_PAM.m_colourBegin;
			particle.m_colourEnd = m_PAM.m_colourEnd;

			// Lifetime
			particle.m_lifeTime = m_PAM.m_lifeTime;
			particle.m_lifeRemaining = m_PAM.m_lifeTime;

			// Size Variation
			particle.m_scale.x = m_PAM.m_scaleBegin.x + m_PAM.m_scaleVariation.x *(Random::Float() - 0.5f);
			particle.m_scale.y = m_PAM.m_scaleBegin.y + m_PAM.m_scaleVariation.y *(Random::Float() - 0.5f);
			particle.m_scale.z = m_PAM.m_scaleBegin.z + m_PAM.m_scaleVariation.z *(Random::Float() - 0.5f);

			particle.m_renderingPath = m_RM.m_renderingPath;

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}