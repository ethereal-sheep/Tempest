

#include "CharacterTileCharged_Emitter_3D.h"

#include "../Random.h"
#include <algorithm>

#include "Logger/Log.h"

CharacterTileCharged_Emitter_3D::CharacterTileCharged_Emitter_3D()
{
	// Initialise all the waypoints
	for (short i = 0; i < waypoint_Index; ++i)
		m_wayPoints[i] = glm::vec3{ 0.0f, 0.0f, 0.0f };

	// Values for Emission Module
	m_GM.m_active = true;

	m_MM.m_duration = 60.0f;
	
	m_MM.m_preWarm = true;
	Emitter_3D::UpdateMaxParticle(1000);

	//m_EM.m_spawnTimeInterval = 0.016f;
	//m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	//m_EM.m_rateOverTime = 2;

	m_EM.m_burstInterval = 0.016f;
	m_EM.m_burstCount = 2;
	m_EM.m_burstCycle = 56;

	// Particle Architype values - without consideration for default ctor
	m_PAM.m_velocityStart = glm::vec3{ 0.f, 1.f, 0.0f };
	m_PAM.m_velocityEnd = glm::vec3{ 0.f, 1.f, 0.0f };
	m_PAM.m_velocityVariation = glm::vec3{ 0.0f, 6.0f, 0.0f };

	m_PAM.m_scaleBegin = glm::vec3{ 0.01f, 0.01f, 0.01f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.02f, 0.02f, 0.02f };

	// Note - Values to be divided by 255.0f - Forgot the reason
	m_PAM.m_colourBegin = glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f };
	m_PAM.m_colourEnd = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };

	m_PAM.m_lifeTime = 0.5f;
	m_PAM.m_rebirth = false;

	m_RM.m_renderingPath = "Models/Cube.a";
	m_RM.m_emissiveLighting = true;
}

void CharacterTileCharged_Emitter_3D::AssignWaypoint(const std::array<glm::vec3, waypoint_Index> newWaypoints)
{
	// Initialise all the waypoints
	for (short i = 0; i < waypoint_Index; ++i)
		m_wayPoints[i] = newWaypoints[i];
}

void CharacterTileCharged_Emitter_3D::Emit(const int particleAmount)
{
	static int incremental_Amount = 100;
	// Emit only if enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initailisation of the particle
			Particle_3D particle;
			ParticleSetUp(particle);

			// The random point to emit from
			particle.m_position = RandomPoint();

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}

	// Each time emit increase the speed by abit
	if (incremental_Amount++ < 100)
	{
		m_PAM.m_velocityStart *= 1.5f;
		m_PAM.m_velocityEnd *= 1.5f;
		m_PAM.m_velocityVariation *= 1.5f;
		m_PAM.m_lifeTime *= 0.5f;
	}
}

glm::vec3 CharacterTileCharged_Emitter_3D::RandomPoint()
{
	// Random points to emit the particle
	int first_Waypoint = std::rand() % waypoint_Index;
	int second_Waypoint = 0;

	/*
	* 0 - Min / Bottom Left
	* 1 - Bottom Right
	* 2 - Top Left
	* 3 - Max / Top Right
	*/

	// Last waypoint is chosen
	if (first_Waypoint == waypoint_Index - 1)
	{
		int rand = static_cast<int>(std::floor(Random::Float()));

		if (rand)
			second_Waypoint = first_Waypoint - 1;
		else
			second_Waypoint = 0;

		std::swap(first_Waypoint, second_Waypoint);
	}
	else
		second_Waypoint = first_Waypoint + 1;	

	//LOG_INFO("firstIndex: {0}", first_Waypoint);
	//LOG_INFO("secondIndex: {0}", second_Waypoint);

	// Calculate & Pick a random point from the line
	glm::vec3 vecDistance = m_wayPoints[second_Waypoint] - m_wayPoints[first_Waypoint];
	//vecDistance.x = std::abs(static_cast<int>(vecDistance.x));
	//vecDistance.z = std::abs(static_cast<int>(vecDistance.z));

	//LOG_INFO("vecDistance_X: {0}", vecDistance.x);
	//LOG_INFO("vecDistance_Z: {0}", vecDistance.z);

	// Random point on the lin
	glm::vec3 randomPoint = Random::Float() * vecDistance;

	// Pick a random point
	glm::vec3 result_Point = m_wayPoints[first_Waypoint] + randomPoint;

	//LOG_INFO("Random Point: {0}, {1}, {2}", result_Point.x, result_Point.y, result_Point.z);

	return result_Point;
}