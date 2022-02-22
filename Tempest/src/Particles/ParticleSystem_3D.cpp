/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "ParticleSystem_3D.h"

#include "Trail_WaypointEmitter_3D.h"
#include "CircularMotionEmitter_2D.h"
#include "ExplosionEmitter_3D.h"

// Debugging purpose
#include "Logger/Log.h"

// For conversion of Imvec 2
#include "../../Editor/src/Extern/imgui/imgui.h"

// Convert Imvec2 -> glm::vec3
auto Imvec2_To_GlmVec3Converter = [](ImVec2 imvec)
{
	glm::vec3 tempVec;
	tempVec.x = imvec.x;
	tempVec.y = imvec.y;
	tempVec.z = 0.0f;

	return tempVec;
};

ParticleSystem_3D::ParticleSystem_3D()
{}

ParticleSystem_3D& ParticleSystem_3D::GetInstance()
{
	static ParticleSystem_3D instance;
	return instance;
}

void ParticleSystem_3D::Update()
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

void ParticleSystem_3D::AddEmitter(const std::shared_ptr<Emitter> emitter)
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

const std::weak_ptr<Trail_WaypointEmitter_3D> ParticleSystem_3D::CreateButtonEmitter(ImVec2 topLeftPos, ImVec2 buttonSize)
{
	return CreateButtonEmitter(Imvec2_To_GlmVec3Converter(topLeftPos), Imvec2_To_GlmVec3Converter(buttonSize));
}

const std::weak_ptr<Trail_WaypointEmitter_3D> ParticleSystem_3D::CreateButtonEmitter(glm::vec3 topLeftPos, glm::vec3 buttonSize)
{
	auto tempEmitter = std::make_shared<Trail_WaypointEmitter_3D>();
	Trail_WaypointEmitter_3D& emitter = *tempEmitter.get();
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
	emitter.m_PAM.m_startVelocity     = glm::vec3{ 0.f, 0.f, 0.0f };
	emitter.m_PAM.m_endVelocity       = glm::vec3{ 0.f, 0.f, 0.0f };
	emitter.m_PAM.m_velocityVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	emitter.m_PAM.m_sizeBegin = 3.0f;
	emitter.m_PAM.m_sizeEnd = 3.0f;
	emitter.m_PAM.m_sizeVariation = 0.0f;

	emitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	emitter.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	emitter.m_PAM.m_lifeTime = 2.0f;
	emitter.m_RM.m_type = ParticleType::Circle;

	// Center position of the button
	emitter.m_GM.m_position = glm::vec3{ topLeftPos.x, topLeftPos.y, 0.0f };

	// Shift to one corner of the button - Left Bottom
	//emitter.m_GM.m_position.x -= buttonSize.x * 0.5f;
	//emitter.m_GM.m_position.y -= buttonSize.y * 0.5f;

	// Prepare the waypoints
	glm::vec3 wp_LeftTop = topLeftPos;

	glm::vec3 wp_RightTop = wp_LeftTop;
	wp_RightTop.x += buttonSize.x;

	glm::vec3 wp_LeftBottom = wp_LeftTop;
	wp_LeftBottom.y += buttonSize.y;

	glm::vec3 wp_RightBottom = wp_RightTop;
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

void ParticleSystem_3D::ReuseButtonEmitter(const std::shared_ptr<Trail_WaypointEmitter_3D>& emitter, ImVec2 topLeftPos, ImVec2 buttonSize)
{
	return ReuseButtonEmitter(emitter, Imvec2_To_GlmVec3Converter(topLeftPos), Imvec2_To_GlmVec3Converter(buttonSize));
}

void ParticleSystem_3D::ReuseButtonEmitter(const std::shared_ptr<Trail_WaypointEmitter_3D>& emitter, glm::vec3 topLeftPos, glm::vec3 buttonSize)
{
	Trail_WaypointEmitter_3D& waypointEmitter = *emitter.get();

	// Clear all previous particle of the emitter
	waypointEmitter.ClearAllParticles();

	// Emitter values - Without consideration for default ctor values
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
	waypointEmitter.m_PAM.m_startVelocity     = glm::vec3{ 0.f, 0.f, 0.0f };
	waypointEmitter.m_PAM.m_endVelocity       = glm::vec3{ 0.f, 0.f, 0.0f };
	waypointEmitter.m_PAM.m_velocityVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	waypointEmitter.m_PAM.m_sizeBegin = 3.0f;
	waypointEmitter.m_PAM.m_sizeEnd = 3.0f;
	waypointEmitter.m_PAM.m_sizeVariation = 0.0f;

	waypointEmitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	waypointEmitter.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	waypointEmitter.m_PAM.m_lifeTime = 2.0f;
	waypointEmitter.m_RM.m_type = ParticleType::Circle;

	// Center position of the button
	waypointEmitter.m_GM.m_position = glm::vec3{ topLeftPos.x, topLeftPos.y, 0.0f };

	// Shift to one corner of the button - Left Bottom
	//waypointEmitter.m_GM.m_position.x -= buttonSize.x * 0.5f;
	//waypointEmitter.m_GM.m_position.y -= buttonSize.y * 0.5f;

	// Prepare the waypoints
	glm::vec3 wp_LeftTop = topLeftPos;

	glm::vec3 wp_RightTop = wp_LeftTop;
	wp_RightTop.x += buttonSize.x;

	glm::vec3 wp_LeftBottom = wp_LeftTop;
	wp_LeftBottom.y += buttonSize.y;

	glm::vec3 wp_RightBottom = wp_RightTop;
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

const std::weak_ptr<ExplosionEmitter_3D> ParticleSystem_3D::CreateExplosionEmitter_3D(ImVec2 spawnPos)
{
	return CreateExplosionEmitter_3D(Imvec2_To_GlmVec3Converter(spawnPos));
}

const std::weak_ptr<ExplosionEmitter_3D> ParticleSystem_3D::CreateExplosionEmitter_3D(glm::vec3 spawnPos)
{
	auto tempEmitter = std::make_shared<ExplosionEmitter_3D>();
	ExplosionEmitter_3D& ExplosionEmitter_3D = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Emitter values - Without consideration for default ctor values
	ExplosionEmitter_3D.m_GM.m_position = spawnPos;
	//ExplosionEmitter_3D.m_GM.m_velocity.x = -500.0f;
	ExplosionEmitter_3D.m_MM.m_duration = 2.f;
	ExplosionEmitter_3D.m_GM.m_active = true;
	ExplosionEmitter_3D.m_MM.m_preWarm = true;

	ExplosionEmitter_3D.m_EM.m_spawnTimeInterval = 2.f; // 5x slower of dt
	ExplosionEmitter_3D.m_EM.m_spawnCountTimer = ExplosionEmitter_3D.m_EM.m_spawnTimeInterval;
	ExplosionEmitter_3D.m_EM.m_rateOverTime = 20;
	ExplosionEmitter_3D.m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	ExplosionEmitter_3D.m_PAM.m_startVelocity     = glm::vec3{ 0.f, 0.f, 0.0f };
	ExplosionEmitter_3D.m_PAM.m_endVelocity       = glm::vec3{ 0.f, 0.f, 0.0f };
	ExplosionEmitter_3D.m_PAM.m_velocityVariation = glm::vec3{ 3.0f, 1.0f, 0.0f };

	ExplosionEmitter_3D.m_PAM.m_sizeBegin = 10.0f;
	ExplosionEmitter_3D.m_PAM.m_sizeEnd = 0.0f;
	ExplosionEmitter_3D.m_PAM.m_sizeVariation = 0.3f;

	ExplosionEmitter_3D.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	ExplosionEmitter_3D.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	ExplosionEmitter_3D.m_PAM.m_lifeTime = 0.3f;
	ExplosionEmitter_3D.m_RM.m_type = ParticleType::Circle;

	return tempEmitter;
}

void ParticleSystem_3D::ReuseExplosionEmitter_3D(const std::shared_ptr<ExplosionEmitter_3D>& emitter, ImVec2 spawnPos)
{
	return ReuseExplosionEmitter_3D(emitter, Imvec2_To_GlmVec3Converter(spawnPos));
}

void ParticleSystem_3D::ReuseExplosionEmitter_3D(const std::shared_ptr<ExplosionEmitter_3D>& emitter, glm::vec3 spawnPos)
{
	Emitter& ExplosionEmitter_3D = *emitter.get();

	// Clear all previous particle of the emitter
	ExplosionEmitter_3D.ClearAllParticles();

	// Emitter values - Without consideration for default ctor values
	ExplosionEmitter_3D.m_GM.m_position = spawnPos;
	//ExplosionEmitter_3D.m_GM.m_velocity.x = -500.0f;
	ExplosionEmitter_3D.m_MM.m_duration = 2.f;
	ExplosionEmitter_3D.m_GM.m_active = true;
	ExplosionEmitter_3D.m_MM.m_preWarm = true;

	ExplosionEmitter_3D.m_EM.m_spawnTimeInterval = 2.f; // 5x slower of dt
	ExplosionEmitter_3D.m_EM.m_spawnCountTimer = ExplosionEmitter_3D.m_EM.m_spawnTimeInterval;
	ExplosionEmitter_3D.m_EM.m_rateOverTime = 20;
	ExplosionEmitter_3D.m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	ExplosionEmitter_3D.m_PAM.m_startVelocity		= glm::vec3{ 0.0f, 0.0f, 0.0f };
	ExplosionEmitter_3D.m_PAM.m_endVelocity		= glm::vec3{ 0.0f, 0.0f, 0.0f };
	ExplosionEmitter_3D.m_PAM.m_velocityVariation	= glm::vec3{ 3.0f, 1.0f, 0.0f };

	ExplosionEmitter_3D.m_PAM.m_sizeBegin = 10.0f;
	ExplosionEmitter_3D.m_PAM.m_sizeEnd = 0.0f;
	ExplosionEmitter_3D.m_PAM.m_sizeVariation = 0.3f;

	ExplosionEmitter_3D.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	ExplosionEmitter_3D.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	ExplosionEmitter_3D.m_PAM.m_lifeTime = 0.3f;
	ExplosionEmitter_3D.m_RM.m_type = ParticleType::Circle;
}

const std::weak_ptr<CircularMotionEmitter_2D> ParticleSystem_3D::CreateCircularMotionEmitter_2D(ImVec2 centrePos, float radius)
{
	return CreateCircularMotionEmitter_2D(Imvec2_To_GlmVec3Converter(centrePos), radius);
}

const std::weak_ptr<CircularMotionEmitter_2D> ParticleSystem_3D::CreateCircularMotionEmitter_2D(glm::vec3 centrePos, float radius)
{
	auto tempEmitter = std::make_shared<CircularMotionEmitter_2D>();
	Emitter& emitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Center position of the circle
	tempEmitter->m_centrePoint = centrePos;
	//tempEmitter->m_GM.m_position = centrePos;
	tempEmitter->m_radius = radius;

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

	// Particle Architype values - without consideration for default ctor
	emitter.m_PAM.m_startVelocity		= glm::vec3{ 0.0f, 0.0f, 0.0f };
	emitter.m_PAM.m_endVelocity			= glm::vec3{ 0.0f, 0.0f, 0.0f };
	emitter.m_PAM.m_velocityVariation	= glm::vec3{ 0.0f, 0.0f, 0.0f };

	emitter.m_PAM.m_sizeBegin = 8.0f;
	emitter.m_PAM.m_sizeEnd = 8.0f;
	emitter.m_PAM.m_sizeVariation = 0.0f;

	emitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	emitter.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	emitter.m_PAM.m_lifeTime = 1.0f;
	emitter.m_RM.m_type = ParticleType::Square;

	return tempEmitter;
}

void ParticleSystem_3D::ReuseCircularMotionEmitter_2D(const std::shared_ptr<CircularMotionEmitter_2D>& emitter, ImVec2 centrePos, float radius)
{
	ReuseCircularMotionEmitter_2D(emitter, Imvec2_To_GlmVec3Converter(centrePos), radius);
}

void ParticleSystem_3D::ReuseCircularMotionEmitter_2D(const std::shared_ptr<CircularMotionEmitter_2D>& emitter, glm::vec3 centrePos, float radius)
{
	// Center position of the circle
	emitter->m_centrePoint = centrePos;
	//tempEmitter->m_GM.m_position = centrePos;
	emitter->m_radius = radius;

	// Emitter values - Without consideration for default ctor values
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
	emitter->m_PAM.m_startVelocity		= glm::vec3{ 0.0f, 0.0f, 0.0f };
	emitter->m_PAM.m_endVelocity		= glm::vec3{ 0.0f, 0.0f, 0.0f };
	emitter->m_PAM.m_velocityVariation	= glm::vec3{ 0.0f, 0.0f, 0.0f };

	emitter->m_PAM.m_sizeBegin = 8.0f;
	emitter->m_PAM.m_sizeEnd = 8.0f;
	emitter->m_PAM.m_sizeVariation = 0.0f;

	emitter->m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	emitter->m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	emitter->m_PAM.m_lifeTime = 1.0f;
	emitter->m_RM.m_type = ParticleType::Square;
}