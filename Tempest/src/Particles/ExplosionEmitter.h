


#ifndef EXPLOSION_EMITTER_H
#define EXPLOSION_EMITTER_H

#include "Emitter.h"

#include <gtx/transform.hpp>


struct ExplosionEmitter final : public Emitter
{
	void Emit(const int particleAmount) override;
};

#endif // ! EXPLOSION_EMITTER_H

