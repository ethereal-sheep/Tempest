

#include "MultipleExplosionEmitter_3D.h"
#include "ExplosionEmitter_3D.h"
#include "ParticleSystem_3D.h"

// Additional Includes
#include "../Random.h"

MultipleExplosionEmitter_3D::MultipleExplosionEmitter_3D(int explosionEmitterAmount)
	: m_explosionEmitterAmount{ explosionEmitterAmount }
{}


void MultipleExplosionEmitter_3D::Emit(const int particleAmount)
{
	// Emit only if enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initailisation of the particle
			Particle_3D particle;

			particle.m_position = m_GM.m_position;
			particle.m_isActive = true;
			//particle.m_rotation = Random::Float() * 2.0f * std::numbers::pi;

			// Initial Velocity 
			particle.m_velocity = m_PAM.m_startVelocity;

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

			// Color
			particle.m_colourBegin = m_PAM.m_colourBegin;
			particle.m_colourEnd = m_PAM.m_colourEnd;

			// Lifetime
			particle.m_lifeTime = m_PAM.m_lifeTime;
			particle.m_lifeRemaining = m_PAM.m_lifeTime;

			// Size Variation
			particle.m_scale.x = m_PAM.m_scaleBegin.x + m_PAM.m_scaleVariation.x * (Random::Float() - 0.5f);
			particle.m_scale.y = m_PAM.m_scaleBegin.y + m_PAM.m_scaleVariation.y * (Random::Float() - 0.5f);
			particle.m_scale.z = m_PAM.m_scaleBegin.z + m_PAM.m_scaleVariation.z * (Random::Float() - 0.5f);

			particle.m_renderingPath = m_RM.m_renderingPath;

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}

void MultipleExplosionEmitter_3D::OnDeath()
{
	 // Emit for explosion emitter
	for (short i = 0; i < m_explosionEmitterAmount; ++i)
	{
		short spawnSector = std::rand() % 7;
		short directionX = 1;
		short directionY = 1;
		short directionZ = 1;

		// Spawn Velocity Direction
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

		// Spawn Position of the particle - To be between min and max of range
		int rangeX = static_cast<int>(maxPos.x - minPos.x) > 0 ? static_cast<int>(maxPos.x - minPos.x) : 1;
		int rangeY = static_cast<int>(maxPos.y - minPos.y) > 0 ? static_cast<int>(maxPos.y - minPos.y) : 1;
		int rangeZ = static_cast<int>(maxPos.z - minPos.z) > 0 ? static_cast<int>(maxPos.z - minPos.z) : 1;

		glm::vec3 spawnPos;

		spawnPos.x = Random::Float() * rangeX + minPos.x;
		spawnPos.y = Random::Float() * rangeY + minPos.y;
		spawnPos.z = Random::Float() * rangeZ + minPos.z;

		// Creation of explosion emitter
		const std::shared_ptr<ExplosionEmitter_3D> tempEmitter = ParticleSystem_3D::GetInstance().CreateExplosionEmitter(spawnPos).lock();

		// Emitter's velocity
		//tempEmitter->m_GM.m_velocity = glm::vec3{ directionX, directionY, directionZ };
		tempEmitter->m_MM.m_duration = 0.6f;
		tempEmitter->m_EM.m_spawnTimeInterval = 1.0f;
		tempEmitter->m_EM.m_spawnCountTimer = tempEmitter->m_EM.m_spawnTimeInterval;
	}
}