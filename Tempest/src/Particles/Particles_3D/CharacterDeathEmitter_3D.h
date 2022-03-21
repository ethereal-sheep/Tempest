
#ifndef CHARACTER_DEATH_EMITTER_H
#define CHARACTER_DEATH_EMITTER_H

#include "Multiple_Rotation_ExplosionEmitter_3D.h"

struct CharacterDeathEmitter_3D : public Multiple_Rotation_ExplosionEmitter_3D
{
	CharacterDeathEmitter_3D();
	void OnDeath() override;
};

#endif // !CHARACTER_DEATH_EMITTER_H
