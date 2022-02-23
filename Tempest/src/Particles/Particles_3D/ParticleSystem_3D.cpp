

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

auto ParticleSystem_3D::GetEmitter() const
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