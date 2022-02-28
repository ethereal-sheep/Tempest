

#ifndef MULTIPLE_EXPLOSION_EMITTER_3D_H
#define MULTIPLE_EXPLOSION_EMITTER_3D_H

#include "Emitter_3D.h"

// Special Class Handler
struct MultipleExplosionEmitter_3D : public Emitter_3D
{
	MultipleExplosionEmitter_3D(int explosionEmitterAmount = 0);

	// Additional amount of explosion emitter to create
	int m_explosionEmitterAmount;

	void Emit(const int particleAmount) override;
};



#endif // !MULTIPLE_EXPLOSION_EMITTER_3D_H

