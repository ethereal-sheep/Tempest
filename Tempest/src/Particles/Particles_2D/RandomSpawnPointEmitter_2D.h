



#ifndef RANDOM_SPAWN_POINT_EMITTER_2D
#define RANDOM_SPAWN_POINT_EMITTER_2D

#include "EmitterSystem_2D.h"

struct RandomSpawnPointEmitter_2D : public Emitter_2D
{
	RandomSpawnPointEmitter_2D();

	// Random Point
	glm::vec2 m_Min, m_Max;

	glm::vec2 RandomPoint();
};

#endif // !RANDOM_SPAWN_POINT_EMITTER_2D




