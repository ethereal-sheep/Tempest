







#include "Particle_System.h"
#include <glm.hpp>

void ParticleSystem::Emitter::Update(const float dt)
{
	static float timeInterval = 0.f;

	// Particles' Behaviour
	for (auto particle : m_particles)
	{
		if (particle.m_lifeRemaining > 0)
		{
			// Calculate the lifeTime remaining
			float lifePercent = particle.m_lifeRemaining / particle.m_lifeTime;

			// Transform Update
			particle.m_position += particle.m_velocity * dt;

			// Size Update
			particle.m_size = glm::mix(m_endSize, m_startSize, lifePercent);

			// Colour Update
			//particle.m_colour = glm::mix(m_colourEnd, m_colourBegin, lifePercent);
			// 
			// Reduce the particle life
			particle.m_lifeRemaining -= dt;
		}
	}

	// TO DO, Emitter Behaviour - One second is per unit time
	if (timeInterval >= 1.f)
	{
		if (m_particles.size() < m_maxParticles)
		{
			// Emit particle
			for (short i = 0; i < m_rateOvertime; ++i)
			{
				Particle newParticle;

				// Initialise the particle data - Not supposed to but too much member variable?
				// 
				// Initial position to be at emitter
				newParticle.m_position = m_position;

				// Lifetime
				newParticle.m_lifeTime = m_lifeTime;
				newParticle.m_lifeRemaining = m_lifeTime;

				// Velocity
				newParticle.m_velocity = m_startVelocity;
				newParticle.m_velocity.x = std::rand() % static_cast<int>(std::abs(m_endVelocity.x - m_startVelocity.x)) * m_velocityVariation.x;
				newParticle.m_velocity.y = std::rand() % static_cast<int>(std::abs(m_endVelocity.y - m_startVelocity.y)) * m_velocityVariation.y;

				// Colour
				newParticle.m_colour = m_colourBegin;
				newParticle.m_colour.x = std::rand() % static_cast<int>(std::abs(m_colourEnd.x - m_colourBegin.x));
				newParticle.m_colour.y = std::rand() % static_cast<int>(std::abs(m_colourEnd.y - m_colourBegin.y));
				newParticle.m_colour.z = std::rand() % static_cast<int>(std::abs(m_colourEnd.z - m_colourBegin.z));

				// Size
				newParticle.m_size = m_startSize;
				newParticle.m_size = std::rand() % static_cast<int>(std::abs(m_endSize - m_startSize)) * m_sizeVariation;

				// Add the particle into the emitter
				m_particles.push_back(newParticle);
			}
		}

		// Reset time interval
		timeInterval = 0.f;
	}
}

void ParticleSystem::Update(const float dt)
{
	// Update all the emitters here
	for (auto emitter : m_emitters)
		emitter->Update(dt);
}