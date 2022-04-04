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

ExplosionEmitter_2D::ExplosionEmitter_2D()
{
	// Emitter_2D values - Without consideration for default ctor values
	m_GM.m_position = glm::vec2{ 0.f, 0.f };
	m_MM.m_duration = 2.f;
	m_GM.m_active = true;
	m_MM.m_preWarm = true;

	m_EM.m_spawnTimeInterval = 2.f;
	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	m_EM.m_rateOverTime = 20;
	m_MM.m_maxParticles = 1000;

	m_PAM.m_velocityStart = glm::vec2{ 0.f, 0.f };
	m_PAM.m_velocityEnd = glm::vec2{ 0.f, 0.f };
	m_PAM.m_velocityVariation = glm::vec2{ 500.0f, 500.0f };

	m_PAM.m_scaleBegin = 10.0f;
	m_PAM.m_scaleEnd = 0.0f;
	m_PAM.m_scaleVariation = 0.3f;

	m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	m_PAM.m_lifeTime = 0.3f;
	m_RM.m_type = ParticleType::Circle;
}

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

			particle.m_velocityBegin.x *= directionX;
			particle.m_velocityEnd.x *= directionX;

			particle.m_velocityBegin.y *= directionY;
			particle.m_velocityEnd.y *= directionY;

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}