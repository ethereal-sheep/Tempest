


#include "Multiple_Rotation_ExplosionEmitter_3D.h"
#include "Rotation_ExplosionEmitter_3D.h"
#include "EmitterSystem_3D.h"

#include "../Random.h"


void Multiple_Rotation_ExplosionEmitter_3D::Setup()
{
	m_GM.m_active = true;

	m_MM.m_duration = 0.3f;
	m_MM.m_simulationSpeed = 0.016f;
	m_MM.m_preWarm = true;
	m_MM.m_maxParticles = 1000;

	m_EM.m_spawnTimeInterval = 10.0f;
	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	m_EM.m_rateOverTime = 20;
	
	// Particle Architype values - without consideration for default ctor
	m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityVariation = glm::vec3{ 2.0f, 2.0f, 2.0f };

	m_PAM.m_scaleBegin = glm::vec3{ 0.1f, 0.1f, 0.1f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.3f, 0.3f, 0.3f };

	m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };
	m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };

	m_PAM.m_lifeTime = 0.3f;
	m_RM.m_renderingPath = "Models/Cube.a";
}

void Multiple_Rotation_ExplosionEmitter_3D::ParticleSetUp(Particle_3D& particle)
{
	// Position, velcoity and base variables are all done here
	Emitter_3D::ParticleSetUp(particle);

	// Velocity Variations
	if (m_PAM.m_velocityVariation.x >= 1)
		particle.m_velocity.x += Random::Float() * static_cast<float>(m_PAM.m_velocityVariation.x) + m_PAM.m_startVelocity.x;

	if (m_PAM.m_velocityVariation.y >= 1)
		particle.m_velocity.y += Random::Float() * static_cast<float>(m_PAM.m_velocityVariation.y) + m_PAM.m_startVelocity.y;

	if (m_PAM.m_velocityVariation.z >= 1)
		particle.m_velocity.z += Random::Float() * static_cast<float>(m_PAM.m_velocityVariation.z) + m_PAM.m_startVelocity.z;

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

	// Shift the particle back by emitter to origin point
	auto shiftedPosition = particle.m_position - m_GM.m_position;

	// Dot Product
	float dotProduct_X_Axis = particle.m_velocity.x * 1;
	float dotProduct_Y_Axis = particle.m_velocity.y * 1;
	float dotProduct_Z_Axis = particle.m_velocity.z * 1;

	// Magnitude of First Vector
	float magnitude_ParticleVelocity = std::sqrtf((particle.m_velocity.x * particle.m_velocity.x) +
		(particle.m_velocity.y * particle.m_velocity.y) +
		(particle.m_velocity.z * particle.m_velocity.z));

	// Magnitude of Second Vector - Unit Vector (Axis)
	float magnitude_Axis = 1;

	// Calculate angle to rotate in degrees
	float radian_X = std::acosf(dotProduct_X_Axis / magnitude_ParticleVelocity * magnitude_Axis);
	float radian_Y = std::acosf(dotProduct_Y_Axis / magnitude_ParticleVelocity * magnitude_Axis);
	float radian_Z = std::acosf(dotProduct_Z_Axis / magnitude_ParticleVelocity * magnitude_Axis);

	particle.m_rotation.x = radian_X * 180.0f / 3.14159f;
	particle.m_rotation.y = radian_Y * 180.0f / 3.14159f;
	particle.m_rotation.z = radian_Z * 180.0f / 3.14159f;
}

void Multiple_Rotation_ExplosionEmitter_3D::OnDeath()
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
		const std::shared_ptr<Rotation_ExplosionEmitter_3D> tempEmitter = EmitterSystem_3D::GetInstance().CreateRotationExplosionEmitter(spawnPos).lock();

		// Emitter's velocity
		//tempEmitter->m_GM.m_velocity = glm::vec3{ directionX, directionY, directionZ };
		tempEmitter->m_MM.m_duration = 0.6f;
		tempEmitter->m_EM.m_spawnTimeInterval = 1.0f;
		tempEmitter->m_EM.m_spawnCountTimer = tempEmitter->m_EM.m_spawnTimeInterval;
	}
}