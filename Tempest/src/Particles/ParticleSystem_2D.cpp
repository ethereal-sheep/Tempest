
#include "ParticleSystem_2D.h"

void ParticleSystem_2D::Update(const float dt)
{
	// Update all the emitters here
	for (auto& emitter : m_emitters)
		if (emitter->m_GM.m_active)
		{
			emitter->Update(dt);
		}
}

void ParticleSystem_2D::ButtonEmitter(Emitter& emitter)
{
	// Emitter values - Without consideration for default ctor values
	emitter.m_GM.m_velocity.x = -500.0f;
	emitter.m_MM.m_duration = 1000.0f;
	emitter.m_GM.m_active = true;
	//emitter.m_preWarm = true;

	emitter.m_EM.m_spawnTimeInterval = 0.080f; // 5x slower of dt
	emitter.m_EM.m_spawnCountTimer = emitter.m_EM.m_spawnTimeInterval;
	emitter.m_EM.m_rateOverTime = 1;
	emitter.m_MM.m_maxParticles = 1000;

	emitter.m_wayPointIndex = 0;
	emitter.m_recalculateVelocity = true;

	// Particle Architype values - without consideration for default ctor
	emitter.m_PAM.m_startVelocity = glm::vec2{ 0.f, 0.f };
	emitter.m_PAM.m_endVelocity = glm::vec2{ 0.f, 0.f };
	emitter.m_PAM.m_velocityVariation = glm::vec2{ 3.0f, 1.0f };
	
	emitter.m_PAM.m_sizeBegin = 20.0f;
	emitter.m_PAM.m_sizeEnd = 0.0f;
	emitter.m_PAM.m_sizeVariation = 0.3f;

	emitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	emitter.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };

	emitter.m_PAM.m_lifeTime= 2.0f;

	emitter.m_RM.m_type = ParticleType::Square;

	glm::vec2 wayPoint_1 = emitter.m_GM.m_position;
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

void ParticleSystem_2D::ButtonEmitter_2(Emitter& emitter, glm::vec2 topLeftPos, glm::vec2 buttonSize)
{
	// Emitter values - Without consideration for default ctor values
	emitter.m_GM.m_velocity.x = -500.0f;
	emitter.m_MM.m_duration = 1000.0f;
	emitter.m_GM.m_active = true;
	//emitter.m_preWarm = true;

	emitter.m_EM.m_spawnTimeInterval = 0.080f; // 5x slower of dt
	emitter.m_EM.m_spawnCountTimer = emitter.m_EM.m_spawnTimeInterval;
	emitter.m_EM.m_rateOverTime = 1;
	emitter.m_MM.m_maxParticles = 1000;

	emitter.m_wayPointIndex = 0;
	emitter.m_recalculateVelocity = true;

	// Particle Architype values - without consideration for default ctor
	emitter.m_PAM.m_startVelocity = glm::vec2{ 0.f, 0.f };
	emitter.m_PAM.m_endVelocity = glm::vec2{ 0.f, 0.f };
	emitter.m_PAM.m_velocityVariation = glm::vec2{ 3.0f, 1.0f };

	emitter.m_PAM.m_sizeBegin = 20.0f;
	emitter.m_PAM.m_sizeEnd = 0.0f;
	emitter.m_PAM.m_sizeVariation = 0.3f;

	emitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	emitter.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };

	emitter.m_PAM.m_lifeTime = 2.0f;
	emitter.m_RM.m_type = ParticleType::Square;

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

	glm::vec2 wp_LeftTop = wp_RightTop;
	wp_LeftTop.x -= buttonSize.x;

	// Add the waypoints
	emitter.m_wayPoints.push_back(wp_LeftBottom);
	emitter.m_wayPoints.push_back(wp_RightBottom);
	emitter.m_wayPoints.push_back(wp_RightTop);
	emitter.m_wayPoints.push_back(wp_LeftTop);
}