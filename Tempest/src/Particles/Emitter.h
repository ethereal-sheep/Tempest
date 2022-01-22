



#ifndef EMITTER_H
#define EMITTER_H

#include <glm.hpp>
#include <vector>
#include <queue>

#include "Particles.h"

struct ParticleArchetype
{
	ParticleArchetype();

	glm::vec2 m_startVelocity, m_endVelocity;
	glm::vec2 m_velocityVariation;

	float m_sizeBegin, m_sizeEnd;
	float m_sizeVariation;

	glm::vec4 m_colourBegin, m_colourEnd;
	//glm::vec4 m_colourVariation;

	float m_lifeTime;
	//float m_lifeVariation;

	ParticleType m_type;
};

// Note, glm::vectors are used to utilise the mix function
struct Emitter
{
	glm::vec2 m_position;

	// Particles Management
	std::vector<Particle> m_particles;
	std::queue<short> m_available_ParticleSlots;

	ParticleArchetype m_PA;

	float m_lifeTime;
	bool m_active;

	int m_maxParticles;

	bool m_preWarm;
	float m_countTimer;
	float m_spawnTimeInterval; 
	short m_spawnCount; // How much particles to spawn per unit of time
	//bool m_loop;
	
	//bool  m_clockwise;
	//float m_xOffset, m_yOffset;

	Emitter();
	void Update(const float dt);

private:
	void Emit(const int particleAmount);
};


#endif // !EMITTER_H
