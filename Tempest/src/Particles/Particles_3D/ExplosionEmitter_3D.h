

#ifndef EXPLOSION_EMITTER_3D_H
#define EXPLOSION_EMITTER_3D_H

#include "Emitter_3D.h"

struct ExplosionEmitter_3D : public Emitter_3D
{
	ExplosionEmitter_3D();
	void Emit(const int particleAmount) override;
	void ParticleSetUp(Particle_3D& particle) override;
};

#endif // !EXPLOSION_EMITTER_3D_H
