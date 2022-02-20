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

#include "Emitter.h"

struct WaypointEmitter final : public Emitter
{
	WaypointEmitter();
	void SelfUpdate() override;


	// Travel Progression
	float m_initialDistanceSquared;

	// Velocity controls
	glm::vec2 m_startVelocity, m_endVelocity;
};

#endif // !WAYPOINT_EMITTER_H
