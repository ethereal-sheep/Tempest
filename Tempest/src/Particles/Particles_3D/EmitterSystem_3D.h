


#ifndef EMITTER_SYSTEM_3D_H
#define EMITTER_SYSTEM_3D_H

#include "Core.h"
#include "Emitter_3D.h"

#include <glm.hpp>

// Forward Declarations
struct ExplosionEmitter_3D;
struct Rotation_ExplosionEmitter_3D;
struct MultipleExplosionEmitter_3D;
struct Multiple_Rotation_ExplosionEmitter_3D;

struct Interactive_DoorParticle_3D;

struct Unit_Turn_IndicatorEmitter_3D;
struct Unit_MovementTrailEmitter_3D;

struct CharacterSpawnEmitter_3D;
struct CharacterDamageEmitter_3D;
struct CharacterDeathEmitter_3D;

struct Weather_Rain_Emitter_3D;

struct EmitterSystem_3D
{
	static EmitterSystem_3D& GetInstance();

	void ClearEmitters();
	void Update(const float dt);
	std::vector<std::shared_ptr<Emitter_3D>> GetEmitter();

	// Test Model Emitter
	const std::weak_ptr<Emitter_3D> CreateTestModelShapeEmitter(glm::vec3 spawnPos);

	// Explosion 
	const std::weak_ptr<ExplosionEmitter_3D> CreateExplosionEmitter(glm::vec3 spawnPos);
	const std::weak_ptr<Rotation_ExplosionEmitter_3D> CreateRotationExplosionEmitter(glm::vec3 spawnPos);
	const std::weak_ptr<MultipleExplosionEmitter_3D> CreateMultipleExplosionEmitter(glm::vec3 spawnPos, glm::vec3 minSpawnPos, glm::vec3 maxSpawnPos, int explosionEmitterAmount);
	const std::weak_ptr<Multiple_Rotation_ExplosionEmitter_3D> CreateMultipleRotationExplosionEmitter(glm::vec3 spawnPos, glm::vec3 minSpawnPos, glm::vec3 maxSpawnPos, int explosionEmitterAmount);

	// Other Emitters
	const std::weak_ptr<Interactive_DoorParticle_3D> CreateInteractiveParticle(glm::vec3 spawnPos, glm::vec3 minSpawnPos, glm::vec3 maxSpawnPos);
	const std::weak_ptr<Unit_Turn_IndicatorEmitter_3D> CreateTileWaypointEmitter(glm::vec3 spawnPos);
	const std::weak_ptr<Unit_MovementTrailEmitter_3D> CreateUnitTrailEmitter(glm::vec3 spawnPos);

	void CreateChracterSpawnEmitter(glm::vec3 spawnPos = { 0.f, 0.f, 0.f }, std::weak_ptr<CharacterSpawnEmitter_3D> wk_ptr = std::weak_ptr<CharacterSpawnEmitter_3D>());
	const std::weak_ptr<CharacterDamageEmitter_3D> CreateChracterDamageEmitter(glm::vec3 spawnPos, glm::vec4 colourBegin, glm::vec4 colourEnd);
	const std::weak_ptr<CharacterDeathEmitter_3D> CreateChracterDeathEmitter(glm::vec3 spawnPos, glm::vec3 minSpawnPos, glm::vec3 maxSpawnPos, int explosionEmitterAmount, glm::vec4 colourBegin, glm::vec4 colourEnd);

	const std::weak_ptr<Weather_Rain_Emitter_3D> CreateWeatherRainEmitter(glm::vec3 spawnPos, glm::vec3 minSpawnPos, glm::vec3 maxSpawnPos);

private:
	EmitterSystem_3D();

	std::vector<std::shared_ptr<Emitter_3D>> m_Emitters;
	// Emitter 3D Management
	std::set<short> m_UniqueEmitterSlots;
	std::queue<short> m_AvailableEmitterSlots;

	// Helper Functions
	void AddEmitter(const std::shared_ptr<Emitter_3D> emitter);
};

#endif