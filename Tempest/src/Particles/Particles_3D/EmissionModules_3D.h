/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef EMISSION_MODULE_3D_H
#define EMISSION_MODULE_3D_H

#include <glm.hpp>
#include <string>

struct MainModule_3D
{
	MainModule_3D();

	float m_duration;		 // Duration of emitter
	bool  m_looping;		 // Resets on the emitter death
	short m_maxParticles;	 // Max amount of particles 

	float m_simulationSpeed; // What DT to run in
	bool m_preWarm;
};

struct GameObjectModule_3D
{
	GameObjectModule_3D();

	glm::vec3 m_position;
	glm::vec3 m_velocity;

	bool m_active;
};

struct ParticleArchetypeModule_3D
{
	ParticleArchetypeModule_3D();

	glm::vec3 m_startVelocity, m_endVelocity;
	glm::vec3 m_velocityVariation;

	glm::vec3 m_scaleBegin, m_scaleEnd;
	glm::vec3 m_scaleVariation;

	glm::vec4 m_colourBegin, m_colourEnd;
	//glm::vec4 m_colourVariation

	float m_lifeTime;		// Particle life duration
	bool m_rebirth;
	bool m_gravity;
};

struct EmissionModule_3D
{
	EmissionModule_3D();

	float m_burstTime;		// What time to spawn particle
	float m_burstInterval;	// Interval before the next burst cycle
	short m_burstCycle;		// How many times to burst
	short m_burstCount;		// How many particle to spawn each burst

	short m_rateOverTime;	// Particle to spawn per unit time

	// Hidden - For internal use
	float m_spawnTimeInterval;
	float m_spawnCountTimer;	// Time the interval

	float m_burstCountTimer;
	bool m_startBurstCycle;
};

struct RendererModule_3D
{
	RendererModule_3D();
 
	// Path Name for rendering modle
	std::string m_renderingPath;
};

#endif // !EMISSION_MODULE_3D_H
