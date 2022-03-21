

#ifndef TILE_WAYPOINT_EMITTER_3D_H
#define TILE_WAYPOINT_EMITTER_3D_H

#include "Emitter_3D.h"

struct Unit_Turn_IndicatorEmitter_3D final : public Emitter_3D
{
	Unit_Turn_IndicatorEmitter_3D();

	void Update(const float dt) override;

	std::vector<glm::vec3> m_wayPoints;
	short m_wayPointIndex;
	bool m_recalculateVelocity;

	// Own Functions
	void UpdateWaypoints(glm::vec3 tileOriginPosition);
};

#endif 
