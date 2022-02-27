

#ifndef EXPLOSION_EMITTER_H
#define EXPLOSION_EMITTER_H

#include "Emitter_3D.h"

#include <gtx/transform.hpp>

struct ExplosionEmitter_3D : public Emitter_3D
{
	void Emit(const int particleAmount) override;
};


#endif // !EXPLOSION_EMITTER_H
