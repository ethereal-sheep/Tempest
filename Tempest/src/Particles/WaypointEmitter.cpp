

#include "WaypointEmitter.h"

void WaypointEmitter::SelfUpdate()
{
	//if (m_preWarm)
	//{
	//	Emit(m_EM.m_rateOverTime);
	//	m_preWarm = false;
	//}

	// Update Emittor position
	if (m_wayPoints.size())
	{
		// Move in way point position
		if (m_GM.m_position != m_wayPoints[m_wayPointIndex])
		{
			if (m_recalculateVelocity)
			{
				m_GM.m_velocity = m_wayPoints[m_wayPointIndex] - m_GM.m_position;
				m_recalculateVelocity = false;
			}

			//Move towards way point
			m_GM.m_position += m_GM.m_velocity * m_MM.m_simulationSpeed;

			auto DistanceCalculation = [](glm::vec2 endPos, glm::vec2 startPos)
			{
				glm::vec2 distanceVec = endPos - startPos;
				float distanceSquared = (distanceVec.x * distanceVec.x) + (distanceVec.y * distanceVec.y);

				return distanceSquared;
			};

			// Check how near it is
			float distanceSquared = DistanceCalculation(m_wayPoints[m_wayPointIndex], m_GM.m_position);

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

	// Emitter emittion
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