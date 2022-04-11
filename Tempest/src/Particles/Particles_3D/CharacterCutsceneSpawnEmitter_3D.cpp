

#include "CharacterCutsceneSpawnEmitter_3D.h"

#include "../Random.h"
#include <algorithm>

#include "Logger/Log.h"

CharacterCutsceneSpawnEmitter_3D::CharacterCutsceneSpawnEmitter_3D()
{
	// Initialise all the waypoints
	for (short i = 0; i < waypoint_Index; ++i)
		m_wayPoints[i] = glm::vec3{ 0.0f, 0.0f, 0.0f };

	// Values for Emission Module
	m_GM.m_active = true;

	m_MM.m_duration = 0.4f;
	m_MM.m_preWarm = true;
	Emitter_3D::UpdateMaxParticle(500);

	/*m_EM.m_spawnTimeInterval = 0.08f;
	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	m_EM.m_rateOverTime = 4;*/

	m_EM.m_burstInterval = 0.096f;
	m_EM.m_burstCount = 4;
	m_EM.m_burstCycle = 8;

	// Particle Architype values - without consideration for default ctor
	m_PAM.m_velocityStart = glm::vec3{ 0.f, 5.f, 0.0f };
	m_PAM.m_velocityEnd = glm::vec3{ 0.f, 5.f, 0.0f };
	m_PAM.m_velocityVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	m_PAM.m_scaleBegin = glm::vec3{ 0.03f, 0.03f, 0.03f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.03f, 0.03f, 0.03f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.f, 0.f, 0.f };

	// Note - Values to be divided by 255.0f - Forgot the reason
	m_PAM.m_colourBegin = glm::vec4{ 0.f / 255.f, 191.f / 255.f, 255.f / 255.f, 1.0f };
	m_PAM.m_colourEnd = glm::vec4{ 0.f / 255.f, 191.f / 255.f, 255.f / 255.f, 1.0f };

	m_PAM.m_lifeTime = 0.5f;
	//m_PAM.m_lifeTime = 0.3f;
	m_PAM.m_rebirth = false;

	m_RM.m_renderingPath = "Models/Cube.a";
	m_RM.m_emissiveLighting = true;
}

void CharacterCutsceneSpawnEmitter_3D::AssignWaypoint(const std::array<glm::vec3, waypoint_Index> newWaypoints)
{
	// Initialise all the waypoints
	for (short i = 0; i < waypoint_Index; ++i)
		m_wayPoints[i] = newWaypoints[i];
}

void CharacterCutsceneSpawnEmitter_3D::Emit(const int particleAmount)
{
	// Emit only if enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initailisation of the particle
			Particle_3D particle;
			ParticleSetUp(particle);

			/*
			* 0 - Left
			* 1 - Right
			* 2 - Top
			* 3 - Btm
			*/

			switch (i)
			{
			case 0: // LEFT
				particle.m_position = m_wayPoints[0];

				particle.m_velocityBegin = glm::vec3{ 0.f, -8.f, -1.5f };
				particle.m_velocityEnd = glm::vec3{ 0.f, -8.f, -1.5f };

				particle.m_scaleBegin = glm::vec3{ 0.2f, 0.03f, 0.01f };
				particle.m_scaleEnd = glm::vec3{ 0.85f, 0.03f, 0.03f };
				break;
			case 1: // RIGHT
				particle.m_position = m_wayPoints[1];

				particle.m_velocityBegin = glm::vec3{ 0.f, -8.f, 1.5f };
				particle.m_velocityEnd = glm::vec3{ 0.f, -8.f, 1.5f };

				particle.m_scaleBegin = glm::vec3{ 0.2f, 0.03f, 0.01f };
				particle.m_scaleEnd = glm::vec3{ 0.85f, 0.03f, 0.03f };
				break;
			case 2: // TOP
				particle.m_position = m_wayPoints[2];

				particle.m_velocityBegin = glm::vec3{ 1.5f, -8.f, 0.f };
				particle.m_velocityEnd = glm::vec3{ 1.5f, -8.f, 0.0f };

				particle.m_scaleBegin = glm::vec3{ 0.01f, 0.03f, 0.2f };
				particle.m_scaleEnd = glm::vec3{ 0.03f, 0.03f, 0.85f };
				break;
			case 3: // BTM
				particle.m_position = m_wayPoints[3];

				particle.m_velocityBegin = glm::vec3{ -1.5f, -8.f, 0.f };
				particle.m_velocityEnd = glm::vec3{ -1.5f, -8.f, 0.0f };

				particle.m_scaleBegin = glm::vec3{ 0.01f, 0.03f, 0.2f };
				particle.m_scaleEnd = glm::vec3{ 0.03f, 0.03f, 0.85f };
				break;
			default:
				break;
			}

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}