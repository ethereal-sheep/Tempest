

#ifndef ELONGATED_EXPLOSION_EMITTER_3D_H
#define ELONGATED_EXPLOSION_EMITTER_3D_H

#include "ExplosionEmitter_3D.h"

struct Rotation_ExplosionEmitter_3D : public ExplosionEmitter_3D
{
	void ParticleSetUp(Particle_3D& particle) override;
};

#endif