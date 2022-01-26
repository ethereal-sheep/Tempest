

// Main Include
#include "Emitter.h"

// Additional Includes
#include "Random.h"
#include "Util.h"
#include <numbers>

Emitter::Emitter()
	: m_wayPointIndex {0}
	, m_recalculateVelocity {true}
{
	m_particles.resize(m_MM.m_maxParticles);

	// Store all available slots
	for (short i = 0; i < m_MM.m_maxParticles; ++i)
		m_available_ParticleSlots.push(i);

	//if (m_preWarm)
	//	Emit(m_EM.m_rateOverTime);
}

void Emitter::SelfUpdate(const float dt)
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
			m_GM.m_position += m_GM.m_velocity * dt;

			auto DistanceCalculation = [](glm::vec2 endPos, glm::vec2 startPos)
			{
				glm::vec2 distanceVec = endPos - startPos;
				float distanceSquared = (distanceVec.x * distanceVec.x) + (distanceVec.y * distanceVec.y);

				return distanceSquared;
			};

			// Check how near it is
			float distanceSquared = DistanceCalculation(m_wayPoints[m_wayPointIndex], m_GM.m_position);

			/*LOG_INFO("distanceSquared");
			LOG_INFO(distanceSquared);*/

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
		m_GM.m_position += m_GM.m_velocity * dt;

	// Emitter emittion
	if (m_EM.m_spawnCountTimer <= 0.f)
	{
		/*while (m_EM.m_spawnCountTimer <= 0.f)
		{*/
			// Emit particle
			Emit(m_EM.m_rateOverTime);

		//	// Ensure if over lag, still spawn right amount
		//	m_EM.m_spawnCountTimer += dt;
		//}

		// Reset time interval
		m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	}
	else
		m_EM.m_spawnCountTimer -= dt;

	if (m_MM.m_duration <= 0.f)
		m_GM.m_active = false;
	else
		m_MM.m_duration -= dt;

}

void Emitter::Update(const float dt)
{
	SelfUpdate(dt);

	// Particles Behaviour
	for (short i = 0; i < m_particles.size(); ++i)
	{
		auto& particle = m_particles[i];

		if (!particle.m_isActive)
			continue;
		else if (particle.m_lifeRemaining <= 0)
		{
			particle.m_isActive = false;

			// To be reused
			m_available_ParticleSlots.push(i);
		}
		else
		{
			particle.m_position += particle.m_velocity * dt;
			//particle.m_rotation += 0.01f * dt;

			// Calculate the lifeTime remaining
			float lifePercent = particle.m_lifeRemaining / particle.m_lifeTime;

			particle.m_size = glm::mix(m_PAM.m_sizeEnd, m_PAM.m_sizeBegin, lifePercent);
			particle.m_colour = glm::mix(m_PAM.m_colourEnd, m_PAM.m_colourBegin, lifePercent);
			particle.m_lifeRemaining -= dt;
		}
	}
}

void Emitter::Emit(const int particleAmount)
{
	// Emit only if enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initailisation of the particle
			Particle particle;

			particle.m_position = m_GM.m_position;
			particle.m_isActive = true;
			//particle.m_rotation = Random::Float() * 2.0f * std::numbers::pi;

			// Velocity
			particle.m_velocity = m_PAM.m_startVelocity;
			particle.m_velocity.x += m_PAM.m_velocityVariation.x * (Random::Float() - 0.5f);
			particle.m_velocity.y += m_PAM.m_velocityVariation.y * (Random::Float() - 0.5f);

			// Color
			particle.m_colour.r = (Random::Float() - 0.5f);
			particle.m_colour.g = (Random::Float() - 0.5f);
			particle.m_colour.b = (Random::Float() - 0.5f);
			
			particle.m_type = m_RM.m_type;

			// Lifetime
			particle.m_lifeTime = m_PAM.m_lifeTime;
			particle.m_lifeRemaining = m_PAM.m_lifeTime;
			particle.m_size = m_PAM.m_sizeBegin + m_PAM.m_sizeVariation * (Random::Float() - 0.5f);

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}
