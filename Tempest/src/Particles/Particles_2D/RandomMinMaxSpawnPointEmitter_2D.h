



#ifndef RANDOM_SPAWN_POINT_EMITTER_2D
#define RANDOM_SPAWN_POINT_EMITTER_2D

#include "EmitterSystem_2D.h"

struct RandomMinMaxSpawnPointEmitter_2D : public Emitter_2D
{
	RandomMinMaxSpawnPointEmitter_2D();

	// Random Point
	glm::vec2 m_Min, m_Max;

	glm::vec2 RandomPoint();

	void Emit(const int particleAmount);
};

#endif // !RANDOM_SPAWN_POINT_EMITTER_2D




