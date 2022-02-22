/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef TRAIL_WAYPOINT_EMITTER_3D_H
#define TRAIL_WAYPOINT_EMITTER_3D_H

#include "Emitter.h"

struct Trail_WaypointEmitter_3D final : public Emitter
{
	Trail_WaypointEmitter_3D();
	void SelfUpdate() override;

	std::vector<glm::vec3> m_wayPoints;
	short m_wayPointIndex;
	bool m_recalculateVelocity;

	// Travel Progression
	//float m_initialDistanceSquared;

	// Velocity controls
	// glm::vec2 m_startVelocity, m_endVelocity;
};

#endif // !WAYPOINT_EMITTER_H
