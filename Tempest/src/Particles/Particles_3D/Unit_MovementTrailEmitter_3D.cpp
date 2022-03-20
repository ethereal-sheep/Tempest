

#include "Unit_MovementTrailEmitter_3D.h"



Unit_MovementTrailEmitter_3D::Unit_MovementTrailEmitter_3D()
{
	m_GM.m_active = true;

	m_MM.m_duration = 1000.0f;
	m_MM.m_preWarm = true;
	m_MM.m_simulationSpeed = 0.016f;
	m_MM.m_maxParticles = 1;

	m_EM.m_spawnTimeInterval = 0.08f;
	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	m_EM.m_rateOverTime = 1;

	m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	m_PAM.m_scaleBegin = glm::vec3{ 0.05f, 0.05f, 0.05f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	m_PAM.m_lifeTime = 1.0f;
	m_RM.m_renderingPath = "Models/Cube.a";
}

void Unit_MovementTrailEmitter_3D::Update([[maybe_unused]] const float dt)
{
	
}

void Unit_MovementTrailEmitter_3D::Emit(const int particleAmount)
{
	// Emit only if there is enough particle and waypoint is reached
	//if (!m_waypointsCompleted && particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initialisation of the particle
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

void Unit_MovementTrailEmitter_3D::ParticleSetUp(Particle_3D& particle)
{
	Emitter_3D::ParticleSetUp(particle);
}

void Unit_MovementTrailEmitter_3D::Reset()
{
	/*m_GM.m_position = m_startPoint;
	m_waypointReached = false;
	m_recalculateVelocity = true;*/
	//m_EM.m_rateOverTime = 0;

	m_MM.m_duration = 0.f;
}