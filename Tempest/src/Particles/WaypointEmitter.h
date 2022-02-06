

#ifndef WAYPOINT_EMITTER_H
#define WAYPOINT_EMITTER_H

#include "Emitter.h"

struct WaypointEmitter final : public Emitter
{
	void SelfUpdate() override;


	// Travel Progression
	float m_initialDistanceSquared;

	// Velocity controls
	glm::vec2 m_startVelocity, m_endVelocity;
};

#endif // !WAYPOINT_EMITTER_H
