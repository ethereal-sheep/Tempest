

#ifndef MULTIPLE_EXPLOSION_EMITTER_3D_H
#define MULTIPLE_EXPLOSION_EMITTER_3D_H

#include "Emitter_3D.h"

// Special Class Handler
struct MultipleExplosionEmitter_3D : public Emitter_3D
{
	MultipleExplosionEmitter_3D(int explosionEmitterAmount = 0);

	// Additional amount of explosion emitter to create
	int m_explosionEmitterAmount;

	// Min and Max range of the spawn pos - Assume values given are correct to work
	glm::vec3 m_minPos;
	glm::vec3 m_maxPos;

	virtual void Emit(const int particleAmount) override;
	virtual void ParticleSetUp(Particle_3D& particle) override;
	virtual void OnDeath() override;
};


#endif // !MULTIPLE_EXPLOSION_EMITTER_3D_H

