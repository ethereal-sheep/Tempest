
// Main Header
#include "ParticleSystem_3D.h"

// Types of Emitters
#include "ExplosionEmitter_3D.h"
#include "SphereExplosionEmitter_3D.h"
#include "MultipleExplosionEmitter_3D.h"

#include "InteractiveParticle_3D.h"
#include "TileWaypointEmitter_3D.h"
#include "UnitTrailEmitter_3D.h"
#include "CharacterDamageEmitter_3D.h"
#include "CharacterDeathEmitter_3D.h"

ParticleSystem_3D::ParticleSystem_3D()
{}

ParticleSystem_3D& ParticleSystem_3D::GetInstance()
{
	static ParticleSystem_3D instance;
	return instance;
}

void ParticleSystem_3D::ClearEmitters()
{
	m_Emitters.clear();
}

void ParticleSystem_3D::Update(const float dt)
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

std::vector<std::shared_ptr<Emitter_3D>> ParticleSystem_3D::GetEmitter()
{
	return m_Emitters;
}

void ParticleSystem_3D::AddEmitter(const std::shared_ptr<Emitter_3D> emitter)
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

const std::weak_ptr<Emitter_3D> ParticleSystem_3D::CreateTestModelShapeEmitter(glm::vec3 spawnPos)
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
	tempEmitter->m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	tempEmitter->m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	tempEmitter->m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
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

const std::weak_ptr<ExplosionEmitter_3D> ParticleSystem_3D::CreateExplosionEmitter(glm::vec3 spawnPos)
{
	auto tempEmitter = std::make_shared<ExplosionEmitter_3D>();
	//ExplosionEmitter_3D& explosionEmitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Emitter_3D values - Without consideration for default ctor values
	tempEmitter->m_GM.m_position = spawnPos;
	//tempEmitter->m_GM.m_velocity.x = -500.0f;
	tempEmitter->m_MM.m_duration = 15.0f;
	tempEmitter->m_GM.m_active = true;
	tempEmitter->m_MM.m_preWarm = true;

	tempEmitter->m_EM.m_spawnTimeInterval = 1.0f;
	tempEmitter->m_EM.m_spawnCountTimer = tempEmitter->m_EM.m_spawnTimeInterval;
	tempEmitter->m_EM.m_rateOverTime = 20;
	tempEmitter->m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	tempEmitter->m_PAM.m_gravity = true;
	tempEmitter->m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	tempEmitter->m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	tempEmitter->m_PAM.m_velocityVariation = glm::vec3{ 10.0f, 10.0f, 10.0f };

	tempEmitter->m_PAM.m_scaleBegin = glm::vec3{ 0.5f, 0.5f, 0.5f };
	tempEmitter->m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	tempEmitter->m_PAM.m_scaleVariation = glm::vec3{ 0.3f, 0.3f, 0.3f };

	tempEmitter->m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f }; 
	tempEmitter->m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };

	tempEmitter->m_PAM.m_lifeTime = 10.0f;
	tempEmitter->m_RM.m_renderingPath = "Models/Cube.a";

	return tempEmitter;
}

const std::weak_ptr<MultipleExplosionEmitter_3D> ParticleSystem_3D::CreateMultipleExplosionEmitter(glm::vec3 spawnPos, glm::vec3 minSpawnPos, glm::vec3 maxSpawnPos, int explosionEmitterAmount)
{
	auto tempEmitter = std::make_shared<MultipleExplosionEmitter_3D>();

	// MultipleExplosionEmitter - Own Explosion
	// Emitter_3D values - Without consideration for default ctor values
	tempEmitter->m_GM.m_position = spawnPos;
	tempEmitter->m_minPos = minSpawnPos;
	tempEmitter->m_maxPos = maxSpawnPos;
	tempEmitter->m_MM.m_duration = 0.3f;
	tempEmitter->m_MM.m_simulationSpeed = 0.016f;
	tempEmitter->m_GM.m_active = true;
	tempEmitter->m_MM.m_preWarm = true;

	tempEmitter->m_EM.m_spawnTimeInterval = 10.0f;
	tempEmitter->m_EM.m_spawnCountTimer = tempEmitter->m_EM.m_spawnTimeInterval;
	tempEmitter->m_EM.m_rateOverTime = 20;
	tempEmitter->m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	tempEmitter->m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	tempEmitter->m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	tempEmitter->m_PAM.m_velocityVariation = glm::vec3{ 10.0f, 10.0f, 10.0f };

	tempEmitter->m_PAM.m_scaleBegin = glm::vec3{ 0.5f, 0.5f, 0.5f };
	tempEmitter->m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	tempEmitter->m_PAM.m_scaleVariation = glm::vec3{ 0.3f, 0.3f, 0.3f };

	//tempEmitter->m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	//tempEmitter->m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	tempEmitter->m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };
	tempEmitter->m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };

	tempEmitter->m_PAM.m_lifeTime = 0.3f;
	tempEmitter->m_RM.m_renderingPath = "Models/Cube.a";

	// Amount of Followup Explosion
	tempEmitter->m_explosionEmitterAmount = explosionEmitterAmount;
	AddEmitter(tempEmitter);

	return tempEmitter;
}

const std::weak_ptr<InteractiveParticle_3D> ParticleSystem_3D::CreateInteractiveParticle(glm::vec3 spawnPos, glm::vec3 minSpawnPos, glm::vec3 maxSpawnPos)
{
	auto tempEmitter = std::make_shared<InteractiveParticle_3D>();
	InteractiveParticle_3D& interactiveEmitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Emitter_3D values - Without consideration for default ctor values
	interactiveEmitter.m_GM.m_position = spawnPos;
	interactiveEmitter.minPos = minSpawnPos;
	interactiveEmitter.maxPos = maxSpawnPos;
	interactiveEmitter.m_GM.m_active = true;

	interactiveEmitter.m_MM.m_duration = 1000000.f;
	interactiveEmitter.m_MM.m_preWarm = true;

	interactiveEmitter.m_EM.m_spawnTimeInterval = 0.1f;
	interactiveEmitter.m_EM.m_spawnCountTimer = interactiveEmitter.m_EM.m_spawnTimeInterval;
	interactiveEmitter.m_EM.m_rateOverTime = 1;
	interactiveEmitter.m_MM.m_maxParticles = 30;

	// Particle Architype values - without consideration for default ctor
	interactiveEmitter.m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	interactiveEmitter.m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	interactiveEmitter.m_PAM.m_velocityVariation = glm::vec3{ 0.0f, 3.0f, 0.0f };

	interactiveEmitter.m_PAM.m_scaleBegin = glm::vec3{ 0.03f, 0.03f, 0.03f };
	interactiveEmitter.m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	interactiveEmitter.m_PAM.m_scaleVariation = glm::vec3{ 0.02f, 0.02f, 0.02f };

	//interactiveEmitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	//interactiveEmitter.m_PAM.m_colourEnd = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };
	
	// Note - Values to be divided by 255.0f - Forgot the reason
	interactiveEmitter.m_PAM.m_colourBegin = glm::vec4{ 0.996f, 0.831f, 0.482f, 1.0f };
	interactiveEmitter.m_PAM.m_colourEnd = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };

	interactiveEmitter.m_PAM.m_lifeTime = 1.0f;
	interactiveEmitter.m_PAM.m_rebirth = false;

	interactiveEmitter.m_RM.m_renderingPath = "Models/Cube.a";

	return tempEmitter;
}

const std::weak_ptr<TileWaypointEmitter_3D> ParticleSystem_3D::CreateTileWaypointEmitter(glm::vec3 spawnPos)
{
	auto tempEmitter = std::make_shared<TileWaypointEmitter_3D>();
	TileWaypointEmitter_3D& emitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Emitter_3D values - Without consideration for default ctor values
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
	emitter.m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.1f, 0.0f };
	emitter.m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	emitter.m_PAM.m_velocityVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	emitter.m_PAM.m_scaleBegin = glm::vec3{ 0.05f, 0.05f, 0.05f };
	emitter.m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	emitter.m_PAM.m_scaleVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	emitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	emitter.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	emitter.m_PAM.m_lifeTime = 0.3f;
	emitter.m_RM.m_renderingPath = "Models/Cube.a";

	// Center position of the tile
	emitter.m_GM.m_position = spawnPos;

	// Shift to one corner of the button - Left Bottom
	//emitter.m_GM.m_position.x -= buttonSize.x * 0.5f;
	//emitter.m_GM.m_position.y -= buttonSize.y * 0.5f;

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

const std::weak_ptr<UnitTrailEmitter_3D> ParticleSystem_3D::CreateUnitTrailEmitter(glm::vec3 spawnPos)
{
	auto tempEmitter = std::make_shared<UnitTrailEmitter_3D>();
	UnitTrailEmitter_3D& emitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	emitter.m_recalculateVelocity = true;

	// Emitter_3D values - Without consideration for default ctor values
	emitter.m_GM.m_position = spawnPos;
	emitter.m_GM.m_active = true;

	emitter.m_MM.m_duration = 1000.0f;
	emitter.m_MM.m_preWarm = true;
	emitter.m_MM.m_simulationSpeed = 0.016f;

	emitter.m_EM.m_spawnTimeInterval = 0.08f;
	emitter.m_EM.m_spawnCountTimer = emitter.m_EM.m_spawnTimeInterval;
	emitter.m_EM.m_rateOverTime = 1;
	emitter.m_MM.m_maxParticles = 1;

	// Particle Architype values - without consideration for default ctor
	emitter.m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	emitter.m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	emitter.m_PAM.m_velocityVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	emitter.m_PAM.m_scaleBegin = glm::vec3{ 0.1f, 0.1f, 0.1f };
	emitter.m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	emitter.m_PAM.m_scaleVariation = glm::vec3{ 0.0f, 0.0f, 0.0f };

	emitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	emitter.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	// Note - Values to be divided by 255.0f - Forgot the reason
	//emitter.m_PAM.m_colourBegin = glm::vec4{ 0.996f, 0.831f, 0.482f, 1.0f };
	//emitter.m_PAM.m_colourEnd = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };

	emitter.m_PAM.m_lifeTime = 1.0f;
	emitter.m_RM.m_renderingPath = "Models/Cube.a";

	emitter.m_wayPoints.push_back(glm::vec3{ -20.0f, 0.0f, 0.0f });
	emitter.m_wayPoints.push_back(glm::vec3{ -20.0f, 0.0f, 20.0f });
	
	return tempEmitter;
}

const std::weak_ptr<CharacterDamageEmitter_3D> ParticleSystem_3D::CreateChracterDamageEmitter(glm::vec3 spawnPos)
{
	auto tempEmitter = std::make_shared<CharacterDamageEmitter_3D>();
	CharacterDamageEmitter_3D& explosionEmitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Emitter_3D values - Without consideration for default ctor values
	tempEmitter->m_GM.m_position = spawnPos;
	//tempEmitter->m_GM.m_velocity.x = -500.0f;
	tempEmitter->m_MM.m_duration = 0.6f;
	tempEmitter->m_GM.m_active = true;
	tempEmitter->m_MM.m_preWarm = true;

	tempEmitter->m_EM.m_spawnTimeInterval = 1.0f; // 5x slower of dt
	tempEmitter->m_EM.m_spawnCountTimer = tempEmitter->m_EM.m_spawnTimeInterval;
	tempEmitter->m_EM.m_rateOverTime = 0;
	tempEmitter->m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	tempEmitter->m_PAM.m_gravity = true;
	tempEmitter->m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	tempEmitter->m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	tempEmitter->m_PAM.m_velocityVariation = glm::vec3{ 1.0f, -20.0f, 1.0f };

	tempEmitter->m_PAM.m_scaleBegin = glm::vec3{ 0.03f, 0.03f, 0.03f };
	tempEmitter->m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	tempEmitter->m_PAM.m_scaleVariation = glm::vec3{ 0.02f, 0.02f, 0.02f };

	tempEmitter->m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };
	tempEmitter->m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };

	tempEmitter->m_PAM.m_lifeTime = 2.0f;

	tempEmitter->m_EM.m_burstCycle = 1;
	tempEmitter->m_EM.m_burstTime = 0.01f;
	tempEmitter->m_EM.m_burstCount = 20;

	// Rendering Module
	tempEmitter->m_RM.m_renderingPath = "Models/Cube.a";

	return tempEmitter;
}

const std::weak_ptr<CharacterDeathEmitter_3D> ParticleSystem_3D::CreateChracterDeathEmitter(glm::vec3 spawnPos, glm::vec3 minSpawnPos, glm::vec3 maxSpawnPos, int explosionEmitterAmount)
{
	auto tempEmitter = std::make_shared<CharacterDeathEmitter_3D>();

	// MultipleExplosionEmitter - Own Explosion
	// Emitter_3D values - Without consideration for default ctor values
	tempEmitter->m_GM.m_position = spawnPos;
	tempEmitter->m_minPos = minSpawnPos;
	tempEmitter->m_maxPos = maxSpawnPos;
	tempEmitter->m_MM.m_duration = 0.3f;
	tempEmitter->m_MM.m_simulationSpeed = 0.016f;
	tempEmitter->m_GM.m_active = true;
	tempEmitter->m_MM.m_preWarm = true;

	tempEmitter->m_EM.m_spawnTimeInterval = 10.0f;
	tempEmitter->m_EM.m_spawnCountTimer = tempEmitter->m_EM.m_spawnTimeInterval;
	tempEmitter->m_EM.m_rateOverTime = 20;
	tempEmitter->m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	tempEmitter->m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	tempEmitter->m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	tempEmitter->m_PAM.m_velocityVariation = glm::vec3{ 10.0f, 10.0f, 10.0f };

	tempEmitter->m_PAM.m_scaleBegin = glm::vec3{ 0.03f, 0.03f, 0.03f };
	tempEmitter->m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	tempEmitter->m_PAM.m_scaleVariation = glm::vec3{ 0.02f, 0.02f, 0.02f };

	//tempEmitter->m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	//tempEmitter->m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	tempEmitter->m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };
	tempEmitter->m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };

	tempEmitter->m_PAM.m_lifeTime = 0.3f;
	tempEmitter->m_RM.m_renderingPath = "Models/Cube.a";

	// Amount of Followup Explosion
	tempEmitter->m_explosionEmitterAmount = explosionEmitterAmount;
	AddEmitter(tempEmitter);

	return tempEmitter;
}