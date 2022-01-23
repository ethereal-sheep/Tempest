
#include "ParticleSystem_2D.h"

void ParticleSystem_2D::Update(const float dt)
{
	// Update all the emitters here
	for (auto& emitter : m_emitters)
		if (emitter->m_active)
		{
			emitter->Update(dt);
		}
}