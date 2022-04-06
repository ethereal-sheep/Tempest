

#ifndef SMOKE_POOF_EMITTER_3D
#define SMOKE_POOF_EMITTER_3D

#include "ExplosionEmitter_3D.h"

struct Smoke_Poof_Emitter_3D : public ExplosionEmitter_3D
{
	Smoke_Poof_Emitter_3D();
	void Emit(const int particleAmount) override;
	void ParticleUpdate(const float dt) override;
};

#endif // !SMOKE_POOF_EMITTER_3D

