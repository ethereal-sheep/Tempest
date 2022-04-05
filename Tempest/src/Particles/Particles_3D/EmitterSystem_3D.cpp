

// C++ lib


// Main Header
#include "EmitterSystem_3D.h"

// General VFX
#include "ExplosionEmitter_3D.h"
#include "Rotation_ExplosionEmitter_3D.h"
#include "MultipleExplosionEmitter_3D.h"
#include "Multiple_Rotation_ExplosionEmitter_3D.h"

// Indicator/Feedback VFX w.r.t the user
#include "Interactive_DoorParticle_3D.h"
#include "Smoke_Poof_Emitter_3D.h"

// Unit VFX
#include "Unit_Turn_IndicatorEmitter_3D.h"
#include "Unit_MovementTrailEmitter_3D.h"

// Characters Spawning VFX
#include "CharacterSpawnEmitter_3D.h"
#include "CharacterDespawnEmitter_3D.h"

// Character Actions VFX
#include "CharacterDamageEmitter_3D.h"
#include "CharacterDeathEmitter_3D.h"

#include "CharacterTileCharged_Emitter_3D.h"

// Weather VFX
#include "Weather_Rain_Emitter_3D.h"

#include "Logger/Log.h"


EmitterSystem_3D::EmitterSystem_3D()
{}

EmitterSystem_3D& EmitterSystem_3D::GetInstance()
{
	static EmitterSystem_3D instance;
	return instance;
}

void EmitterSystem_3D::ClearEmitters()
{
	m_Emitters.clear();
}

void EmitterSystem_3D::Update(const float dt)
{
	// Update all the emitters here
	for (short i = 0; i < m_Emitters.size(); ++i)
	{
		// Check for null pointer - shouldn't happen
		if (m_Emitters[i])
		{
			Emitter_3D& emitter = *m_Emitters[i];

			// Update active emitter
			if (emitter.m_GM.m_active)
				emitter.Update(dt);
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

std::vector<std::shared_ptr<Emitter_3D>> EmitterSystem_3D::GetEmitter()
{
	return m_Emitters;
}

void EmitterSystem_3D::AddEmitter(const std::shared_ptr<Emitter_3D> emitter)
{
	// There is available slot
	//if (m_AvailableEmitterSlots.size())
	//{
	//	// The index of the free slot
	//	short freeSlot = m_AvailableEmitterSlots.front();

	//	// Update the emitter memory management
	//	m_AvailableEmitterSlots.pop();
	//	m_UniqueEmitterSlots.erase(freeSlot);

	//	// Allocate the emitter, destructor is handled s_ptr
	//	m_Emitters[freeSlot] = emitter;
	//}
	//else
		m_Emitters.push_back(emitter);
}

const std::weak_ptr<Emitter_3D> EmitterSystem_3D::CreateTestModelShapeEmitter(glm::vec3 spawnPos)
{
	auto tempEmitter = std::make_shared<Emitter_3D>();
	AddEmitter(tempEmitter);

	// Emitter_3D values - Without consideration for default ctor values
	tempEmitter->m_GM.m_position = spawnPos;
	//tempEmitter->m_GM.m_velocity.x = -500.0f;
	tempEmitter->m_MM.m_duration = 20.0f;
	tempEmitter->m_GM.m_active = true;
	tempEmitter->m_MM.m_preWarm = true;

	tempEmitter->m_EM.m_spawnTimeInterval = 1.0f; // 5x slower of dt
	tempEmitter->m_EM.m_spawnCountTimer = tempEmitter->m_EM.m_spawnTimeInterval;
	tempEmitter->m_EM.m_rateOverTime = 1;

	// Particle Architype values - without consideration for default ctor
	tempEmitter->m_PAM.m_velocityStart = glm::vec3{ 0.f, 0.f, 0.0f };
	tempEmitter->m_PAM.m_velocityEnd = glm::vec3{ 0.f, 0.f, 0.0f };
	tempEmitter->m_PAM.m_velocityVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	tempEmitter->m_PAM.m_scaleBegin = glm::vec3{ 1.0f, 1.0f, 1.0f };
	tempEmitter->m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	tempEmitter->m_PAM.m_scaleVariation = glm::vec3{ 0.0f, 1.0f, 0.0f };

	// Note - Values to be divided by 255.0f - Forgot the reason
	tempEmitter->m_PAM.m_colourBegin = glm::vec4{ 0.996f, 0.831f, 0.482f, 1.0f };
	tempEmitter->m_PAM.m_colourEnd = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };

	tempEmitter->m_PAM.m_lifeTime = 1.0f;
	tempEmitter->m_RM.m_renderingPath = "Models/SquareHole.a";

	return tempEmitter;
}

const std::weak_ptr<ExplosionEmitter_3D> EmitterSystem_3D::CreateExplosionEmitter(glm::vec3 spawnPos)
{
	auto tempEmitter = std::make_shared<ExplosionEmitter_3D>();
	AddEmitter(tempEmitter);

	// Customise values to Setup with
	tempEmitter->m_GM.m_position = spawnPos;
	return tempEmitter;
}

const std::weak_ptr<Rotation_ExplosionEmitter_3D> EmitterSystem_3D::CreateRotationExplosionEmitter(glm::vec3 spawnPos)
{
	auto tempEmitter = std::make_shared<Rotation_ExplosionEmitter_3D>();
	AddEmitter(tempEmitter);

	tempEmitter->m_GM.m_position = spawnPos;

	return tempEmitter;
}

const std::weak_ptr<MultipleExplosionEmitter_3D> EmitterSystem_3D::CreateMultipleExplosionEmitter(glm::vec3 spawnPos, glm::vec3 minSpawnPos, glm::vec3 maxSpawnPos, int explosionEmitterAmount)
{
	auto tempEmitter = std::make_shared<MultipleExplosionEmitter_3D>();

	tempEmitter->m_GM.m_position = spawnPos;
	tempEmitter->m_minPos = minSpawnPos;
	tempEmitter->m_maxPos = maxSpawnPos;

	tempEmitter->m_explosionEmitterAmount = explosionEmitterAmount;
	AddEmitter(tempEmitter);

	return tempEmitter;
}

const std::weak_ptr<Multiple_Rotation_ExplosionEmitter_3D> EmitterSystem_3D::CreateMultipleRotationExplosionEmitter(glm::vec3 spawnPos, glm::vec3 minSpawnPos, glm::vec3 maxSpawnPos, int explosionEmitterAmount)
{
	auto tempEmitter = std::make_shared<Multiple_Rotation_ExplosionEmitter_3D>();

	tempEmitter->m_GM.m_position = spawnPos;
	tempEmitter->m_minPos = minSpawnPos;
	tempEmitter->m_maxPos = maxSpawnPos;

	tempEmitter->m_explosionEmitterAmount = explosionEmitterAmount;
	AddEmitter(tempEmitter);

	return tempEmitter;
}

const std::weak_ptr<Interactive_DoorParticle_3D> EmitterSystem_3D::CreateInteractiveParticle(glm::vec3 spawnPos, glm::vec3 minSpawnPos, glm::vec3 maxSpawnPos)
{
	auto tempEmitter = std::make_shared<Interactive_DoorParticle_3D>();
	Interactive_DoorParticle_3D& interactiveEmitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Swap positions of max and min if wrong value
	if (minSpawnPos.x > maxSpawnPos.x)
		std::swap(minSpawnPos.x, maxSpawnPos.x);

	if (minSpawnPos.y > maxSpawnPos.y)
		std::swap(minSpawnPos.y, maxSpawnPos.y);

	if (minSpawnPos.z > maxSpawnPos.z)
		std::swap(minSpawnPos.z, maxSpawnPos.z);

	// Emitter_3D values - Without consideration for default ctor values
	interactiveEmitter.m_GM.m_position = spawnPos;
	interactiveEmitter.m_PAM.m_minSpawnPos = minSpawnPos;
	interactiveEmitter.m_PAM.m_maxSpawnPos = maxSpawnPos;
	
	return tempEmitter;
}

const std::weak_ptr<Unit_Turn_IndicatorEmitter_3D> EmitterSystem_3D::CreateTileWaypointEmitter(glm::vec3 spawnPos)
{
	auto tempEmitter = std::make_shared<Unit_Turn_IndicatorEmitter_3D>();
	Unit_Turn_IndicatorEmitter_3D& emitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Center position of the tile
	emitter.m_GM.m_position = spawnPos;

	// Assume its a unit tile
	glm::vec3 wp_LeftBtm = spawnPos;
	wp_LeftBtm = glm::vec3{ spawnPos.x - 0.5f, spawnPos.y + 0.1f, spawnPos.z - 0.5f };

	glm::vec3 wp_RightBtm = wp_LeftBtm;
	wp_RightBtm.x += 1.0f;

	glm::vec3 wp_RightTop = wp_RightBtm;
	wp_RightTop.z += 1.0f;

	glm::vec3 wp_LeftTop = wp_RightTop;
	wp_LeftTop.x -= 1.0f;

	// Add the waypoints
	emitter.m_wayPoints.push_back(wp_RightBtm);
	emitter.m_wayPoints.push_back(wp_RightTop);
	emitter.m_wayPoints.push_back(wp_LeftTop);
	emitter.m_wayPoints.push_back(wp_LeftBtm);

	//LOG_INFO("Given Value x: {0}, y: {1}", topLeftPos.x, topLeftPos.y);
	//LOG_INFO("Button Size x: {0}, y: {1}", buttonSize.x, buttonSize.y);
	//LOG_INFO("Btm Left  x: {0}, y: {1}", wp_LeftBottom.x, wp_LeftBottom.y);
	//LOG_INFO("Btm Right x: {0}, y: {1}", wp_RightBottom.x, wp_RightBottom.y);
	//LOG_INFO("Top Right x: {0}, y: {1}", wp_RightTop.x, wp_RightTop.y);
	//LOG_INFO("Top Left  x: {0}, y: {1}", wp_LeftTop.x, wp_LeftTop.y);

	return tempEmitter;
}

const std::weak_ptr<Unit_MovementTrailEmitter_3D> EmitterSystem_3D::CreateUnitTrailEmitter(glm::vec3 spawnPos)
{
	auto tempEmitter = std::make_shared<Unit_MovementTrailEmitter_3D>();
	Unit_MovementTrailEmitter_3D& emitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Emitter_3D values - Without consideration for default ctor values
	emitter.m_GM.m_position = spawnPos;

	return tempEmitter;
}

void EmitterSystem_3D::CharacterSpawnEmitter(std::weak_ptr<CharacterSpawnEmitter_3D>& wk_ptr, glm::vec3 spawnPos)
{
	if (wk_ptr.expired())
	{
		auto sh_ptr = std::make_shared<CharacterSpawnEmitter_3D>();
		wk_ptr = sh_ptr;
		AddEmitter(sh_ptr);
	}
	else
	{
		wk_ptr.lock()->ClearAllParticles();
	}

	spawnPos.y += 2.0f;

	// Emitter_3D values - Without consideration for default ctor values
	wk_ptr.lock()->m_GM.m_position = spawnPos;
	wk_ptr.lock()->m_MM.m_duration = 5.0f;
	wk_ptr.lock()->m_GM.m_active = true;

	wk_ptr.lock()->m_EM.m_burstInterval = 0.064f;
	wk_ptr.lock()->m_EM.m_burstCount = 4;
	wk_ptr.lock()->m_EM.m_burstCycle = 8;

	// Assign waypoints
	std::array<glm::vec3, 4> waypoints;

	/*
	* 0 - Left
	* 1 - Right
	* 2 - Top
	* 3 - Btm
	*/
	waypoints[0] = glm::vec3{ spawnPos.x       ,  spawnPos.y, spawnPos.z - 0.1f };
	waypoints[1] = glm::vec3{ spawnPos.x       ,  spawnPos.y, spawnPos.z + 0.1f };
	waypoints[2] = glm::vec3{ spawnPos.x + 0.1f,  spawnPos.y, spawnPos.z };
	waypoints[3] = glm::vec3{ spawnPos.x - 0.1f,  spawnPos.y, spawnPos.z };

	wk_ptr.lock()->AssignWaypoint(waypoints);
}

void EmitterSystem_3D::CreateChracterDespawnEmitter(std::weak_ptr<CharacterDespawnEmitter_3D>& wk_ptr, glm::vec3 spawnPos)
{
	if (wk_ptr.expired())
	{
		auto sh_ptr = std::make_shared<CharacterDespawnEmitter_3D>();
		wk_ptr = sh_ptr;
		AddEmitter(sh_ptr);
	}
	else
	{
		wk_ptr.lock()->ClearAllParticles();
	}

	// Emitter_3D values - Without consideration for default ctor values
	wk_ptr.lock()->m_GM.m_position = spawnPos;
	wk_ptr.lock()->m_MM.m_duration = 5.0f;
	wk_ptr.lock()->m_GM.m_active = true;

	wk_ptr.lock()->m_EM.m_burstInterval = 0.064f;
	wk_ptr.lock()->m_EM.m_burstCount = 4;
	wk_ptr.lock()->m_EM.m_burstCycle = 8;

	// Assign waypoints
	std::array<glm::vec3, 4> waypoints;

	/*
	* 0 - Left
	* 1 - Right
	* 2 - Top
	* 3 - Btm
	*/
	waypoints[0] = glm::vec3{ spawnPos.x       ,  spawnPos.y, spawnPos.z - 0.5f };
	waypoints[1] = glm::vec3{ spawnPos.x       ,  spawnPos.y, spawnPos.z + 0.5f };
	waypoints[2] = glm::vec3{ spawnPos.x + 0.5f,  spawnPos.y, spawnPos.z        };
	waypoints[3] = glm::vec3{ spawnPos.x - 0.5f,  spawnPos.y, spawnPos.z        };

	wk_ptr.lock()->AssignWaypoint(waypoints);
}

const std::weak_ptr<CharacterDamageEmitter_3D> EmitterSystem_3D::CreateChracterDamageEmitter(glm::vec3 spawnPos, glm::vec4 colourBegin, glm::vec4 colourEnd)
{
	auto tempEmitter = std::make_shared<CharacterDamageEmitter_3D>();
	AddEmitter(tempEmitter);

	// Emitter_3D values - Without consideration for default ctor values
	tempEmitter->m_GM.m_position = spawnPos;
	
	tempEmitter->m_PAM.m_colourBegin = colourBegin;
	tempEmitter->m_PAM.m_colourEnd = colourEnd;

	return tempEmitter;
}

const std::weak_ptr<CharacterDeathEmitter_3D> EmitterSystem_3D::CreateChracterDeathEmitter(glm::vec3 spawnPos, glm::vec3 minSpawnPos, glm::vec3 maxSpawnPos, int explosionEmitterAmount, glm::vec4 colourBegin, glm::vec4 colourEnd)
{
	auto tempEmitter = std::make_shared<CharacterDeathEmitter_3D>();

	tempEmitter->m_GM.m_position = spawnPos;
	tempEmitter->m_minPos = minSpawnPos;
	tempEmitter->m_maxPos = maxSpawnPos;

	tempEmitter->m_PAM.m_colourBegin = colourBegin;
	tempEmitter->m_PAM.m_colourEnd = colourEnd;

	// Amount of Followup Explosion
	tempEmitter->m_explosionEmitterAmount = explosionEmitterAmount;
	AddEmitter(tempEmitter);

	return tempEmitter;
}

const std::weak_ptr<Weather_Rain_Emitter_3D> EmitterSystem_3D::CreateWeatherRainEmitter(glm::vec3 spawnPos, glm::vec3 minSpawnPos, glm::vec3 maxSpawnPos)
{
	auto tempEmitter = std::make_shared<Weather_Rain_Emitter_3D>();
	Weather_Rain_Emitter_3D& interactiveEmitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Swap positions of max and min if wrong value
	if (minSpawnPos.x > maxSpawnPos.x)
		std::swap(minSpawnPos.x, maxSpawnPos.x);

	if (minSpawnPos.y > maxSpawnPos.y)
		std::swap(minSpawnPos.y, maxSpawnPos.y);

	if (minSpawnPos.z > maxSpawnPos.z)
		std::swap(minSpawnPos.z, maxSpawnPos.z);

	// Emitter_3D values - Without consideration for default ctor values
	interactiveEmitter.m_GM.m_position = spawnPos;
	interactiveEmitter.m_PAM.m_minSpawnPos = minSpawnPos;
	interactiveEmitter.m_PAM.m_maxSpawnPos = maxSpawnPos;

	return tempEmitter;
}

void EmitterSystem_3D::CreateChracterChargedAttackEmitter(std::weak_ptr<CharacterTileCharged_Emitter_3D>& wk_ptr, glm::vec3 spawnPos)
{
	if (wk_ptr.expired())
	{
		auto sh_ptr = std::make_shared<CharacterTileCharged_Emitter_3D>();
		wk_ptr = sh_ptr;
		AddEmitter(sh_ptr);
	}
	else
	{
		wk_ptr.lock()->ClearAllParticles();
	}

	// Emitter_3D values - Without consideration for default ctor values
	wk_ptr.lock()->m_GM.m_position = spawnPos;
	wk_ptr.lock()->m_MM.m_duration = 60.0f;
	wk_ptr.lock()->m_GM.m_active = true;

	// Edit the position for waypoint - spawnPos is the middle of the tile;
	glm::vec3 wp_btm_left_Pos = spawnPos;
	wp_btm_left_Pos.x -= 0.5f;
	wp_btm_left_Pos.z -= 0.5f;

	// Assign waypoints
	std::array<glm::vec3, 4> waypoints;
	waypoints[0] = wp_btm_left_Pos;
	waypoints[1] = glm::vec3{ wp_btm_left_Pos.x,        wp_btm_left_Pos.y, wp_btm_left_Pos.z + 1.0f };
	waypoints[2] = glm::vec3{ wp_btm_left_Pos.x + 1.0f, wp_btm_left_Pos.y, wp_btm_left_Pos.z + 1.0f };
	waypoints[3] = glm::vec3{ wp_btm_left_Pos.x + 1.0f, wp_btm_left_Pos.y, wp_btm_left_Pos.z };

	wk_ptr.lock()->AssignWaypoint(waypoints);
}

void EmitterSystem_3D::CreateSmokePoofEmitter(std::weak_ptr<Smoke_Poof_Emitter_3D>& wk_ptr, glm::vec3 spawnPos)
{
	if (wk_ptr.expired())
	{
		auto sh_ptr = std::make_shared<Smoke_Poof_Emitter_3D>();
		wk_ptr = sh_ptr;
		AddEmitter(sh_ptr);
	}
	else
	{
		wk_ptr.lock()->ClearAllParticles();
	}

	// Emitter_3D values - Without consideration for default ctor values
	wk_ptr.lock()->m_GM.m_position = spawnPos;
	wk_ptr.lock()->m_MM.m_duration = 0.8f;
	wk_ptr.lock()->m_GM.m_active = true;
	wk_ptr.lock()->m_MM.m_preWarm = true;
	wk_ptr.lock()->m_EM.m_spawnTimeInterval = 1.0f;
	wk_ptr.lock()->m_EM.m_spawnCountTimer = wk_ptr.lock()->m_EM.m_spawnTimeInterval;
}