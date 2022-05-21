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

ExplosionEmitter_3D::ExplosionEmitter_3D()
{
	m_GM.m_position = glm::vec3{ 0.0f, 0.0f, 0.0f };	
	m_MM.m_duration = 15.0f;
	m_GM.m_active = true;
	m_MM.m_preWarm = true;

	m_EM.m_spawnTimeInterval = 1.0f;
	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;

	m_EM.m_rateOverTime = 20;
	Emitter_3D::UpdateMaxParticle(1000);

	// Particle Architype values - without consideration for default ctor
	m_PAM.m_gravity = true;
	m_PAM.m_velocityStart = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityEnd = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityVariation = glm::vec3{ 10.0f, 10.0f, 10.0f };

	m_PAM.m_scaleBegin = glm::vec3{ 0.005f, 0.005f, 0.005f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	//m_PAM.m_scaleVariation = glm::vec3{ 0.3f, 0.3f, 0.3f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };
	m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };

	m_PAM.m_lifeTime = 10.0f;
	//m_RM.m_renderingPath = "Models/Cube.a";
	m_RM.m_renderingPath = "Models/Sphere.a";
}

void ExplosionEmitter_3D::Emit(const int particleAmount)
{
	// Emit only if enough particle
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

void ExplosionEmitter_3D::ParticleSetUp(Particle_3D& particle)
{
	Emitter_3D::ParticleSetUp(particle);

	short spawnSector = std::rand() % 7;
	short directionX = 1;
	short directionY = 1;
	short directionZ = 1;

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

	// Velocities Direction
	particle.m_velocity.x *= directionX;
	particle.m_velocity.y *= directionY;
	particle.m_velocity.z *= directionZ;

	particle.m_velocityBegin.x *= directionX;
	particle.m_velocityBegin.y *= directionY;
	particle.m_velocityBegin.z *= directionZ;

	particle.m_velocityEnd.x *= directionX;
	particle.m_velocityEnd.y *= directionY;
	particle.m_velocityEnd.z *= directionZ;
}