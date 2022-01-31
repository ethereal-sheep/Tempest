
#include "ParticleSystem_2D.h"

#include "WaypointEmitter.h"
#include "ExplosionEmitter.h"

// Debugging purpose
//#include "Logger/Log.h"

ParticleSystem_2D::ParticleSystem_2D()
{}

ParticleSystem_2D& ParticleSystem_2D::GetInstance()
{
	static ParticleSystem_2D instance;
	return instance;
}

void ParticleSystem_2D::Update()
{
	// Update all the emitters here
	for (short i = 0; i < m_emitters.size(); ++i)
	{
		// Check for null pointer - shouldn't happen
		if (m_emitters[i])
		{
			Emitter& emitter = *m_emitters[i];

			// Update active emitter
			if (emitter.m_GM.m_active)
				emitter.Update();
			else
			{
				// Store the unused slots to be reused later
				if (!m_UniqueEmitterSlots.contains(i))
				{  
					m_UniqueEmitterSlots.insert(i);
					m_availableEmitterSlots.push(i);
				}
			}
		}
	}
}

void ParticleSystem_2D::AddEmitter(const std::shared_ptr<Emitter> emitter)
{
	// There is an available slot
	if (m_availableEmitterSlots.size())
	{
		// The index of the free slot
		short freeSlot = m_availableEmitterSlots.front();

		// Update the emitter memory management
		m_availableEmitterSlots.pop();
		m_UniqueEmitterSlots.erase(freeSlot);

		// Allocate the emitter, destructor is handled s_ptr
		m_emitters[freeSlot] = emitter;
	}
	else
		m_emitters.push_back(emitter);
}

void ParticleSystem_2D::ButtonEmitter(glm::vec2 topLeftPos, glm::vec2 buttonSize)
{
	auto tempEmitter = std::make_shared<WaypointEmitter>();
	Emitter& emitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Emitter values - Without consideration for default ctor values
	emitter.m_GM.m_velocity.x = 0.0f;
	emitter.m_MM.m_duration = 1000.0f;
	emitter.m_GM.m_active = true;
	emitter.m_MM.m_preWarm = true;
	emitter.m_MM.m_simulationSpeed = 0.016f;

	emitter.m_EM.m_spawnTimeInterval = 0.016f;
	emitter.m_EM.m_spawnCountTimer = emitter.m_EM.m_spawnTimeInterval;
	emitter.m_EM.m_rateOverTime = 1;
	emitter.m_MM.m_maxParticles = 1000;

	emitter.m_wayPointIndex = 0;
	emitter.m_recalculateVelocity = true;

	// Particle Architype values - without consideration for default ctor
	emitter.m_PAM.m_startVelocity = glm::vec2{ 0.f, 0.f };
	emitter.m_PAM.m_endVelocity = glm::vec2{ 0.f, 0.f };
	emitter.m_PAM.m_velocityVariation = glm::vec2{ 0.0f, 0.0f };

	emitter.m_PAM.m_sizeBegin = 3.0f;
	emitter.m_PAM.m_sizeEnd = 3.0f;
	emitter.m_PAM.m_sizeVariation = 0.0f;

	emitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	emitter.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	emitter.m_PAM.m_lifeTime = 2.0f;
	emitter.m_RM.m_type = ParticleType::Circle;

	// Center position of the button
	emitter.m_GM.m_position = topLeftPos;

	// Shift to one corner of the button - Left Bottom
	//emitter.m_GM.m_position.x -= buttonSize.x * 0.5f;
	//emitter.m_GM.m_position.y -= buttonSize.y * 0.5f;

	// Prepare the waypoints
	glm::vec2 wp_LeftBottom = topLeftPos;
	wp_LeftBottom.y += buttonSize.y;

	glm::vec2 wp_RightBottom = wp_LeftBottom;
	wp_RightBottom.x += buttonSize.x;

	glm::vec2 wp_RightTop = wp_RightBottom;
	wp_RightTop.y -= buttonSize.y;

	glm::vec2 wp_LeftTop = topLeftPos;

	// Add the waypoints
	emitter.m_wayPoints.push_back(wp_LeftBottom);
	emitter.m_wayPoints.push_back(wp_RightBottom);
	emitter.m_wayPoints.push_back(wp_RightTop);
	emitter.m_wayPoints.push_back(wp_LeftTop);
}

void ParticleSystem_2D::ExplosionEmitter_2(glm::vec2 spawnPos)
{
	auto tempEmitter = std::make_shared<ExplosionEmitter>();
	Emitter& explosionEmitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Emitter values - Without consideration for default ctor values
	explosionEmitter.m_GM.m_position = spawnPos;
	//explosionEmitter.m_GM.m_velocity.x = -500.0f;
	explosionEmitter.m_MM.m_duration = 2.f;
	explosionEmitter.m_GM.m_active = true;
	explosionEmitter.m_MM.m_preWarm = true;

	explosionEmitter.m_EM.m_spawnTimeInterval = 2.f; // 5x slower of dt
	explosionEmitter.m_EM.m_spawnCountTimer = explosionEmitter.m_EM.m_spawnTimeInterval;
	explosionEmitter.m_EM.m_rateOverTime = 20;
	explosionEmitter.m_MM.m_maxParticles = 1000;

	explosionEmitter.m_wayPointIndex = 0;
	explosionEmitter.m_recalculateVelocity = true;

	// Particle Architype values - without consideration for default ctor
	explosionEmitter.m_PAM.m_startVelocity = glm::vec2{ 0.f, 0.f };
	explosionEmitter.m_PAM.m_endVelocity = glm::vec2{ 0.f, 0.f };
	explosionEmitter.m_PAM.m_velocityVariation = glm::vec2{ 3.0f, 1.0f };

	explosionEmitter.m_PAM.m_sizeBegin = 10.0f;
	explosionEmitter.m_PAM.m_sizeEnd = 0.0f;
	explosionEmitter.m_PAM.m_sizeVariation = 0.3f;

	explosionEmitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	explosionEmitter.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };

	explosionEmitter.m_PAM.m_lifeTime = 0.3f;
	explosionEmitter.m_RM.m_type = ParticleType::Circle;
}