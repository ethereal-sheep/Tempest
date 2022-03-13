/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "ParticleSystem_2D.h"

#include "WaypointEmitter_2D.h"
#include "CircularMotionEmitter_2D.h"
#include "ExplosionEmitter_2D.h"

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

void ParticleSystem_2D::AddEmitter(const std::shared_ptr<Emitter_2D> emitter)
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

const std::weak_ptr<WaypointEmitter_2D> ParticleSystem_2D::CreateButtonEmitter(ImVec2 topLeftPos, ImVec2 buttonSize)
{
	return CreateButtonEmitter(Imvec2_To_GlmVec2_Converter(topLeftPos), Imvec2_To_GlmVec2_Converter(buttonSize));
}

const std::weak_ptr<WaypointEmitter_2D> ParticleSystem_2D::CreateButtonEmitter(glm::vec2 topLeftPos, glm::vec2 buttonSize)
{
	auto tempEmitter = std::make_shared<WaypointEmitter_2D>();
	WaypointEmitter_2D& emitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Emitter_2D values - Without consideration for default ctor values
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
	glm::vec2 wp_LeftTop = topLeftPos;

	glm::vec2 wp_RightTop = wp_LeftTop;
	wp_RightTop.x += buttonSize.x;

	glm::vec2 wp_LeftBottom = wp_LeftTop;
	wp_LeftBottom.y += buttonSize.y;

	glm::vec2 wp_RightBottom = wp_RightTop;
	wp_RightBottom.y = wp_LeftBottom.y;

	// Add the waypoints
	emitter.m_wayPoints.push_back(wp_LeftBottom); 
	emitter.m_wayPoints.push_back(wp_RightBottom);
	emitter.m_wayPoints.push_back(wp_RightTop);
	emitter.m_wayPoints.push_back(wp_LeftTop);

	//LOG_INFO("Given Value x: {0}, y: {1}", topLeftPos.x, topLeftPos.y);
	//LOG_INFO("Button Size x: {0}, y: {1}", buttonSize.x, buttonSize.y);
	//LOG_INFO("Btm Left  x: {0}, y: {1}", wp_LeftBottom.x, wp_LeftBottom.y);
	//LOG_INFO("Btm Right x: {0}, y: {1}", wp_RightBottom.x, wp_RightBottom.y);
	//LOG_INFO("Top Right x: {0}, y: {1}", wp_RightTop.x, wp_RightTop.y);
	//LOG_INFO("Top Left  x: {0}, y: {1}", wp_LeftTop.x, wp_LeftTop.y);

	return tempEmitter;
}

void ParticleSystem_2D::ReuseButtonEmitter(const std::shared_ptr<WaypointEmitter_2D>& emitter, ImVec2 topLeftPos, ImVec2 buttonSize)
{
	return ReuseButtonEmitter(emitter, Imvec2_To_GlmVec2_Converter(topLeftPos), Imvec2_To_GlmVec2_Converter(buttonSize));
}

void ParticleSystem_2D::ReuseButtonEmitter(const std::shared_ptr<WaypointEmitter_2D>& emitter, glm::vec2 topLeftPos, glm::vec2 buttonSize)
{
	WaypointEmitter_2D& waypointEmitter = *emitter.get();

	// Clear all previous particle of the emitter
	waypointEmitter.ClearAllParticles();

	// Emitter_2D values - Without consideration for default ctor values
	waypointEmitter.m_GM.m_velocity.x = 0.0f;
	waypointEmitter.m_MM.m_duration = 1000.0f;
	waypointEmitter.m_GM.m_active = true;
	waypointEmitter.m_MM.m_preWarm = true;
	waypointEmitter.m_MM.m_simulationSpeed = 0.016f;

	waypointEmitter.m_EM.m_spawnTimeInterval = 0.016f;
	waypointEmitter.m_EM.m_spawnCountTimer = waypointEmitter.m_EM.m_spawnTimeInterval;
	waypointEmitter.m_EM.m_rateOverTime = 1;
	waypointEmitter.m_MM.m_maxParticles = 1000;

	waypointEmitter.m_wayPointIndex = 0;
	waypointEmitter.m_recalculateVelocity = true;

	// Particle Architype values - without consideration for default ctor
	waypointEmitter.m_PAM.m_startVelocity = glm::vec2{ 0.f, 0.f };
	waypointEmitter.m_PAM.m_endVelocity = glm::vec2{ 0.f, 0.f };
	waypointEmitter.m_PAM.m_velocityVariation = glm::vec2{ 0.0f, 0.0f };

	waypointEmitter.m_PAM.m_sizeBegin = 3.0f;
	waypointEmitter.m_PAM.m_sizeEnd = 3.0f;
	waypointEmitter.m_PAM.m_sizeVariation = 0.0f;

	waypointEmitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	waypointEmitter.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	waypointEmitter.m_PAM.m_lifeTime = 2.0f;
	waypointEmitter.m_RM.m_type = ParticleType::Circle;

	// Center position of the button
	waypointEmitter.m_GM.m_position = topLeftPos;

	// Shift to one corner of the button - Left Bottom
	//waypointEmitter.m_GM.m_position.x -= buttonSize.x * 0.5f;
	//waypointEmitter.m_GM.m_position.y -= buttonSize.y * 0.5f;

	// Prepare the waypoints
	glm::vec2 wp_LeftTop = topLeftPos;

	glm::vec2 wp_RightTop = wp_LeftTop;
	wp_RightTop.x += buttonSize.x;

	glm::vec2 wp_LeftBottom = wp_LeftTop;
	wp_LeftBottom.y += buttonSize.y;

	glm::vec2 wp_RightBottom = wp_RightTop;
	wp_RightBottom.y = wp_LeftBottom.y;

	// Reset the waypoints
	waypointEmitter.m_wayPoints.clear();
	
	// Add the waypoints
	waypointEmitter.m_wayPoints.push_back(wp_LeftBottom);
	waypointEmitter.m_wayPoints.push_back(wp_RightBottom);
	waypointEmitter.m_wayPoints.push_back(wp_RightTop);
	waypointEmitter.m_wayPoints.push_back(wp_LeftTop);

	//LOG_INFO("Given Value x: {0}, y: {1}", topLeftPos.x, topLeftPos.y);
	//LOG_INFO("Button Size x: {0}, y: {1}", buttonSize.x, buttonSize.y);
	//LOG_INFO("Btm Left  x: {0}, y: {1}", wp_LeftBottom.x, wp_LeftBottom.y);
	//LOG_INFO("Btm Right x: {0}, y: {1}", wp_RightBottom.x, wp_RightBottom.y);
	//LOG_INFO("Top Right x: {0}, y: {1}", wp_RightTop.x, wp_RightTop.y);
	//LOG_INFO("Top Left  x: {0}, y: {1}", wp_LeftTop.x, wp_LeftTop.y);
}

const std::weak_ptr<ExplosionEmitter_2D> ParticleSystem_2D::CreateExplosionEmitter(ImVec2 spawnPos)
{
	return CreateExplosionEmitter(Imvec2_To_GlmVec2_Converter(spawnPos));
}

const std::weak_ptr<ExplosionEmitter_2D> ParticleSystem_2D::CreateExplosionEmitter(glm::vec2 spawnPos)
{
	auto tempEmitter = std::make_shared<ExplosionEmitter_2D>();
	Emitter_2D& explosionEmitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Emitter_2D values - Without consideration for default ctor values
	explosionEmitter.m_GM.m_position = spawnPos;
	//explosionEmitter.m_GM.m_velocity.x = -500.0f;
	explosionEmitter.m_MM.m_duration = 2.f;
	explosionEmitter.m_GM.m_active = true;
	explosionEmitter.m_MM.m_preWarm = true;

	explosionEmitter.m_EM.m_spawnTimeInterval = 2.f; // 5x slower of dt
	explosionEmitter.m_EM.m_spawnCountTimer = explosionEmitter.m_EM.m_spawnTimeInterval;
	explosionEmitter.m_EM.m_rateOverTime = 20;
	explosionEmitter.m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	explosionEmitter.m_PAM.m_startVelocity = glm::vec2{ 0.f, 0.f };
	explosionEmitter.m_PAM.m_endVelocity = glm::vec2{ 0.f, 0.f };
	explosionEmitter.m_PAM.m_velocityVariation = glm::vec2{ 3.0f, 1.0f };

	explosionEmitter.m_PAM.m_sizeBegin = 10.0f;
	explosionEmitter.m_PAM.m_sizeEnd = 0.0f;
	explosionEmitter.m_PAM.m_sizeVariation = 0.3f;

	explosionEmitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	explosionEmitter.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	explosionEmitter.m_PAM.m_lifeTime = 0.3f;
	explosionEmitter.m_RM.m_type = ParticleType::Circle;

	return tempEmitter;
}

void ParticleSystem_2D::ReuseExplosionEmitter(const std::shared_ptr<ExplosionEmitter_2D>& emitter, ImVec2 spawnPos)
{
	return ReuseExplosionEmitter(emitter, Imvec2_To_GlmVec2_Converter(spawnPos));
}

void ParticleSystem_2D::ReuseExplosionEmitter(const std::shared_ptr<ExplosionEmitter_2D>& emitter, glm::vec2 spawnPos)
{
	Emitter_2D& explosionEmitter = *emitter.get();

	// Clear all previous particle of the emitter
	explosionEmitter.ClearAllParticles();

	// Emitter_2D values - Without consideration for default ctor values
	explosionEmitter.m_GM.m_position = spawnPos;
	//explosionEmitter.m_GM.m_velocity.x = -500.0f;
	explosionEmitter.m_MM.m_duration = 2.f;
	explosionEmitter.m_GM.m_active = true;
	explosionEmitter.m_MM.m_preWarm = true;

	explosionEmitter.m_EM.m_spawnTimeInterval = 2.f; // 5x slower of dt
	explosionEmitter.m_EM.m_spawnCountTimer = explosionEmitter.m_EM.m_spawnTimeInterval;
	explosionEmitter.m_EM.m_rateOverTime = 20;
	explosionEmitter.m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	explosionEmitter.m_PAM.m_startVelocity = glm::vec2{ 0.f, 0.f };
	explosionEmitter.m_PAM.m_endVelocity = glm::vec2{ 0.f, 0.f };
	explosionEmitter.m_PAM.m_velocityVariation = glm::vec2{ 3.0f, 1.0f };

	explosionEmitter.m_PAM.m_sizeBegin = 10.0f;
	explosionEmitter.m_PAM.m_sizeEnd = 0.0f;
	explosionEmitter.m_PAM.m_sizeVariation = 0.3f;

	explosionEmitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	explosionEmitter.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	explosionEmitter.m_PAM.m_lifeTime = 0.3f;
	explosionEmitter.m_RM.m_type = ParticleType::Circle;
}

const std::weak_ptr<CircularMotionEmitter_2D> ParticleSystem_2D::CreateCircularMotionEmitter_2D(ImVec2 centrePos, float radius)
{
	return CreateCircularMotionEmitter_2D(Imvec2_To_GlmVec2_Converter(centrePos), radius);
}

const std::weak_ptr<CircularMotionEmitter_2D> ParticleSystem_2D::CreateCircularMotionEmitter_2D(glm::vec2 centrePos, float radius)
{
	auto tempEmitter = std::make_shared<CircularMotionEmitter_2D>();
	Emitter_2D& emitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Center position of the circle
	tempEmitter->m_centrePoint = centrePos;
	//tempEmitter->m_GM.m_position = centrePos;
	tempEmitter->m_radius = radius;

	// Emitter_2D values - Without consideration for default ctor values
	emitter.m_GM.m_velocity.x = 0.0f;
	emitter.m_MM.m_duration = 1000.0f;
	emitter.m_GM.m_active = true;
	emitter.m_MM.m_preWarm = true;
	emitter.m_MM.m_simulationSpeed = 0.016f;

	emitter.m_EM.m_spawnTimeInterval = 0.016f;
	emitter.m_EM.m_spawnCountTimer = emitter.m_EM.m_spawnTimeInterval;
	emitter.m_EM.m_rateOverTime = 1;
	emitter.m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	emitter.m_PAM.m_startVelocity = glm::vec2{ 0.f, 0.f };
	emitter.m_PAM.m_endVelocity = glm::vec2{ 0.f, 0.f };
	emitter.m_PAM.m_velocityVariation = glm::vec2{ 0.0f, 0.0f };

	emitter.m_PAM.m_sizeBegin = 8.0f;
	emitter.m_PAM.m_sizeEnd = 8.0f;
	emitter.m_PAM.m_sizeVariation = 0.0f;

	emitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	emitter.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	emitter.m_PAM.m_lifeTime = 1.0f;
	emitter.m_RM.m_type = ParticleType::Square;

	return tempEmitter;
}

void ParticleSystem_2D::ReuseCircularMotionEmitter_2D(const std::shared_ptr<CircularMotionEmitter_2D>& emitter, ImVec2 centrePos, float radius)
{
	ReuseCircularMotionEmitter_2D(emitter, Imvec2_To_GlmVec2_Converter(centrePos), radius);
}

void ParticleSystem_2D::ReuseCircularMotionEmitter_2D(const std::shared_ptr<CircularMotionEmitter_2D>& emitter, glm::vec2 centrePos, float radius)
{
	// Center position of the circle
	emitter->m_centrePoint = centrePos;
	//tempEmitter->m_GM.m_position = centrePos;
	emitter->m_radius = radius;

	// Emitter_2D values - Without consideration for default ctor values
	emitter->m_GM.m_velocity.x = 0.0f;
	emitter->m_MM.m_duration = 1000.0f;
	emitter->m_GM.m_active = true;
	emitter->m_MM.m_preWarm = true;
	emitter->m_MM.m_simulationSpeed = 0.016f;

	emitter->m_EM.m_spawnTimeInterval = 0.016f;
	emitter->m_EM.m_spawnCountTimer = emitter->m_EM.m_spawnTimeInterval;
	emitter->m_EM.m_rateOverTime = 1;
	emitter->m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	emitter->m_PAM.m_startVelocity = glm::vec2{ 0.f, 0.f };
	emitter->m_PAM.m_endVelocity = glm::vec2{ 0.f, 0.f };
	emitter->m_PAM.m_velocityVariation = glm::vec2{ 0.0f, 0.0f };

	emitter->m_PAM.m_sizeBegin = 8.0f;
	emitter->m_PAM.m_sizeEnd = 8.0f;
	emitter->m_PAM.m_sizeVariation = 0.0f;

	emitter->m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	emitter->m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	emitter->m_PAM.m_lifeTime = 1.0f;
	emitter->m_RM.m_type = ParticleType::Square;
}