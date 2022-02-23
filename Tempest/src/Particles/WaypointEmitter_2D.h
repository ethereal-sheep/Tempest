/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef WAYPOINT_EMITTER_H
#define WAYPOINT_EMITTER_H

#include "Emitter_2D.h"

struct WaypointEmitter_2D final : public Emitter_2D
{
	WaypointEmitter_2D();
	void SelfUpdate() override;

	std::vector<glm::vec2> m_wayPoints;
	short m_wayPointIndex;
	bool m_recalculateVelocity;


	// Travel Progression
	// float m_initialDistanceSquared;

	// Velocity controls
	// glm::vec2 m_startVelocity, m_endVelocity;
};

#endif // !WAYPOINT_EMITTER_H
