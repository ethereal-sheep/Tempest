
#include "ParticleSystem_2D.h"

void ParticleSystem_2D::Update(const float dt)
{
	// Update all the emitters here
	for (auto& emitter : m_emitters)
		if (emitter->m_active)
		{
			emitter->Update(dt);
		}
}

void ParticleSystem_2D::ButtonEmitter(Emitter& emitter)
{
	// Emitter values - Without consideration for default ctor values
	emitter.m_velocity.x = -500.0f;
	emitter.m_lifeTime = 1000.0f;
	emitter.m_active = true;
	emitter.m_preWarm = true;

	emitter.m_spawnTimeInterval = 0.080f; // 5x slower of dt
	emitter.m_countTimer = emitter.m_spawnTimeInterval;
	emitter.m_spawnCount = 1;
	emitter.m_maxParticles = 1000;

	emitter.m_wayPointIndex = 0;
	emitter.m_recalculateVelocity = true;

	// Particle Architype values - without consideration for default ctor
	emitter.m_PA.m_startVelocity = glm::vec2{ 0.f, 0.f };
	emitter.m_PA.m_endVelocity = glm::vec2{ 0.f, 0.f };
	emitter.m_PA.m_velocityVariation = glm::vec2{ 3.0f, 1.0f };
	
	emitter.m_PA.m_sizeBegin = 20.0f;
	emitter.m_PA.m_sizeEnd = 0.0f;
	emitter.m_PA.m_sizeVariation = 0.3f;

	emitter.m_PA.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	emitter.m_PA.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };

	emitter.m_PA.m_lifeTime = 2.0f;

	emitter.m_PA.m_type = ParticleType::Square;

	glm::vec2 wayPoint_1 = emitter.m_position;
	wayPoint_1.x += 300;

	glm::vec2 wayPoint_2 = wayPoint_1;
	wayPoint_2.y -= 50;

	glm::vec2 wayPoint_3 = wayPoint_2;
	wayPoint_3.x -= 300;

	glm::vec2 wayPoint_4 = wayPoint_3;
	wayPoint_4.y += 50;

	emitter.m_wayPoints.push_back(wayPoint_1);
	emitter.m_wayPoints.push_back(wayPoint_2);
	emitter.m_wayPoints.push_back(wayPoint_3);
	emitter.m_wayPoints.push_back(wayPoint_4);
}