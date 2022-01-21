#include "ParticleSystem_2D.h"
#include "Random.h"

#include "Util.h"

#include <numbers>

ParticleSystem_2D::Particle::Particle()
	: m_position{ 0.0f, 0.0f }
	, m_velocity{ 0.0f, 0.0f }
	, m_rotation{ 0.0f }
	, m_colour{ 0.0f, 0.0f, 0.0f, 0.0f }
	, m_size{ 1.0f }
	, m_lifeTime{ 1.0f }
	, m_lifeRemaining{ 1.0f }
	, m_isActive {false}
	, m_type{ParticleSystem_2D::ParticleType::Circle}
{
	minX = m_position.x - m_size * 0.5f;
	maxX = m_position.x + m_size * 0.5f;

	minY = m_position.y - m_size * 0.5f;
	maxY = m_position.y + m_size * 0.5f;
}

ParticleSystem_2D::Particle::Particle(glm::vec2 pos, glm::vec2 vec, glm::vec4 colour, float size, float lifeTime, bool isActive, ParticleType particleType)
	: m_position{ pos }
	, m_velocity{ vec }
	, m_rotation{ 0.0f }
	, m_colour { colour }
	, m_size {size}
	, m_lifeTime {lifeTime}
	, m_lifeRemaining {lifeTime}
	, m_isActive {isActive}
	, m_type{ particleType }
{
	// Initialise the min and max
	minX = m_position.x - size * 0.5f;
	maxX = m_position.x + size * 0.5f;

	minY = m_position.y - size * 0.5f;
	maxY = m_position.y + size * 0.5f;
}
ParticleSystem_2D::Emitter::Emitter()
	: m_colourBegin { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f }
	, m_colourEnd { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f }
	, m_sizeBegin{ 50.0f }
	, m_sizeVariation{ 0.3f }
	, m_sizeEnd{ 0.0f }
	, m_lifeTime{ 5.0f }
	, m_startVelocity { 1.0f, 0.0f }
	, m_endVelocity { 0.0f, 0.0f }
	, m_velocityVariation { 3.0f, 1.0f }
	, m_position { 0.0f, 0.0f }
	//, m_loop{ false }
	//, m_rateOvertime{0.0f}
	, m_maxParticles {1000} // Hard coded for now
{
	//m_particles.resize(m_maxParticles); // Testing for now
}

void ParticleSystem_2D::Emitter::Update(const float dt)
{
	static float timeInterval = 0.f;

	//LOG_INFO("Emitter Update");

	// Particles' Behaviour
	for (auto& particle : m_particles)
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
			//LOG_INFO("Particles Update");

			// Transform Update
			particle.m_position += particle.m_velocity * dt;
			
			particle.minX = m_position.x - particle.m_size * 0.5f;
			particle.maxX = m_position.x + particle.m_size * 0.5f;

			particle.minY = m_position.y - particle.m_size * 0.5f;
			particle.maxY = m_position.y + particle.m_size * 0.5f;

			particle.m_rotation += 0.01f * dt;

			//LOG_INFO("Particle Position X");
			//LOG_INFO(particle.m_position.x);

			//LOG_INFO("Particle Position Y");
			//LOG_INFO(particle.m_position.y);

			//LOG_INFO("Particle Velocity X");
			//LOG_INFO(particle.m_velocity.x);

			//LOG_INFO("Particle Velocity Y");
			//LOG_INFO(particle.m_velocity.y);

			// Calculate the lifeTime remaining
			float lifePercent = particle.m_lifeRemaining / particle.m_lifeTime;

			// Size Update
			//particle.m_size = glm::mix(m_sizeEnd, m_sizeBegin, lifePercent);

			// Colour Update
			particle.m_colour = glm::mix(m_colourEnd, m_colourBegin, lifePercent);

			// Reduce the particle life
			particle.m_lifeRemaining -= dt;
		}
	}

	// TO DO, Emitter Behaviour - One second is per unit time
	//if (timeInterval >= 1.f)
	//{
	//	if (m_particles.size() < m_maxParticles)
	//	{
	//		// Emit particle
	//		for (short i = 0; i < m_rateOvertime; ++i)
	//		{
	//			Particle newParticle;

	//			// Initialise the particle data - Not supposed to but too much member variable?
	//			// 
	//			// Initial position to be at emitter
	//			newParticle.m_position = m_position;

	//			// Lifetime
	//			newParticle.m_lifeTime = m_lifeTime;
	//			newParticle.m_lifeRemaining = m_lifeTime;

	//			// Velocity
	//			newParticle.m_velocity = m_startVelocity;
	//			//newParticle.m_velocity.x = std::rand() % static_cast<int>(std::abs(m_endVelocity.x - m_startVelocity.x)) * m_velocityVariation.x;
	//			//newParticle.m_velocity.y = std::rand() % static_cast<int>(std::abs(m_endVelocity.y - m_startVelocity.y)) * m_velocityVariation.y;
	//			newParticle.m_velocity.x = (Random::Float() - 0.5f);
	//			newParticle.m_velocity.y = (Random::Float() - 0.5f);

	//			// Colour
	//			newParticle.m_colour = m_colourBegin;
	//			//newParticle.m_colour.x = static_cast<float>(std::rand() % static_cast<int>(std::abs(m_colourEnd.x - m_colourBegin.x)));
	//			//newParticle.m_colour.y = static_cast<float>(std::rand() % static_cast<int>(std::abs(m_colourEnd.y - m_colourBegin.y)));
	//			//newParticle.m_colour.z = static_cast<float>(std::rand() % static_cast<int>(std::abs(m_colourEnd.z - m_colourBegin.z)));

	//			// Size
	//			newParticle.m_size = m_sizeBegin;
	//			//newParticle.m_size = std::rand() % static_cast<int>(std::abs(m_sizeEnd - m_sizeBegin)) * m_sizeVariation;
	//			newParticle.m_size = m_sizeBegin + m_sizeVariation * (Random::Float() - 0.5f);

	//			// Add the particle into the emitter
	//			m_particles.push_back(newParticle);
	//		}

	//		// Reset time interval
	//		timeInterval = 0.f;
	//	}

	//	// Reset time interval
	//	timeInterval += dt;
	//}
}

void ParticleSystem_2D::Emitter::Emit(const int particleAmount, ParticleType particleType)
{
	for (short i = 0; i < particleAmount; ++i)
	{
		//LOG_INFO("Emit");

		//Particle particle(m_position, m_startVelocity, m_colourBegin, m_sizeBegin, m_lifeTime, true);
		Particle particle(m_position, m_startVelocity, m_colourBegin, m_sizeBegin, m_lifeTime, true, particleType);

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
		particle.m_colour.r = (Random::Float() - 0.5f);
		particle.m_colour.g = (Random::Float() - 0.5f);
		particle.m_colour.b = (Random::Float() - 0.5f);

		particle.m_lifeTime = m_lifeTime;
		particle.m_lifeRemaining = m_lifeTime;
		particle.m_size = m_sizeBegin + m_sizeVariation * (Random::Float() - 0.5f);

		// Add the particle into the emitter
		m_particles.push_back(particle);
	}
}

void ParticleSystem_2D::Update(const float dt)
{
	//LOG_INFO("ParticleSystem Update");

	// Update all the emitters here
	for (auto& emitter : m_emitters)
		emitter->Update(dt);
}