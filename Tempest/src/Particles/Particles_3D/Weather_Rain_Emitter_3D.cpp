
#include "Weather_Rain_Emitter_3D.h"

Weather_Rain_Emitter_3D::Weather_Rain_Emitter_3D()
{
	m_GM.m_active = true;

	m_MM.m_duration = 1000000.f;
	m_MM.m_preWarm = false;

	Emitter_3D::UpdateMaxParticle(20000);

	m_EM.m_spawnTimeInterval = 0.16f;
	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	m_EM.m_rateOverTime = 2000;

	// Particle Architype values - without consideration for default ctor
	m_PAM.m_SpawnRotation.z = 60;

	m_PAM.m_startVelocity = glm::vec3{ 10.f, -50.0f, 0.0f };
	m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityVariation = glm::vec3{ 30.0f, -1000.0f, 0.0f };
	m_PAM.m_gravity = true;

	m_PAM.m_scaleBegin = glm::vec3{ 0.003f, 0.05f, 0.003f };
	//m_PAM.m_scaleBegin = glm::vec3{ 0.001f, 0.1f, 0.001f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.0f, 0.05f, 0.0f };

	// Note - Values to be divided by 255.0f - Forgot the reason
	m_PAM.m_colourBegin = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
	m_PAM.m_colourEnd = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };

	m_PAM.m_lifeTime = 1.0f;
	m_PAM.m_rebirth = false;

	m_RM.m_renderingPath = "Models/Cube.a";
}