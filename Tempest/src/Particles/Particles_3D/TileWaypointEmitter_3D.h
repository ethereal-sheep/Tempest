

#ifndef TILE_WAYPOINT_EMITTER_3D_H
#define TILE_WAYPOINT_EMITTER_3D_H

#include "Emitter_3D.h"

struct TileWaypointEmitter_3D final : public Emitter_3D
{
	TileWaypointEmitter_3D();
	void SelfUpdate(const float dt) override;

	std::vector<glm::vec3> m_wayPoints;
	short m_wayPointIndex;
	bool m_recalculateVelocity;

	// Own Functions
	void UpdateWaypoints(glm::vec3 tileOriginPosition);
};

#endif 