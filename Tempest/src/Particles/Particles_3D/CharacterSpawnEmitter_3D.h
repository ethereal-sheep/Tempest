

#ifndef CHARACTER_SPAWN_EMITTER_3D_H
#define CHARACTER_SPAWN_EMITTER_3D_H

#include "Emitter_3D.h"
#include <array>

struct CharacterSpawnEmitter_3D : public Emitter_3D
{
#define waypoint_Index 4

	CharacterSpawnEmitter_3D();
	void AssignWaypoint(const std::array<glm::vec3, waypoint_Index> newWaypoints);

	void Emit(const int particleAmount) override;

private:
	std::array<glm::vec3, waypoint_Index> m_wayPoints;
};

#endif // !CHARACTER_SPAWN_EMITTER_3D_H

