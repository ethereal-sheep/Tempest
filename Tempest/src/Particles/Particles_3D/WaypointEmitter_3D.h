

#ifndef WAYPOINT_EMITTER_3D_H
#define WAYPOINT_EMITTER_3D_H

#include "Emitter_3D.h"

struct WaypointEmitter_3D final : public Emitter_3D
{
	WaypointEmitter_3D();
	void SelfUpdate() override;

	std::vector<glm::vec3> m_wayPoints;
	short m_wayPointIndex;
	bool m_recalculateVelocity;
};

#endif 
