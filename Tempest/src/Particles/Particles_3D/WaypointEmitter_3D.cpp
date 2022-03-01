
#include "WaypointEmitter_3D.h"

WaypointEmitter_3D::WaypointEmitter_3D()
	: Emitter_3D()
	, m_wayPointIndex{ 0 }
	, m_recalculateVelocity{ true }
{}

void WaypointEmitter_3D::SelfUpdate()
{
	if (m_MM.m_preWarm)
	{
		Emit(m_EM.m_rateOverTime);
		m_MM.m_preWarm = false;
	}

	// Update Emittor position
	if (m_wayPoints.size())
	{
		// Move in way point position
		if (m_GM.m_position != m_wayPoints[m_wayPointIndex])
		{

			auto DistanceCalculation = [](glm::vec3 endPos, glm::vec3 startPos)
			{
				glm::vec3 distanceVec = endPos - startPos;
				float distanceSquared = (distanceVec.x * distanceVec.x) + (distanceVec.y * distanceVec.y) + (distanceVec.z * distanceVec.z);

				return distanceSquared;
			};

			if (m_recalculateVelocity)
			{
				m_GM.m_velocity = m_wayPoints[m_wayPointIndex] - m_GM.m_position;

				if (m_GM.m_velocity.x != 0.f)
				{
					m_GM.m_velocity.x /= std::abs(m_GM.m_velocity.x);
					m_GM.m_velocity.x *= 200.0f;
				}

				if (m_GM.m_velocity.z != 0.f)
				{
					m_GM.m_velocity.z /= std::abs(m_GM.m_velocity.z);
					m_GM.m_velocity.z *= 200.0f;
				}

				m_recalculateVelocity = false;
			}
			// Check how near it is
			float distanceSquared = DistanceCalculation(m_wayPoints[m_wayPointIndex], m_GM.m_position);

			//Move towards waypoint
			m_GM.m_position += m_GM.m_velocity * m_MM.m_simulationSpeed;

			// 10 pixels difference
			if (distanceSquared <= 20.0f)
			{
				m_GM.m_position = m_wayPoints[m_wayPointIndex];
				++m_wayPointIndex;
				m_recalculateVelocity = true;
			}
		}

		// Reset waypoint
		if (m_wayPointIndex >= m_wayPoints.size())
			m_wayPointIndex = 0;
	}
	else
		m_GM.m_position += m_GM.m_velocity * m_MM.m_simulationSpeed;

	// Emitter_2D emittion
	if (m_EM.m_spawnCountTimer <= 0.f)
	{
		/*while (m_EM.m_spawnCountTimer <= 0.f)
		{*/
		// Emit particle
		Emit(m_EM.m_rateOverTime);

		//	// Ensure if over lag, still spawn right amount
		//	m_EM.m_spawnCountTimer += m_MM.m_simulationSpeed;
		//}

		// Reset time interval
		m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	}
	else
		m_EM.m_spawnCountTimer -= m_MM.m_simulationSpeed;

	if (m_MM.m_duration <= 0.f)
		m_GM.m_active = false;
	else
		m_MM.m_duration -= m_MM.m_simulationSpeed;

}