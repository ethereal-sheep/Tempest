


#ifndef PARTICLE_SYSTEM_3D_H
#define PARTICLE_SYSTEM_3D_H

#include "Core.h"
#include "Emitter_3D.h"

struct ParticleSystem_3D
{
	static ParticleSystem_3D& GetInstance();

	void Update();
	auto GetEmitter() const;

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