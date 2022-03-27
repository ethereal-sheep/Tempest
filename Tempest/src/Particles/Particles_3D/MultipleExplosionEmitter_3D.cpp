

#include "MultipleExplosionEmitter_3D.h"
#include "ExplosionEmitter_3D.h"
#include "EmitterSystem_3D.h"

// Additional Includes
#include "../Random.h"

MultipleExplosionEmitter_3D::MultipleExplosionEmitter_3D(int explosionEmitterAmount)
	: m_explosionEmitterAmount{ explosionEmitterAmount }
	, m_minPos { glm::vec3 {0.0f, 0.0f, 0.0f} }
	, m_maxPos { glm::vec3 {0.0f, 0.0f, 0.0f} }
{
	m_GM.m_active = true;

	m_MM.m_duration = 0.3f;
	m_MM.m_simulationSpeed = 0.016f;
	m_MM.m_preWarm = true;
	Emitter_3D::UpdateMaxParticle(1000);

	m_EM.m_spawnTimeInterval = 10.0f;
	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	m_EM.m_rateOverTime = 20;

	m_PAM.m_velocityStart = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityEnd = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityVariation = glm::vec3{ 10.0f, 10.0f, 10.0f };

	m_PAM.m_scaleBegin = glm::vec3{ 0.5f, 0.5f, 0.5f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.3f, 0.3f, 0.3f };

	m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };
	m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };

	m_PAM.m_lifeTime = 0.3f;

	m_RM.m_renderingPath = "Models/Cube.a";
}

void MultipleExplosionEmitter_3D::Emit(const int particleAmount)
{
	// Emit only if enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initailisation of the particle
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

void MultipleExplosionEmitter_3D::ParticleSetUp(Particle_3D& particle)
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
		float rangeX = static_cast<float>(m_maxPos.x - m_minPos.x) > 0 ? static_cast<float>(m_maxPos.x - m_minPos.x) : 0;
		float rangeY = static_cast<float>(m_maxPos.y - m_minPos.y) > 0 ? static_cast<float>(m_maxPos.y - m_minPos.y) : 0;
		float rangeZ = static_cast<float>(m_maxPos.z - m_minPos.z) > 0 ? static_cast<float>(m_maxPos.z - m_minPos.z) : 0;

		glm::vec3 spawnPos{};


		if (rangeX)
			spawnPos.x = Random::Float() * rangeX + m_minPos.x;

		if (rangeY)
			spawnPos.y = Random::Float() * rangeY + m_minPos.y;

		if (rangeZ)
			spawnPos.z = Random::Float() * rangeZ + m_minPos.z;

		// Creation of explosion emitter
		const std::shared_ptr<ExplosionEmitter_3D> tempEmitter = EmitterSystem_3D::GetInstance().CreateExplosionEmitter(spawnPos).lock();

		// Emitter's velocity
		//tempEmitter->m_GM.m_velocity = glm::vec3{ directionX, directionY, directionZ };
		tempEmitter->m_MM.m_duration = 0.6f;
		tempEmitter->m_EM.m_spawnTimeInterval = 1.0f;
		tempEmitter->m_EM.m_spawnCountTimer = tempEmitter->m_EM.m_spawnTimeInterval;
	}
}