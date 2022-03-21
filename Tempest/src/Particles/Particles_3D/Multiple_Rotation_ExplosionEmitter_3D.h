

#ifndef MULTIPLE_ROTATION_EXPLOSION_EMITTER_3D_H
#define MULTIPLE_ROTATION_EXPLOSION_EMITTER_3D_H

#include "MultipleExplosionEmitter_3D.h"

struct Multiple_Rotation_ExplosionEmitter_3D : MultipleExplosionEmitter_3D
{
	Multiple_Rotation_ExplosionEmitter_3D();
	void ParticleSetUp(Particle_3D& particle) override;
	void OnDeath() override;
};

#endif //!MULTIPLE_ROTATION_EXPLOSION_EMITTER_3D_H
