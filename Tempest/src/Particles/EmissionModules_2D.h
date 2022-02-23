/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef EMISSION_MODULES_2D_H
#define EMISSION_MODULES_2D_H

#include "Particle_2D.h"

#include <glm.hpp> // glm::vec2, glm::vec4
//#include <queue>
//#include <vector>

struct MainModule_2D
{
	MainModule_2D();

	float m_duration;		// Duration of emitter
	bool  m_looping;		// Resets on the emitter death
	short m_maxParticles;	// Max amount of particles 

	float m_simulationSpeed;	// What DT to run in
	bool m_preWarm;
};

struct GameObjectModule_2D
{
	GameObjectModule_2D();

	glm::vec2 m_position;
	glm::vec2 m_velocity;

	bool m_active;
};

struct ParticleArchetypeModule_2D
{
	ParticleArchetypeModule_2D();

	glm::vec2 m_startVelocity, m_endVelocity;
	glm::vec2 m_velocityVariation;

	float m_sizeBegin, m_sizeEnd;
	float m_sizeVariation;

	glm::vec4 m_colourBegin, m_colourEnd;
	//glm::vec4 m_colourVariation

	float m_lifeTime;		// Particle life duration
};

struct EmissionModule_2D
{
	EmissionModule_2D();

	float m_burstTime;		// What time to spawn particle
	float m_burstInterval;	// Interval before the next burst cycle
	short m_burstCycle;		// How many times to burst
	short m_burstCount;		// How many particle to spawn each burst

	short m_rateOverTime;	// Particle to spawn per unit time
	
	// Hidden - For internal use
	float m_spawnTimeInterval;	
	float m_spawnCountTimer;	// Time the interval
};

struct RendererModule_2D
{
	RendererModule_2D();
	ParticleType m_type;
};

#endif
