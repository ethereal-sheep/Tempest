/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef CIRCULAR_MOTION_EMITTER_H
#define CIRCULAR_MOTION_EMITTER_H

#include "Emitter.h"

struct CircularMotionEmitter final : public Emitter
{
	CircularMotionEmitter();

	void SelfUpdate() override;
	void Emit(const int particleAmount) override;

	glm::vec2 m_centrePoint;
	float m_radius; // The radius to rotate around
	float m_angle; // The angle it is rotating at
};

#endif