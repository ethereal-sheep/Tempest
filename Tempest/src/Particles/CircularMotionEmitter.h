

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