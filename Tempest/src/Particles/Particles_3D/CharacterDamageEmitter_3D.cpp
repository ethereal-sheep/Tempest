

#include "CharacterDamageEmitter_3D.h"

void CharacterDamageEmitter_3D::Setup()
{
	m_GM.m_active = true;

	m_MM.m_duration = 0.6f;
	m_MM.m_preWarm = true;
	m_MM.m_maxParticles = 1000;

	m_EM.m_spawnTimeInterval = 1.0f; // 5x slower of dt
	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	m_EM.m_rateOverTime = 0;
	
	m_PAM.m_gravity = true;
	m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityVariation = glm::vec3{ 1.0f, 2.0f, 1.0f };

	m_PAM.m_scaleBegin = glm::vec3{ 0.03f, 0.03f, 0.03f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.02f, 0.02f, 0.02f };

	m_PAM.m_lifeTime = 0.6f;

	m_EM.m_burstCycle = 1;
	m_EM.m_burstTime = 0.01f;
	m_EM.m_burstCount = 20;

	m_RM.m_renderingPath = "Models/Cube.a";
}