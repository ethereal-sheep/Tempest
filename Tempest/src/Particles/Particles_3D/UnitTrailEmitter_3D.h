

#ifndef UNIT_TRAIL_EMITTER_3D_H
#define UNIT_TRAIL_EMITTER_3D_H

#include "Emitter_3D.h"

struct UnitTrailEmitter_3D : public Emitter_3D
{
	UnitTrailEmitter_3D();

	void SelfUpdate(const float dt) override;
	void Emit(const int particleAmount) override;
	void ParticleSetUp(Particle_3D& particle) override;

	// Stop emitting, when waypoint is reached
	bool m_waypointsCompleted;

	std::vector<glm::vec3> m_wayPoints;
	short m_wayPointIndex;
	bool m_recalculateVelocity;

private:
	void Reset();
};

#endif
