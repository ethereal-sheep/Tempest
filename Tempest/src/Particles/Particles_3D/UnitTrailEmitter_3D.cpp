

#include "UnitTrailEmitter_3D.h"

//UnitTrailEmitter_3D::UnitTrailEmitter_3D()
//	: m_waypointsCompleted{ false }
//	, m_recalculateVelocity{ true }
//	, m_wayPointIndex { 0 }
//{}


void UnitTrailEmitter_3D::SelfUpdate(const float dt)
{
	
}

void UnitTrailEmitter_3D::Emit(const int particleAmount)
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

void UnitTrailEmitter_3D::ParticleSetUp(Particle_3D& particle)
{
	Emitter_3D::ParticleSetUp(particle);

	// To have the effect from one place to another place
	particle.m_rebirth = true;
}

void UnitTrailEmitter_3D::Reset()
{
	/*m_GM.m_position = m_startPoint;
	m_waypointReached = false;
	m_recalculateVelocity = true;*/
	//m_EM.m_rateOverTime = 0;

	m_MM.m_duration = 0.f;
}