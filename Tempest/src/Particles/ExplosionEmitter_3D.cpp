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
#include "Random.h"

ExplosionEmitter_3D::ExplosionEmitter_3D()
	: Emitter()
	, m_RandomSpawnDirection{ 0.0f, 0.0f, 0.0f }
{}

void ExplosionEmitter_3D::Emit(const int particleAmount)
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

			// 3D randomised spawn direction
			short randSpawnDir = std::rand() % 13;
			m_RandomSpawnDirection = glm::vec3{ 500.0f, 500.0f, 500.0f };
			short directionX = (std::rand() % static_cast<short>(m_RandomSpawnDirection.x));
			short directionY = (std::rand() % static_cast<short>(m_RandomSpawnDirection.y));
			short directionZ = (std::rand() % static_cast<short>(m_RandomSpawnDirection.z));

			switch (randSpawnDir)
			{
			// Case 0 & 1 - Three Positive & Negative
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

			// Case 2,3,4  - One Negative Direction & Two Positive Direction
			case 2:
				directionX = -directionX;
				directionY = directionY;
				directionZ = directionZ;
				break;

			case 3:
				directionX = directionX;
				directionY = -directionY;
				directionZ = directionZ;
				break;

			case 4:
				directionX = directionX;
				directionY = directionY;
				directionZ = -directionZ;
				break;

			// Case 5,6,7 - Two Negative Direction & One Positive Direction
			case 5:
				directionX = -directionX;
				directionY = -directionY;
				directionZ = directionZ;
				break;

			case 6:
				directionX = -directionX;
				directionY = directionY;
				directionZ = -directionZ;
				break;

			case 7:
				directionX = directionX;
				directionY = -directionY;
				directionZ = -directionZ;
				break;
			
			default:
				break;
			}

			particle.m_velocity.x += m_PAM.m_velocityVariation.x + directionX;
			particle.m_velocity.y += m_PAM.m_velocityVariation.y + directionY;
			particle.m_velocity.z += m_PAM.m_velocityVariation.z + directionZ;

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