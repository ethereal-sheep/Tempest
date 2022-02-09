

#include "WaypointEmitter.h"

#include "Logger/Log.h"

WaypointEmitter::WaypointEmitter()
	: Emitter()
	, m_initialDistanceSquared {0.0f}
	, m_startVelocity { glm::vec2{0.0f, 0.0f}}
	, m_endVelocity{ glm::vec2{0.0f, 0.0f}}
{}

void WaypointEmitter::SelfUpdate()
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

			auto DistanceCalculation = [](glm::vec2 endPos, glm::vec2 startPos)
			{
				glm::vec2 distanceVec = endPos - startPos;
				float distanceSquared = (distanceVec.x * distanceVec.x) + (distanceVec.y * distanceVec.y);

				return distanceSquared;
			};

			if (m_recalculateVelocity)
			{
				m_GM.m_velocity = m_wayPoints[m_wayPointIndex] - m_GM.m_position;

			/*	LOG_INFO("wayPoint position X : {0}", m_wayPoints[m_wayPointIndex].x);
				LOG_INFO("wayPoint position Y : {0}", m_wayPoints[m_wayPointIndex].y);

				LOG_INFO("Position X : {0}", m_GM.m_position.x);
				LOG_INFO("Position Y : {0}", m_GM.m_position.y);*/

				//m_startVelocity = m_wayPoints[m_wayPointIndex] - m_GM.m_position;
				
				// 20% of the initial velocity
				//m_endVelocity = m_startVelocity * 0.5f;

				//m_initialDistanceSquared = DistanceCalculation(m_wayPoints[m_wayPointIndex], m_GM.m_position);

				// Reducing velocity speed - Flawed Normalisation
		/*		LOG_INFO("Before control velocity X : {0}", m_GM.m_velocity.x);
				LOG_INFO("Before control velocity Y : {0}", m_GM.m_velocity.y);*/

				if (m_GM.m_velocity.x != 0.f)
				{
					m_GM.m_velocity.x /= std::abs(m_GM.m_velocity.x);
					m_GM.m_velocity.x *= 200.0f;
				}

				if (m_GM.m_velocity.y != 0.f)
				{
					m_GM.m_velocity.y /= std::abs(m_GM.m_velocity.y);
					m_GM.m_velocity.y *= 200.0f;
				}

				/*LOG_INFO("After control velocity X : {0}", m_GM.m_velocity.x);
				LOG_INFO("After control velocity Y : {0}", m_GM.m_velocity.y);*/

				m_recalculateVelocity = false;
			}
			// Check how near it is
			float distanceSquared = DistanceCalculation(m_wayPoints[m_wayPointIndex], m_GM.m_position);

			// Travel Progression Velocity
			//float travelProgression = (m_initialDistanceSquared - distanceSquared) / m_initialDistanceSquared;
			//m_GM.m_velocity = glm::mix(m_startVelocity, m_endVelocity, travelProgression);

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