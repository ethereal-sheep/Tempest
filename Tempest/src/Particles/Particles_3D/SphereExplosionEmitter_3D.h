



#ifndef SPHERE_EXPLOSION_EMITTER_H
#define SPHERE_EXPLOSION_EMITTER_H

#include "ExplosionEmitter_3D.h"

struct SphereExplosionEmitter_3D final : public ExplosionEmitter_3D
{
	void ParticleSetUp(Particle_3D& particle) override;
};

#endif // !SPHERE_EXPLOSION_EMITTER_H
