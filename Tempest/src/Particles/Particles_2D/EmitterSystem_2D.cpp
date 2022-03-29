/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "EmitterSystem_2D.h"

#include "WaypointEmitter_2D.h"
#include "CircularMotionEmitter_2D.h"
#include "ExplosionEmitter_2D.h"
#include "RandomSpawnPointEmitter_2D.h"

// Debugging purpose
#include "Logger/Log.h"

// Convert Imvec 2
#include "../../Editor/src/Extern/imgui/imgui.h"

// Convert Imvec2 -> glm::vec2
auto Imvec2_To_GlmVec2_Converter = [](ImVec2 imvec)
{
	glm::vec2 tempVec;
	tempVec.x = imvec.x;
	tempVec.y = imvec.y;

	return tempVec;
};

EmitterSystem_2D::EmitterSystem_2D()
{}

EmitterSystem_2D& EmitterSystem_2D::GetInstance()
{
	static EmitterSystem_2D instance;
	return instance;
}

void EmitterSystem_2D::Update()
{
	// Update all the emitters here
	for (short i = 0; i < m_emitters.size(); ++i)
	{
		// Check for null pointer - shouldn't happen
		if (m_emitters[i])
		{
			Emitter_2D& emitter = *m_emitters[i];

			// Update active emitter
			if (emitter.m_GM.m_active)
				emitter.Update();
			//else
			//{
			//	// Store the unused slots to be reused later
			//	if (!m_UniqueEmitterSlots.contains(i))
			//	{  
			//		m_UniqueEmitterSlots.insert(i);
			//		m_availableEmitterSlots.push(i);
			//	}
			//}
		}
	}
}

void EmitterSystem_2D::AddEmitter(const std::shared_ptr<Emitter_2D> emitter)
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

void EmitterSystem_2D::CreateButtonEmitter(std::weak_ptr<WaypointEmitter_2D>& emitter, ImVec2 topLeftPos, ImVec2 buttonSize)
{
	return CreateButtonEmitter(emitter, Imvec2_To_GlmVec2_Converter(topLeftPos), Imvec2_To_GlmVec2_Converter(buttonSize));
}

void EmitterSystem_2D::CreateButtonEmitter(std::weak_ptr<WaypointEmitter_2D>& emitter, glm::vec2 topLeftPos, glm::vec2 buttonSize)
{
	if (emitter.expired())
	{
		auto sh_ptr = std::make_shared<WaypointEmitter_2D>();
		emitter = sh_ptr;
		AddEmitter(sh_ptr);
	}
	else
	{	// Reset the emitter
		emitter.lock()->ClearAllParticles();
	}

	// Emitter_2D values - Without consideration for default ctor values
	emitter.lock()->m_GM.m_velocity.x = 0.0f;
	emitter.lock()->m_MM.m_duration = 1000.0f;
	emitter.lock()->m_GM.m_active = true;
	emitter.lock()->m_MM.m_preWarm = true;
	emitter.lock()->m_MM.m_simulationSpeed = 0.016f;

	emitter.lock()->m_EM.m_spawnTimeInterval = 0.016f;
	emitter.lock()->m_EM.m_spawnCountTimer = emitter.lock()->m_EM.m_spawnTimeInterval;
	emitter.lock()->m_EM.m_rateOverTime = 1;
	emitter.lock()->m_MM.m_maxParticles = 1000;

	emitter.lock()->m_wayPointIndex = 0;
	emitter.lock()->m_recalculateVelocity = true;

	// Emitter_2D values - Without consideration for default ctor values
	emitter.lock()->m_GM.m_velocity.x = 0.0f;
	emitter.lock()->m_MM.m_duration = 1000.0f;
	emitter.lock()->m_GM.m_active = true;
	emitter.lock()->m_MM.m_preWarm = true;
	emitter.lock()->m_MM.m_simulationSpeed = 0.016f;

	emitter.lock()->m_EM.m_spawnTimeInterval = 0.016f;
	emitter.lock()->m_EM.m_spawnCountTimer = emitter.lock()->m_EM.m_spawnTimeInterval;
	emitter.lock()->m_EM.m_rateOverTime = 1;
	emitter.lock()->m_MM.m_maxParticles = 1000;

	emitter.lock()->m_wayPointIndex = 0;
	emitter.lock()->m_recalculateVelocity = true;

	// Particle Architype values - without consideration for default ctor
	emitter.lock()->m_PAM.m_velocityStart = glm::vec2{ 0.f, 0.f };
	emitter.lock()->m_PAM.m_velocityEnd = glm::vec2{ 0.f, 0.f };
	emitter.lock()->m_PAM.m_velocityVariation = glm::vec2{ 0.0f, 0.0f };

	emitter.lock()->m_PAM.m_scaleBegin = 3.0f;
	emitter.lock()->m_PAM.m_scaleEnd = 3.0f;
	emitter.lock()->m_PAM.m_scaleVariation = 0.0f;

	emitter.lock()->m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	emitter.lock()->m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	emitter.lock()->m_PAM.m_lifeTime = 2.0f;
	emitter.lock()->m_RM.m_type = ParticleType::Circle;

	// Center position of the button
	emitter.lock()->m_GM.m_position = topLeftPos;

	// Prepare the waypoints
	glm::vec2 wp_LeftTop = topLeftPos;

	glm::vec2 wp_RightTop = wp_LeftTop;
	wp_RightTop.x += buttonSize.x;

	glm::vec2 wp_LeftBottom = wp_LeftTop;
	wp_LeftBottom.y += buttonSize.y;

	glm::vec2 wp_RightBottom = wp_RightTop;
	wp_RightBottom.y = wp_LeftBottom.y;

	// Reset the waypoints
	emitter.lock()->m_wayPoints.clear();

	// Add the waypoints
	emitter.lock()->m_wayPoints.push_back(wp_LeftBottom);
	emitter.lock()->m_wayPoints.push_back(wp_RightBottom);
	emitter.lock()->m_wayPoints.push_back(wp_RightTop);
	emitter.lock()->m_wayPoints.push_back(wp_LeftTop);
}

void EmitterSystem_2D::CreateExplosionEmitter(std::weak_ptr<ExplosionEmitter_2D>& emitter, ImVec2 spawnPos)
{
	CreateExplosionEmitter(emitter, Imvec2_To_GlmVec2_Converter(spawnPos));
}

void EmitterSystem_2D::CreateExplosionEmitter(std::weak_ptr<ExplosionEmitter_2D>& emitter, glm::vec2 spawnPos)
{
	if (emitter.expired())
	{
		auto tempEmitter = std::make_shared<ExplosionEmitter_2D>();
		emitter = tempEmitter;
		AddEmitter(tempEmitter);
	}
	else
	{
		emitter.lock()->ClearAllParticles();
	}

	// Emitter_2D values - Without consideration for default ctor values
	emitter.lock()->m_GM.m_position = spawnPos;
	//emitter.lock()->m_GM.m_velocity.x = -500.0f;
	emitter.lock()->m_MM.m_duration = 2.f;
	emitter.lock()->m_GM.m_active = true;
	emitter.lock()->m_MM.m_preWarm = true;

	emitter.lock()->m_EM.m_spawnTimeInterval = 2.f; // 5x slower of dt
	emitter.lock()->m_EM.m_spawnCountTimer = emitter.lock()->m_EM.m_spawnTimeInterval;
	emitter.lock()->m_EM.m_rateOverTime = 20;
	emitter.lock()->m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	emitter.lock()->m_PAM.m_velocityStart = glm::vec2{ 0.f, 0.f };
	emitter.lock()->m_PAM.m_velocityEnd = glm::vec2{ 0.f, 0.f };
	emitter.lock()->m_PAM.m_velocityVariation = glm::vec2{ 3.0f, 1.0f };

	emitter.lock()->m_PAM.m_scaleBegin = 10.0f;
	emitter.lock()->m_PAM.m_scaleEnd = 0.0f;
	emitter.lock()->m_PAM.m_scaleVariation = 0.3f;

	emitter.lock()->m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	emitter.lock()->m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	emitter.lock()->m_PAM.m_lifeTime = 0.3f;
	emitter.lock()->m_RM.m_type = ParticleType::Circle;
}

void EmitterSystem_2D::CreateCircularMotionEmitter_2D(std::weak_ptr<CircularMotionEmitter_2D>& emitter, ImVec2 centrePos, float radius)
{
	CreateCircularMotionEmitter_2D(emitter, Imvec2_To_GlmVec2_Converter(centrePos), radius);
}

void EmitterSystem_2D::CreateCircularMotionEmitter_2D(std::weak_ptr<CircularMotionEmitter_2D>& emitter, glm::vec2 centrePos, float radius)
{
	if (emitter.expired())
	{
		auto tempEmitter = std::make_shared<CircularMotionEmitter_2D>();
		emitter = tempEmitter;
		AddEmitter(tempEmitter);
	}
	else
	{
		emitter.lock()->ClearAllParticles();
	}

	// Center position of the circle
	emitter.lock()->m_centrePoint = centrePos;
	//tempEmitter->m_GM.m_position = centrePos;
	emitter.lock()->m_radius = radius;

	// Emitter_2D values - Without consideration for default ctor values
	emitter.lock()->m_GM.m_velocity.x = 0.0f;
	emitter.lock()->m_MM.m_duration = 1000.0f;
	emitter.lock()->m_GM.m_active = true;
	emitter.lock()->m_MM.m_preWarm = true;
	emitter.lock()->m_MM.m_simulationSpeed = 0.016f;

	emitter.lock()->m_EM.m_spawnTimeInterval = 0.016f;
	emitter.lock()->m_EM.m_spawnCountTimer = emitter.lock()->m_EM.m_spawnTimeInterval;
	emitter.lock()->m_EM.m_rateOverTime = 1;
	emitter.lock()->m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	emitter.lock()->m_PAM.m_velocityStart = glm::vec2{ 0.f, 0.f };
	emitter.lock()->m_PAM.m_velocityEnd = glm::vec2{ 0.f, 0.f };
	emitter.lock()->m_PAM.m_velocityVariation = glm::vec2{ 0.0f, 0.0f };

	emitter.lock()->m_PAM.m_scaleBegin = 8.0f;
	emitter.lock()->m_PAM.m_scaleEnd = 8.0f;
	emitter.lock()->m_PAM.m_scaleVariation = 0.0f;

	emitter.lock()->m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	emitter.lock()->m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	emitter.lock()->m_PAM.m_lifeTime = 1.0f;
	emitter.lock()->m_RM.m_type = ParticleType::Square;
}

void EmitterSystem_2D::CreateBackgroundEmitter(std::weak_ptr<RandomSpawnPointEmitter_2D>& emitter, ImVec2 min, ImVec2 max)
{
	CreateBackgroundEmitter(emitter, Imvec2_To_GlmVec2_Converter(min), Imvec2_To_GlmVec2_Converter(max));
}

void EmitterSystem_2D::CreateBackgroundEmitter(std::weak_ptr<RandomSpawnPointEmitter_2D>& emitter, glm::vec2 min, glm::vec2 max)
{
	if (emitter.expired())
	{
		auto tempEmitter = std::make_shared<RandomSpawnPointEmitter_2D>();
		emitter = tempEmitter;
		AddEmitter(tempEmitter);
	}
	else
	{
		emitter.lock()->ClearAllParticles();
	}

	// Center position of the circle
	emitter.lock()->m_Min = min;
	//tempEmitter->m_GM.m_position = centrePos;
	emitter.lock()->m_Max = max;

	// Emitter_2D values - Without consideration for default ctor values
	emitter.lock()->m_GM.m_velocity.x = 0.0f;
	emitter.lock()->m_MM.m_duration = 1000.0f;
	emitter.lock()->m_GM.m_active = true;
	emitter.lock()->m_MM.m_preWarm = true;
	emitter.lock()->m_MM.m_simulationSpeed = 0.016f;

	emitter.lock()->m_EM.m_spawnTimeInterval = 0.08f;
	emitter.lock()->m_EM.m_spawnCountTimer = emitter.lock()->m_EM.m_spawnTimeInterval;
	emitter.lock()->m_EM.m_rateOverTime = 1;
	emitter.lock()->m_MM.m_maxParticles = 300;

	// Particle Architype values - without consideration for default ctor
	emitter.lock()->m_PAM.m_velocityStart = glm::vec2{ 0.f, 0.f };
	emitter.lock()->m_PAM.m_velocityEnd = glm::vec2{ 0.f, 0.f };
	emitter.lock()->m_PAM.m_velocityVariation = glm::vec2{ 100.0f, 100.0f };

	emitter.lock()->m_PAM.m_scaleBegin = 8.0f;
	emitter.lock()->m_PAM.m_scaleEnd = 8.0f;
	emitter.lock()->m_PAM.m_scaleVariation = 0.0f;

	emitter.lock()->m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	emitter.lock()->m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	emitter.lock()->m_PAM.m_lifeTime = 1.0f;
	emitter.lock()->m_RM.m_type = ParticleType::Square;
}

