#include "Particle_System.h"
#include "Random.h"

#include "Util.h"

#include <numbers>

ParticleSystem::Particle::Particle()
	: m_position{ 0.0f, 0.0f }
	, m_velocity{ 0.0f, 0.0f }
	, m_rotation{ 0.0f }
	, m_colour{ 0.0f, 0.0f, 0.0f, 0.0f }
	, m_size{ 1.0f }
	, m_lifeTime{ 1.0f }
	, m_lifeRemaining{ 1.0f }
	, m_isActive {false}
{}

ParticleSystem::Particle::Particle(glm::vec2 pos, glm::vec2 vec, glm::vec4 colour, float size, float lifeTime, bool isActive)
	: m_position{ pos }
	, m_velocity{ vec }
	, m_rotation{ 0.0f }
	, m_colour { colour }
	, m_size {size}
	, m_lifeTime {lifeTime}
	, m_lifeRemaining {lifeTime}
	, m_isActive {isActive}
{}

ParticleSystem::Emitter::Emitter()
	: m_colourBegin { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f }
	, m_colourEnd { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f }
	, m_sizeBegin{ 0.5f }
	, m_sizeVariation{ 0.3f }
	, m_sizeEnd{ 0.0f }
	, m_lifeTime{ 1.0 }
	, m_startVelocity { 0.0f, 0.0f }
	, m_endVelocity { 0.0f, 0.0f }
	, m_velocityVariation { 3.0f, 1.0f }
	, m_position { 0.0f, 0.0f }
	, m_loop{ false }
	, m_rateOvertime{0.0f}
	, m_maxParticles {1000} // Hard coded for now
{
	//m_particles.resize(m_maxParticles); // Testing for now
}

void ParticleSystem::Emitter::Update(const float dt)
{
	LOG_INFO("Emitter Update");

	static float timeInterval = 0.f;

	// Particles' Behaviour
	for (auto particle : m_particles)
	{
		if (!particle.m_isActive)
			continue;

		if (particle.m_lifeRemaining <= 0)
		{
			particle.m_isActive = false;
			continue;
		}
		else
		{
			// Transform Update
			particle.m_position += particle.m_velocity * dt;
			particle.m_rotation += 0.01f * dt;

			// Calculate the lifeTime remaining
			float lifePercent = particle.m_lifeRemaining / particle.m_lifeTime;

			// Size Update
			particle.m_size = glm::mix(m_sizeEnd, m_sizeBegin, lifePercent);

			// Colour Update
			particle.m_colour = glm::mix(m_colourEnd, m_colourBegin, lifePercent);
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
				//newParticle.m_velocity.x = std::rand() % static_cast<int>(std::abs(m_endVelocity.x - m_startVelocity.x)) * m_velocityVariation.x;
				//newParticle.m_velocity.y = std::rand() % static_cast<int>(std::abs(m_endVelocity.y - m_startVelocity.y)) * m_velocityVariation.y;
				newParticle.m_velocity.x = (Random::Float() - 0.5f);
				newParticle.m_velocity.y = (Random::Float() - 0.5f);

				// Colour
				newParticle.m_colour = m_colourBegin;
				newParticle.m_colour.x = static_cast<float>(std::rand() % static_cast<int>(std::abs(m_colourEnd.x - m_colourBegin.x)));
				newParticle.m_colour.y = static_cast<float>(std::rand() % static_cast<int>(std::abs(m_colourEnd.y - m_colourBegin.y)));
				newParticle.m_colour.z = static_cast<float>(std::rand() % static_cast<int>(std::abs(m_colourEnd.z - m_colourBegin.z)));

				// Size
				newParticle.m_size = m_sizeBegin;
				//newParticle.m_size = std::rand() % static_cast<int>(std::abs(m_sizeEnd - m_sizeBegin)) * m_sizeVariation;
				newParticle.m_size = m_sizeBegin + m_sizeVariation * (Random::Float() - 0.5f);

				// Add the particle into the emitter
				m_particles.push_back(newParticle);
			}
		}

		// Reset time interval
		timeInterval = 0.f;
	}
}

void ParticleSystem::Emitter::Emit()
{
	LOG_INFO("Emit");

	//Particle particle(m_position, m_startVelocity, m_colourBegin, m_sizeBegin, m_lifeTime, true);
	Particle particle(m_position, m_startVelocity, m_colourBegin, m_sizeBegin, m_lifeTime, true);

	particle.m_isActive = true;
	particle.m_position = m_position;
	//particle.m_rotation = Random::Float() * 2.0f * glm::pi<float>();
	particle.m_rotation = Random::Float() * 2.0f * std::numbers::pi;

	// Velocity
	particle.m_velocity = m_startVelocity;
	particle.m_velocity.x += m_velocityVariation.x * (Random::Float() - 0.5f);
	particle.m_velocity.y += m_velocityVariation.y * (Random::Float() - 0.5f);

	// Color
	particle.m_colour = m_colourBegin;

	particle.m_lifeTime = m_lifeTime;
	particle.m_lifeRemaining = m_lifeTime;
	particle.m_size = m_sizeBegin + m_sizeVariation * (Random::Float() - 0.5f);

	// Add the particle into the emitter
	m_particles.push_back(particle);
}

void ParticleSystem::Update(const float dt)
{
	LOG_INFO("ParticleSystem Update");

	// Update all the emitters here
	for (auto emitter : m_emitters)
		emitter->Update(dt);
}