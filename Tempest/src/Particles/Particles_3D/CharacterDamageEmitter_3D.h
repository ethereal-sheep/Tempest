

#ifndef CHARACTER_DAMAGE_EMITTER_3D_H
#define CHARACTER_DAMAGE_EMITTER_3D_H

#include "ExplosionEmitter_3D.h"

struct CharacterDamageEmitter_3D : public ExplosionEmitter_3D
{
	void Setup() override;
};
#endif
