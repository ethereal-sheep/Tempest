

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

	if (m_MM.m_preWarm)
		Emit(m_EM.m_rateOverTime);
}

void Emitter::SelfUpdate()
{
	if (m_MM.m_preWarm)
	{
		Emit(m_EM.m_rateOverTime);
		m_MM.m_preWarm = false;
	}

	// Update Emittor position
	m_GM.m_position += m_GM.m_velocity * m_MM.m_simulationSpeed;

	// Emitter emittion
	if (m_EM.m_spawnCountTimer <= 0.f)
	{
		while (m_EM.m_spawnCountTimer <= 0.f)
		{
			// Emit particle
			Emit(m_EM.m_rateOverTime);
			
			// Ensure if simulation speed is different from the spawnTimeInterval, still spawn right amount
			m_EM.m_spawnCountTimer += m_EM.m_spawnTimeInterval;
		}

		// Reset time interval
		m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	}
	else
		m_EM.m_spawnCountTimer -= m_MM.m_simulationSpeed;

	// Emitter Lifetime update
	if (m_MM.m_duration <= 0.f)
		m_GM.m_active = false;
	else
		m_MM.m_duration -= m_MM.m_simulationSpeed;
}

void Emitter::Update()
{
	SelfUpdate();

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
			particle.m_position += particle.m_velocity * m_MM.m_simulationSpeed;
			//particle.m_rotation += 0.01f * m_MM.m_simulationSpeed;

			// Calculate the lifeTime remaining
			float lifePercent = particle.m_lifeRemaining / particle.m_lifeTime;

			particle.m_size = glm::mix(m_PAM.m_sizeEnd, m_PAM.m_sizeBegin, lifePercent);
			particle.m_colour = glm::mix(m_PAM.m_colourEnd, m_PAM.m_colourBegin, lifePercent);
			particle.m_lifeRemaining -= m_MM.m_simulationSpeed;
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
			particle.m_velocity.x += m_PAM.m_velocityVariation.x; //* (Random::Float() - 0.5f);
			particle.m_velocity.y += m_PAM.m_velocityVariation.y; //* (Random::Float() - 0.5f);

			// Color
			particle.m_colour.r = m_PAM.m_colourBegin.r; //(Random::Float() - 0.5f);
			particle.m_colour.g = m_PAM.m_colourBegin.g; //(Random::Float() - 0.5f);
			particle.m_colour.b = m_PAM.m_colourBegin.b; //(Random::Float() - 0.5f);
			
			particle.m_type = m_RM.m_type;

			// Lifetime
			particle.m_lifeTime = m_PAM.m_lifeTime;
			particle.m_lifeRemaining = m_PAM.m_lifeTime;
			particle.m_size = m_PAM.m_sizeBegin + m_PAM.m_sizeVariation;// *(Random::Float() - 0.5f);

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}
