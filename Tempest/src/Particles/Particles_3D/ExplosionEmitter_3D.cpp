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

	// Velocity Variations
	if (m_PAM.m_velocityVariation.x >= 1)
		particle.m_velocity.x += Random::Float() * static_cast<int>(m_PAM.m_velocityVariation.x);

	if (m_PAM.m_velocityVariation.y >= 1)
		particle.m_velocity.y += Random::Float() * static_cast<int>(m_PAM.m_velocityVariation.y);

	if (m_PAM.m_velocityVariation.z >= 1)
		particle.m_velocity.z += Random::Float() * static_cast<int>(m_PAM.m_velocityVariation.z);

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

}