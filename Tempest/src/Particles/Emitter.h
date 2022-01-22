



#ifndef EMITTER_H
#define EMITTER_H

#include <glm.hpp>
#include <vector>
#include <queue>

#include "Particles.h"

// Note, glm::vectors are used to utilise the mix function
struct Emitter
{
	glm::vec2 m_position;

	// Particles Management
	std::vector<Particle> m_particles;
	std::queue<short> m_available_ParticleSlots;

	Emitter();
	void Update(const float dt);

	// Test Function
	void Emit(const int particleAmount);

	glm::vec2 m_startVelocity, m_endVelocity;
	glm::vec2 m_velocityVariation;

	float m_sizeBegin, m_sizeEnd;
	float m_sizeVariation;

	glm::vec4 m_colourBegin, m_colourEnd;
	glm::vec4 m_colourVariation;

	float m_lifeTime;
	//bool m_loop						
	int m_maxParticles;
	int m_particleIndex;
	short m_rateOvertime; // How much particles to spawn per unit of time

	ParticleType m_type;

	//bool  m_clockwise;
	//float m_xOffset, m_yOffset;
};


#endif // !EMITTER_H
