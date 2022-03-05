

#include "UnitTrailEmitter_3D.h"

UnitTrailEmitter_3D::UnitTrailEmitter_3D()
	: m_waypointsCompleted{ false }
	, m_recalculateVelocity{ true }
	, m_wayPointIndex { 0 }
{}


void UnitTrailEmitter_3D::SelfUpdate()
{
	//if (m_MM.m_preWarm)
	//{
	//	Emit(m_EM.m_rateOverTime);
	//	m_MM.m_preWarm = false;
	//}

	//// Update Emittor position
	//if (!m_waypointsCompleted && m_wayPoints.size())
	//{
	//	// Move in way point position
	//	if (m_GM.m_position != m_wayPoints[m_wayPointIndex])
	//	{

	//		auto DistanceCalculation = [](glm::vec3 endPos, glm::vec3 startPos)
	//		{
	//			glm::vec3 distanceVec = endPos - startPos;
	//			float distanceSquared = (distanceVec.x * distanceVec.x) + (distanceVec.y * distanceVec.y) + (distanceVec.z * distanceVec.z);

	//			return distanceSquared;
	//		};

	//		// AssumeTileBased
	//		if (m_recalculateVelocity)
	//		{
	//			m_GM.m_velocity = m_wayPoints[m_wayPointIndex] - m_GM.m_position;

	//			if (m_GM.m_velocity.x != 0.f)
	//			{
	//				m_GM.m_velocity.x /= std::abs(m_GM.m_velocity.x);
	//				m_GM.m_velocity.x *= 1.0f;
	//			}

	//			if (m_GM.m_velocity.z != 0.f)
	//			{
	//				m_GM.m_velocity.z /= std::abs(m_GM.m_velocity.z);
	//				m_GM.m_velocity.z *= 1.0f;
	//			}

	//			m_recalculateVelocity = false;
	//		}
	//		// Check how near it is
	//		float distanceSquared = DistanceCalculation(m_wayPoints[m_wayPointIndex], m_GM.m_position);

	//		//Move towards waypoint
	//		m_GM.m_position += m_GM.m_velocity * m_MM.m_simulationSpeed;

	//		// 10 pixels difference
	//		if (distanceSquared <= 1.0f)
	//		{
	//			m_GM.m_position = m_wayPoints[m_wayPointIndex];
	//			++m_wayPointIndex;
	//			m_recalculateVelocity = true;
	//		}
	//	}

	//	// Reset waypoint
	//	if (m_wayPointIndex >= m_wayPoints.size())
	//		m_waypointsCompleted = true;
	//}
	//else
	//	Reset();

	//// Emitter_3D emittion
	//if (m_EM.m_spawnCountTimer <= 0.f)
	//{
	//	/*while (m_EM.m_spawnCountTimer <= 0.f)
	//	{*/
	//	// Emit particle
	//	Emit(m_EM.m_rateOverTime);

	//	//	// Ensure if over lag, still spawn right amount
	//	//	m_EM.m_spawnCountTimer += m_MM.m_simulationSpeed;
	//	//}

	//	// Reset time interval
	//	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	//}
	//else
	//	m_EM.m_spawnCountTimer -= m_MM.m_simulationSpeed;

	//if (m_MM.m_duration <= 0.f)
	//	m_GM.m_active = false;
	//else
	//	m_MM.m_duration -= m_MM.m_simulationSpeed;
}

void UnitTrailEmitter_3D::Emit(const int particleAmount)
{
	// Emit only if there is enough particle and waypoint is reached
	if (!m_waypointsCompleted && particleAmount > 0 && m_available_ParticleSlots.size() > 0)
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