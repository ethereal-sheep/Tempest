

#include "CharacterSpawnEmitter_3D.h"

CharacterSpawnEmitter_3D::CharacterSpawnEmitter_3D()
{
	m_GM.m_active = true;

	m_MM.m_duration = 3.0f;
	m_MM.m_preWarm = true;
	Emitter_3D::UpdateMaxParticle(10);

	m_EM.m_spawnTimeInterval = 0.16f;
	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	m_EM.m_rateOverTime = 1;

	m_PAM.m_gravity = true;
	m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.0f, 0.0f };
	m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	m_PAM.m_scaleBegin = glm::vec3{ 0.01f, 0.01f, 0.01f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.01f, 0.01f, 0.01f };

	m_PAM.m_lifeTime = 1.6f;

	//m_EM.m_burstCycle = 5;
	//m_EM.m_burstTime = 0.01f;
	//m_EM.m_burstInterval = 0.48f;
	//m_EM.m_burstCount = 1;

	m_RM.m_renderingPath = "Models/SquareHole.a";
}