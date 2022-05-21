


#include "Interactive_DoorParticle_3D.h"

#include "../Random.h"

#include <algorithm> //std::clamp

#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <numbers>		/* numbers */

Interactive_DoorParticle_3D::Interactive_DoorParticle_3D()
{
	m_GM.m_active = true;

	m_MM.m_duration = 1000000.f;
	m_MM.m_preWarm = true;
	Emitter_3D::UpdateMaxParticle(30);

	m_EM.m_spawnTimeInterval = 0.1f;
	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	m_EM.m_rateOverTime = 1;
	
	// Particle Architype values - without consideration for default ctor
	m_PAM.m_velocityStart = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityEnd = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityVariation = glm::vec3{ 0.0f, 3.0f, 0.0f };

	m_PAM.m_scaleBegin = glm::vec3{ 0.0015f, 0.0015f, 0.0015f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	//m_PAM.m_scaleVariation = glm::vec3{ 0.02f, 0.02f, 0.02f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.f, 0.f, 0.f };

	// Note - Values to be divided by 255.0f - Forgot the reason
	m_PAM.m_colourBegin = glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f };
	m_PAM.m_colourEnd = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };

	m_PAM.m_lifeTime = 1.0f;
	m_PAM.m_rebirth = false;

	//m_RM.m_renderingPath = "Models/Cube.a";
	m_RM.m_renderingPath = "Models/Sphere.a";
	m_RM.m_emissiveLighting = true;
}
