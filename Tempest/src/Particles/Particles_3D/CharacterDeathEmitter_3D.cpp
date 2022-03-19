
#include "CharacterDeathEmitter_3D.h"
#include "Rotation_ExplosionEmitter_3D.h"
#include "../Random.h"
#include "EmitterSystem_3D.h"

void CharacterDeathEmitter_3D::Setup()
{
	m_MM.m_duration = 0.3f;
	m_MM.m_simulationSpeed = 0.016f;
	m_GM.m_active = true;
	m_MM.m_preWarm = true;

	m_EM.m_spawnTimeInterval = 10.0f;
	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	m_EM.m_rateOverTime = 20;
	m_MM.m_maxParticles = 1000;

	m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityVariation = glm::vec3{ 10.0f, 10.0f, 10.0f };

	m_PAM.m_scaleBegin = glm::vec3{ 0.03f, 0.03f, 0.03f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.02f, 0.02f, 0.02f };

	m_PAM.m_lifeTime = 0.3f;
	m_RM.m_renderingPath = "Models/Cube.a";
}

void CharacterDeathEmitter_3D::OnDeath()
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
		int rangeX = static_cast<int>(m_maxPos.x - m_minPos.x) > 0 ? static_cast<int>(m_maxPos.x - m_minPos.x) : 1;
		int rangeY = static_cast<int>(m_maxPos.y - m_minPos.y) > 0 ? static_cast<int>(m_maxPos.y - m_minPos.y) : 1;
		int rangeZ = static_cast<int>(m_maxPos.z - m_minPos.z) > 0 ? static_cast<int>(m_maxPos.z - m_minPos.z) : 1;

		glm::vec3 spawnPos;

		spawnPos.x = Random::Float() * rangeX + m_minPos.x;
		spawnPos.y = Random::Float() * rangeY + m_minPos.y;
		spawnPos.z = Random::Float() * rangeZ + m_minPos.z;

		// Creation of explosion emitter
		const std::shared_ptr<Rotation_ExplosionEmitter_3D> tempEmitter = EmitterSystem_3D::GetInstance().CreateRotationExplosionEmitter(spawnPos).lock();

		// Emitter's velocity
		//tempEmitter->m_GM.m_velocity = glm::vec3{ directionX, directionY, directionZ };
		tempEmitter->m_MM.m_duration = 0.6f;
		tempEmitter->m_EM.m_spawnTimeInterval = 1.0f;
		tempEmitter->m_EM.m_spawnCountTimer = tempEmitter->m_EM.m_spawnTimeInterval;

		tempEmitter->m_PAM.m_gravity = true;
		tempEmitter->m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
		tempEmitter->m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
		tempEmitter->m_PAM.m_velocityVariation = glm::vec3{ 10.0f, 10.0f, 10.0f };

		tempEmitter->m_PAM.m_scaleBegin = glm::vec3{ 0.03f, 0.03f, 0.03f };
		tempEmitter->m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
		tempEmitter->m_PAM.m_scaleVariation = glm::vec3{ 0.02f, 0.02f, 0.02f };

		tempEmitter->m_PAM.m_colourBegin = m_PAM.m_colourBegin;
		tempEmitter->m_PAM.m_colourEnd = m_PAM.m_colourEnd;
	}
}