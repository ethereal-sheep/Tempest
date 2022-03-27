

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

	m_PAM.m_lifeTime = 0.5f;
	m_PAM.m_gravity = false;

	m_PAM.m_velocityStart = glm::vec3{ 0.f, 6.0f, -1.0f };
	m_PAM.m_velocityEnd = glm::vec3{ 1.0f, -1.0f, -1.2f };
	m_PAM.m_velocityVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	m_PAM.m_scaleBegin = glm::vec3{ 0.03f, 0.01f, 0.03f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.01f, 0.01f, 0.01f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_PAM.m_colourEnd   = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	//m_EM.m_burstCycle = 5;
	//m_EM.m_burstTime = 0.01f;
	//m_EM.m_burstInterval = 0.48f;
	//m_EM.m_burstCount = 1;

	m_RM.m_renderingPath = "Models/SquareHole.a";
	m_RM.m_emissiveLighting = true;
}