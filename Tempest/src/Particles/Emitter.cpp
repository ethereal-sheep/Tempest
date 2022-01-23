

// Main Include
#include "Emitter.h"

// Additional Includes
#include "Random.h"
#include "Util.h"
#include <numbers>

ParticleArchetype::ParticleArchetype()
	: m_startVelocity{ 100.0f, 0.0f }
	, m_endVelocity{ 0.0f, 0.0f }
	, m_velocityVariation{ 3.0f, 1.0f }

	, m_sizeBegin{ 50.0f }
	, m_sizeEnd{ 0.0f }
	, m_sizeVariation{ 0.3f }

	, m_colourBegin{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f }
	, m_colourEnd{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f }
	//, m_colourVariation { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f }

	, m_lifeTime{ 5.0f }
	//, m_lifeVariation{ 5.0f }

	, m_type{ ParticleType::Circle }
{}

Emitter::Emitter()
	: m_position{ 0.0f, 0.0f }
	, m_lifeTime{ 10.0f }
	, m_active { true }
	, m_preWarm { true }
	//, m_loop{ false }
	, m_countTimer{0.0f}
	, m_spawnTimeInterval{ 5.0f }
	, m_spawnCount{ 1 }
	, m_maxParticles{ 1000 } // Hard coded for now
{
	m_particles.resize(m_maxParticles);

	// Store all available slots
	for (short i = 0; i < m_maxParticles; ++i)
		m_available_ParticleSlots.push(i);

	//if (m_preWarm)
	//	Emit(m_spawnCount);
}

void Emitter::Update(const float dt)
{
	if (m_preWarm)
	{
		Emit(m_spawnCount);
		m_preWarm = false;
	}

	// Emitter emittion
	if (m_countTimer <= 0.f)
	{
		// Emit particle
		Emit(m_spawnCount);

		// Reset time interval
		m_countTimer = m_spawnTimeInterval;
	}
	else
		m_countTimer -= dt;

	if (m_lifeTime <= 0.f)
		m_active = false;
	else
		m_lifeTime -= dt;

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

	/*		LOG_INFO("Particle Position X");
			LOG_INFO(particle.m_position.x);
			LOG_INFO("Particle Position Y");
			LOG_INFO(particle.m_position.y);
			LOG_INFO("Particle Velocity X");
			LOG_INFO(particle.m_velocity.x);
			LOG_INFO("Particle Velocity Y");
			LOG_INFO(particle.m_velocity.y);*/

			// Calculate the lifeTime remaining
			float lifePercent = particle.m_lifeRemaining / particle.m_lifeTime;

			// Size Update
			particle.m_size = glm::mix(m_PA.m_sizeEnd, m_PA.m_sizeBegin, lifePercent);

			// Colour Update
			particle.m_colour = glm::mix(m_PA.m_colourEnd, m_PA.m_colourBegin, lifePercent);

			// Reduce the particle life
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

			particle.m_position = m_position;
			particle.m_isActive = true;
			//particle.m_rotation = Random::Float() * 2.0f * std::numbers::pi;

			// Velocity
			particle.m_velocity.x += m_PA.m_velocityVariation.x * (Random::Float() - 0.5f);
			particle.m_velocity.y += m_PA.m_velocityVariation.y * (Random::Float() - 0.5f);

			// Color
			particle.m_colour.r = (Random::Float() - 0.5f);
			particle.m_colour.g = (Random::Float() - 0.5f);
			particle.m_colour.b = (Random::Float() - 0.5f);
			
			particle.m_type = m_PA.m_type;

			// Lifetime
			particle.m_lifeTime = m_PA.m_lifeTime;
			particle.m_lifeRemaining = m_PA.m_lifeTime;
			particle.m_size = m_PA.m_sizeBegin + m_PA.m_sizeVariation * (Random::Float() - 0.5f);

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}
