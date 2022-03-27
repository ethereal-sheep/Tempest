
#include "Unit_Turn_IndicatorEmitter_3D.h"
#include "Logger/Log.h"


Unit_Turn_IndicatorEmitter_3D::Unit_Turn_IndicatorEmitter_3D()
	: Emitter_3D()
	, m_wayPointIndex{ 0 }
	, m_recalculateVelocity{ true }
{
	m_GM.m_velocity.x = 0.0f;
	m_MM.m_duration = 1000.0f;
	m_GM.m_active = true;
	m_MM.m_preWarm = true;
	m_MM.m_simulationSpeed = 0.016f;

	m_EM.m_spawnTimeInterval = 0.016f;
	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	m_EM.m_rateOverTime = 1;
	Emitter_3D::UpdateMaxParticle(1000);

	m_wayPointIndex = 0;
	m_recalculateVelocity = true;

	m_PAM.m_velocityStart = glm::vec3{ 0.f, 0.1f, 0.0f };
	m_PAM.m_velocityEnd = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	m_PAM.m_scaleBegin = glm::vec3{ 0.05f, 0.05f, 0.05f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	m_PAM.m_lifeTime = 0.3f;
	m_RM.m_renderingPath = "Models/Cube.a";
}

void Unit_Turn_IndicatorEmitter_3D::Update(const float dt)
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
			//m_GM.m_position += m_GM.m_velocity * m_MM.m_simulationSpeed;
			m_GM.m_position += m_GM.m_velocity * dt;

			// 0.01 pixels difference
			if (distanceSquared <= 0.01f)
			{
				m_GM.m_position = m_wayPoints[m_wayPointIndex];
				m_wayPointIndex = (m_wayPointIndex + 1) % static_cast<int>(m_wayPoints.size());
				m_recalculateVelocity = true;
			}
		}
		else
		{
			m_GM.m_position = m_wayPoints[m_wayPointIndex];
			m_wayPointIndex = (m_wayPointIndex + 1) % static_cast<int>(m_wayPoints.size());
			m_recalculateVelocity = true;
		}

		// Reset waypoint
		//if (m_wayPointIndex >= m_wayPoints.size())
		//	m_wayPointIndex = 0;
	}
	else
		//m_GM.m_position += m_GM.m_velocity * m_MM.m_simulationSpeed;
		m_GM.m_position += m_GM.m_velocity * dt;

	// Emitter_3D emittion
	if (m_EM.m_spawnCountTimer <= 0.f)
	{
		// If can spawn even more
		while (m_EM.m_spawnCountTimer <= 0.f)
		{
			//LOG_INFO("Tile Way Point Emit");

			//Emit particle
			Emit(m_EM.m_rateOverTime);

			// Ensure if over lag, still spawn right amount
			m_EM.m_spawnCountTimer += m_EM.m_spawnTimeInterval;
		}

		// Reset time interval
		m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	}
	else
		//m_EM.m_spawnCountTimer -= m_MM.m_simulationSpeed;
		m_EM.m_spawnCountTimer -= dt;

	if (m_MM.m_duration <= 0.f)
		m_GM.m_active = false;
	else
		//m_MM.m_duration -= m_MM.m_simulationSpeed;
		m_MM.m_duration -= dt;

	ParticleUpdate(dt);
}

void Unit_Turn_IndicatorEmitter_3D::UpdateWaypoints(glm::vec3 tileOriginPosition)
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

	// Log Waypoints
	//LOG_INFO("Right BTM:  x: {0}, y:{1}, z{2}", wp_RightBtm.x, wp_RightBtm.y, wp_RightBtm.z);
	//LOG_INFO("Right TOP:  x: {0}, y:{1}, z{2}", wp_RightTop.x, wp_RightTop.y, wp_RightTop.z);

	//LOG_INFO("Left TOP:  x: {0}, y:{1}, z{2}", wp_LeftTop.x, wp_LeftTop.y, wp_LeftTop.z);
	//LOG_INFO("Left BTM:  x: {0}, y:{1}, z{2}", wp_LeftBtm.x, wp_LeftBtm.y, wp_LeftBtm.z);

	// Clear all alive particles
	ClearAllParticles();
}