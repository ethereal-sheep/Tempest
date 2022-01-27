

#ifndef WAYPOINT_EMITTER_H
#define WAYPOINT_EMITTER_H

#include "Emitter.h"

struct WaypointEmitter final : public Emitter
{
	void SelfUpdate() override;
};

#endif // !WAYPOINT_EMITTER_H
