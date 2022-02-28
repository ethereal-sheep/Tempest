

// Main Header
#include "ParticleSystem_3D.h"

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


const std::weak_ptr<Emitter_3D> ParticleSystem_3D::CreateTestEmitter(glm::vec3 spawnPos)
{
	auto tempEmitter = std::make_shared<Emitter_3D>();
	Emitter_3D& explosionEmitter = *tempEmitter.get();
	AddEmitter(tempEmitter);

	// Emitter_2D values - Without consideration for default ctor values
	explosionEmitter.m_GM.m_position = spawnPos;
	//explosionEmitter.m_GM.m_velocity.x = -500.0f;
	explosionEmitter.m_MM.m_duration = 50.f;
	explosionEmitter.m_GM.m_active = true;
	explosionEmitter.m_MM.m_preWarm = true;

	explosionEmitter.m_EM.m_spawnTimeInterval = 2.f; // 5x slower of dt
	explosionEmitter.m_EM.m_spawnCountTimer = explosionEmitter.m_EM.m_spawnTimeInterval;
	explosionEmitter.m_EM.m_rateOverTime = 20;
	explosionEmitter.m_MM.m_maxParticles = 1000;

	// Particle Architype values - without consideration for default ctor
	explosionEmitter.m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	explosionEmitter.m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	explosionEmitter.m_PAM.m_velocityVariation = glm::vec3{ 3.0f, 1.0f, 3.0f};

	explosionEmitter.m_PAM.m_sizeBegin = 10.0f;
	explosionEmitter.m_PAM.m_sizeEnd = 0.0f;
	explosionEmitter.m_PAM.m_sizeVariation = 0.3f;

	explosionEmitter.m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	explosionEmitter.m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

	explosionEmitter.m_PAM.m_lifeTime = 1.0f;

	return tempEmitter;
}