


#ifndef PARTICLE_SYSTEM_3D_H
#define PARTICLE_SYSTEM_3D_H

#include "Core.h"
#include "Emitter_3D.h"

#include <glm.hpp>

// Forward Declarations
struct ExplosionEmitter_3D;
struct MultipleExplosionEmitter_3D;
struct InteractiveParticle_3D;

struct ParticleSystem_3D
{
	static ParticleSystem_3D& GetInstance();

	void Update();
	std::vector<std::shared_ptr<Emitter_3D>> GetEmitter();

	const std::weak_ptr<Emitter_3D> CreateTestModelShapeEmitter(glm::vec3 spawnPos);
	const std::weak_ptr<ExplosionEmitter_3D> CreateBigExplosionEmitter(glm::vec3 spawnPos);
	const std::weak_ptr<ExplosionEmitter_3D> CreateExplosionEmitter(glm::vec3 spawnPos);
	const std::weak_ptr<MultipleExplosionEmitter_3D> CreateMultipleExplosionEmitter(glm::vec3 spawnPos, glm::vec3 minSpawnPos, glm::vec3 maxSpawnPos, int explosionEmitterAmount);
	const std::weak_ptr<InteractiveParticle_3D> CreateInteractiveParticle(glm::vec3 spawnPos, glm::vec3 minSpawnPos, glm::vec3 maxSpawnPos);

private:
	ParticleSystem_3D();


	std::vector<std::shared_ptr<Emitter_3D>> m_Emitters;
	// Emitter 3D Management
	std::set<short> m_UniqueEmitterSlots;
	std::queue<short> m_AvailableEmitterSlots;

	// Helper Functions
	void AddEmitter(const std::shared_ptr<Emitter_3D> emitter);
};

#endif