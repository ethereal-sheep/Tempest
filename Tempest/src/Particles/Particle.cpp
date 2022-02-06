

#include "Particles.h"

Particle::Particle()
	: m_position{ 0.0f, 0.0f }
	, m_velocity{ 0.0f, 0.0f }
	//, m_rotation{ 0.0f }
	, m_colour{ 0.0f, 0.0f, 0.0f, 0.0f }
	, m_size{ 1.0f }
	, m_lifeTime{ 1.0f }
	, m_lifeRemaining{ 1.0f }
	, m_isActive{ false }
	, m_type{ ParticleType::Circle }
{
}

Particle::Particle(glm::vec2 pos, glm::vec2 vec, glm::vec4 colour, float size, float lifeTime, bool isActive, ParticleType particleType)
	: m_position{ pos }
	, m_velocity{ vec }
	//, m_rotation{ 0.0f }
	, m_colour { colour }
	, m_size {size}
	, m_lifeTime {lifeTime}
	, m_lifeRemaining {lifeTime}
	, m_isActive {isActive}
	, m_type{ particleType }
{
}