
#ifndef CHARACTER_CHARGED_ATTACK_TILE_EMITTER_3D_H
#define CHARACTER_CHARGED_ATTACK_TILE_EMITTER_3D_H

#include <array>
#include "Emitter_3D.h"

struct CharacterTileCharged_Emitter_3D : public Emitter_3D
{

#define waypoint_Index 4

	CharacterTileCharged_Emitter_3D();
	void AssignWaypoint(const std::array<glm::vec3, waypoint_Index> newWaypoints);

	void Emit(const int particleAmount) override;
	
private:
	std::array<glm::vec3, waypoint_Index> m_wayPoints;

	// Helper function
	glm::vec3 RandomPoint();
};

#endif // !CHARACTER_CHARGED_ATTACK_TILE_EMITTER_3D_H
