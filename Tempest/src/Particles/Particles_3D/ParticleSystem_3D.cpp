

// Main Header
#include "ParticleSystem_3D.h"

#include "ExplosionEmitter_3D.h"
#include "MultipleExplosionEmitter_3D.h"
#include "InteractiveParticle_3D.h"

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
	for (short i = 0; i < m_Emitters.size(); ++i)
	{
		// Check for null pointer - shouldn't happen
		if (m_Emitters[i])
		{
			Emitter_3D& emitter = *m_Emitters[i];

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

std::vector<std::shared_ptr<Emitter_3D>> ParticleSystem_3D::GetEmitter()
{
	return m_Emitters;
}

void ParticleSystem_3D::AddEmitter(const std::shared_ptr<Emitter_3D> emitter)
{
	// There is available slot
	if (m_AvailableEmitterSlots.size())
	{
		// The index of the free slot
		short freeSlot = m_AvailableEmitterSlots.front();

		// Update the emitter memory management
		m_AvailableEmitterSlots.pop();
		m_UniqueEmitterSlots.erase(freeSlot);

		// Allocate the emitter, destructor is handled s_ptr
		m_Emitters[freeSlot] = emitter;
	}
	else
		m_Emitters.push_back(emitter);
}

const std::weak_ptr<ExplosionEmitter_3D> ParticleSystem_3D::CreateTestEmitter(glm::vec3 spawnPos)
{
	auto tempEmitter = std::make_shared<ExplosionEmitter_3D>();
	ExplosionEmitter_3D& explosionEmitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Emitter_3D values - Without consideration for default ctor values
	tempEmitter->m_GM.m_position = spawnPos;
	//tempEmitter->m_GM.m_velocity.x = -500.0f;
	tempEmitter->m_MM.m_duration = 0.6f;
	tempEmitter->m_GM.m_active = true;
	tempEmitter->m_MM.m_preWarm = true;

	tempEmitter->m_EM.m_spawnTimeInterval = 0.3f; // 5x slower of dt
	tempEmitter->m_EM.m_spawnCountTimer = tempEmitter->m_EM.m_spawnTimeInterval;
	tempEmitter->m_EM.m_rateOverTime = 20;
	tempEmitter->m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	tempEmitter->m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	tempEmitter->m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	tempEmitter->m_PAM.m_velocityVariation = glm::vec3{ 3.0f, 1.0f, 3.0f};

	tempEmitter->m_PAM.m_scaleBegin = glm::vec3{ 0.5f, 0.5f, 0.5f };
	tempEmitter->m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	tempEmitter->m_PAM.m_scaleVariation = glm::vec3{ 0.3f, 0.3f, 0.3f };

	tempEmitter->m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	tempEmitter->m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	tempEmitter->m_PAM.m_lifeTime = 0.3f;

	return tempEmitter;
}

const std::weak_ptr<MultipleExplosionEmitter_3D> ParticleSystem_3D::CreateMultipleExplosionEmitter(glm::vec3 spawnPos, int explosionEmitterAmount)
{
	auto tempEmitter = std::make_shared<MultipleExplosionEmitter_3D>();

	// MultipleExplosionEmitter - Own Explosion
	// Emitter_3D values - Without consideration for default ctor values
	tempEmitter->m_GM.m_position = spawnPos;
	//tempEmitter->m_GM.m_velocity.x = -500.0f;
	tempEmitter->m_MM.m_duration = 0.6f;
	tempEmitter->m_GM.m_active = true;
	tempEmitter->m_MM.m_preWarm = true;

	tempEmitter->m_EM.m_spawnTimeInterval = 0.3f; // 5x slower of dt
	tempEmitter->m_EM.m_spawnCountTimer = tempEmitter->m_EM.m_spawnTimeInterval;
	tempEmitter->m_EM.m_rateOverTime = 20;
	tempEmitter->m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	tempEmitter->m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	tempEmitter->m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	tempEmitter->m_PAM.m_velocityVariation = glm::vec3{ 3.0f, 1.0f, 3.0f };

	tempEmitter->m_PAM.m_scaleBegin = glm::vec3{ 0.5f, 0.5f, 0.5f };
	tempEmitter->m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	tempEmitter->m_PAM.m_scaleVariation = glm::vec3{ 0.3f, 0.3f, 0.3f };

	tempEmitter->m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	tempEmitter->m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	tempEmitter->m_PAM.m_lifeTime = 0.3f;

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
	interactiveEmitter.m_MM.m_duration = 1000000.f;
	interactiveEmitter.m_GM.m_active = true;
	interactiveEmitter.m_MM.m_preWarm = true;

	interactiveEmitter.m_EM.m_spawnTimeInterval = 0.05f;; // 5x slower of dt
	interactiveEmitter.m_EM.m_spawnCountTimer = interactiveEmitter.m_EM.m_spawnTimeInterval;
	interactiveEmitter.m_EM.m_rateOverTime = 1;
	interactiveEmitter.m_MM.m_maxParticles = 30;

	// Particle Architype values - without consideration for default ctor
	interactiveEmitter.m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	interactiveEmitter.m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	interactiveEmitter.m_PAM.m_velocityVariation = glm::vec3{ 3.0f, 1.0f, 3.0f };

	interactiveEmitter.m_PAM.m_scaleBegin = glm::vec3{ 0.2f, 0.2f, 0.2f };
	interactiveEmitter.m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	interactiveEmitter.m_PAM.m_scaleVariation = glm::vec3{ 1.0f, 1.0f, 1.0f };

	//interactiveEmitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	//interactiveEmitter.m_PAM.m_colourEnd = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };
	
	// Note - Values to be divided by 255.0f - Forgot the reason
	interactiveEmitter.m_PAM.m_colourBegin = glm::vec4{ 0.996f, 0.831f, 0.482f, 1.0f };
	interactiveEmitter.m_PAM.m_colourEnd = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };

	interactiveEmitter.m_PAM.m_lifeTime = 0.3f;

	return tempEmitter;
}