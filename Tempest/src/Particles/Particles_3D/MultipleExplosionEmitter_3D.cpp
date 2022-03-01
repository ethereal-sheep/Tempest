

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

			// Velocity - RNG
			particle.m_velocity = m_PAM.m_startVelocity;

			short spawnSector = std::rand() % 7;
			short directionX = (std::rand() % 40);
			short directionY = (std::rand() % 40);
			short directionZ = (std::rand() % 40);

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

	// Emit for explosion emitter
	for (short i = 0; i < m_explosionEmitterAmount; ++i)
	{
		short spawnSector = std::rand() % 7;
		short directionX = (std::rand() % 10);
		short directionY = (std::rand() % 10);
		short directionZ = (std::rand() % 10);

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

		// Creation of explosion emitter
		const std::shared_ptr<ExplosionEmitter_3D> tempEmitter = ParticleSystem_3D::GetInstance().CreateExplosionEmitter(glm::vec3{ 0.0f, 0.0f, 0.0f }).lock();

		// Emitter's velocity
		tempEmitter->m_GM.m_velocity = glm::vec3{ directionX, directionY, directionZ };
		tempEmitter->m_MM.m_preWarm = false;
	}

	// Performs emit only once
	m_explosionEmitterAmount = 0;
}