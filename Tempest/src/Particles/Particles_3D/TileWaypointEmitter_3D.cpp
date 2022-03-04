
#include "TileWaypointEmitter_3D.h"
#include "Logger/Log.h"

TileWaypointEmitter_3D::TileWaypointEmitter_3D()
	: Emitter_3D()
	, m_wayPointIndex{ 0 }
	, m_recalculateVelocity{ true }
{}

void TileWaypointEmitter_3D::SelfUpdate()
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
			auto MagnitudeSquared = [](glm::vec3 distanceVec)
			{
				return (distanceVec.x * distanceVec.x) + (distanceVec.y * distanceVec.y) + (distanceVec.z * distanceVec.z);
			};

			auto DistanceCalculation = [&](glm::vec3 endPos, glm::vec3 startPos)
			{
				glm::vec3 distanceVec = endPos - startPos;

				return MagnitudeSquared(distanceVec);
			};

			// AssumeTileBased
			if (m_recalculateVelocity)
			{
				m_GM.m_velocity = m_wayPoints[m_wayPointIndex] - m_GM.m_position;

				if (m_GM.m_velocity.x != 0.f)
				{
					m_GM.m_velocity.x /= std::abs(m_GM.m_velocity.x);
					//m_GM.m_velocity.x /= MagnitudeSquared(m_GM.m_velocity);
					m_GM.m_velocity.x *= 10.0f;
				}

				if (m_GM.m_velocity.z != 0.f)
				{
					m_GM.m_velocity.z /= std::abs(m_GM.m_velocity.z);
					//m_GM.m_velocity.z /= MagnitudeSquared(m_GM.m_velocity);
					m_GM.m_velocity.z *= 10.0f;
				}

				m_recalculateVelocity = false;
			}
			// Check how near it is
			float distanceSquared = DistanceCalculation(m_wayPoints[m_wayPointIndex], m_GM.m_position);

			//Move towards waypoint
			m_GM.m_position += m_GM.m_velocity * m_MM.m_simulationSpeed;

			// 0.01 pixels difference
			if (distanceSquared <= 0.01f)
			{
				m_GM.m_position = m_wayPoints[m_wayPointIndex];
				m_wayPointIndex = (m_wayPointIndex + 1) % static_cast<int>(m_wayPoints.size());
				m_recalculateVelocity = true;
			}
		}

		// Reset waypoint
		//if (m_wayPointIndex >= m_wayPoints.size())
		//	m_wayPointIndex = 0;
	}
	else
		m_GM.m_position += m_GM.m_velocity * m_MM.m_simulationSpeed;

	// Emitter_3D emittion
	if (m_EM.m_spawnCountTimer <= 0.f)
	{
		// If can spawn even more
		while (m_EM.m_spawnCountTimer <= 0.f)
		{
			//Emit particle
			Emit(m_EM.m_rateOverTime);

			// Ensure if over lag, still spawn right amount
			m_EM.m_spawnCountTimer += m_EM.m_spawnTimeInterval;
		}

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

void TileWaypointEmitter_3D::UpdateWaypoints(glm::vec3 tileOriginPosition)
{
	m_GM.m_position = tileOriginPosition;
	m_wayPoints.clear();
	m_wayPointIndex = 0;
	m_recalculateVelocity = true;

	glm::vec3 wp_LeftBtm = tileOriginPosition;
	wp_LeftBtm = glm::vec3{ tileOriginPosition.x - 0.5f, tileOriginPosition.y + 0.1f, tileOriginPosition.z - 0.5f };

	glm::vec3 wp_RightBtm = wp_LeftBtm;
	wp_RightBtm.x += 1.0f;

	glm::vec3 wp_RightTop = wp_RightBtm;
	wp_RightTop.z += 1.0f;

	glm::vec3 wp_LeftTop = wp_RightTop;
	wp_LeftTop.x -= 1.0f;

	// Add the waypoints
	m_wayPoints.push_back(wp_RightBtm);
	m_wayPoints.push_back(wp_RightTop);
	m_wayPoints.push_back(wp_LeftTop);
	m_wayPoints.push_back(wp_LeftBtm);

	// Clear all alive particles
	ClearAllParticles();
}