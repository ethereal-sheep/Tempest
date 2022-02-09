



#ifndef EMITTER_H
#define EMITTER_H

#include <glm.hpp>
#include <vector>
#include <queue>

#include "EmissionModules.h"

// Note, glm::vectors are used to utilise the mix function
struct Emitter
{
	// Particles Management
	std::vector<Particle> m_particles;
	std::queue<short> m_available_ParticleSlots;

	MainModule m_MM;
	GameObjectModule m_GM;
	ParticleArchetypeModule m_PAM;
	EmissionModule m_EM;
	RendererModule m_RM;

	std::vector<glm::vec2> m_wayPoints;
	short m_wayPointIndex;
	bool m_recalculateVelocity;

	Emitter();
	void Update();
	void ClearAllParticles();

protected:
	virtual void SelfUpdate();
	virtual void Emit(const int particleAmount);
};


#endif // !EMITTER_H
