

#ifndef CHARACTER_CUTSCENE_SPAWN_EMITTER__3D_H
#define CHARACTER_CUTSCENE_SPAWN_EMITTER__3D_H

#include "Emitter_3D.h"
#include <array>

struct CharacterCutsceneSpawnEmitter_3D : public Emitter_3D
{
#define waypoint_Index 4

	CharacterCutsceneSpawnEmitter_3D();
	void AssignWaypoint(const std::array<glm::vec3, waypoint_Index> newWaypoints);

	void Emit(const int particleAmount) override;

private:
	std::array<glm::vec3, waypoint_Index> m_wayPoints;
};

#endif // !CHARACTER_CUTSCENE_SPAWN_EMITTER_H

