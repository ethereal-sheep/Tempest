

#include "Emitter.h"

#include "Random.h"
#include "Util.h"
#include <numbers>



Emitter::Emitter()
	: m_colourBegin{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f }
	, m_colourEnd{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f }
	, m_sizeBegin{ 50.0f }
	, m_sizeVariation{ 0.3f }
	, m_sizeEnd{ 0.0f }
	, m_lifeTime{ 5.0f }
	, m_startVelocity{ 100.0f, 0.0f }
	, m_endVelocity{ 0.0f, 0.0f }
	, m_velocityVariation{ 3.0f, 1.0f }
	, m_position{ 0.0f, 0.0f }
	//, m_loop{ false }
	, m_rateOvertime{ 0 }
	, m_maxParticles{ 1000 } // Hard coded for now
	, m_particleIndex{ 0 }
	, m_type{ ParticleType::Circle }
{
	m_particles.resize(m_maxParticles);

	// Store all available slots
	for (short i = 0; i < m_maxParticles; ++i)
		m_available_ParticleSlots.push(i);
}

void Emitter::Update(const float dt)
{
	static float timeInterval = 0.f;

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
			particle.m_size = glm::mix(m_sizeEnd, m_sizeBegin, lifePercent);

			// Colour Update
			particle.m_colour = glm::mix(m_colourEnd, m_colourBegin, lifePercent);

			// Reduce the particle life
			particle.m_lifeRemaining -= dt;
		}
	}

	// TO DO, Emitter Behaviour - One second is per unit time
	if (timeInterval >= 1.f)
	{
		// Emit particle
		Emit(m_rateOvertime);

		// Reset time interval
		timeInterval = 0.f;
	}
	else
		timeInterval += dt;
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
			particle.m_velocity.x += m_velocityVariation.x * (Random::Float() - 0.5f);
			particle.m_velocity.y += m_velocityVariation.y * (Random::Float() - 0.5f);

			// Color
			particle.m_colour.r = (Random::Float() - 0.5f);
			particle.m_colour.g = (Random::Float() - 0.5f);
			particle.m_colour.b = (Random::Float() - 0.5f);

			particle.m_lifeTime = m_lifeTime;
			particle.m_lifeRemaining = m_lifeTime;
			particle.m_size = m_sizeBegin + m_sizeVariation * (Random::Float() - 0.5f);


			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}
