/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

// Main Include
#include "Emitter_2D.h"

// Additional Includes
#include "../Random.h"
#include "Util.h"
#include <numbers>

#define GRAVITY -9.8f

Emitter_2D::Emitter_2D()
{
	m_particles.resize(m_MM.m_maxParticles);

	// Store all available slots
	for (short i = 0; i < m_MM.m_maxParticles; ++i)
		m_available_ParticleSlots.push(i);

	if (m_MM.m_preWarm)
		Emit(m_EM.m_rateOverTime);
}

void Emitter_2D::Update()
{
	if (m_MM.m_preWarm)
	{
		Emit(m_EM.m_rateOverTime);
		m_MM.m_preWarm = false;
	}

	// Update Emittor position
	m_GM.m_position += m_GM.m_velocity * m_MM.m_simulationSpeed;

	// Emitter_2D emission
	if (m_EM.m_spawnCountTimer <= 0.0f)
	{
		while (m_EM.m_spawnCountTimer <= 0.f)
		{
			// Emit particle
			Emit(m_EM.m_rateOverTime);
			
			// Ensure if simulation speed is different from the spawnTimeInterval, still spawn right amount
			m_EM.m_spawnCountTimer += m_EM.m_spawnTimeInterval;
		}

		// Reset time interval
		m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	}
	else
		m_EM.m_spawnCountTimer -= m_MM.m_simulationSpeed;

	// Emitter_2D Lifetime update
	if (m_MM.m_duration <= 0.f)
		m_GM.m_active = false;
	else
		m_MM.m_duration -= m_MM.m_simulationSpeed;

	ParticleUpdate();
}

void Emitter_2D::ParticleUpdate()
{
	// Particle_2D Behaviour
	for (short i = 0; i < m_particles.size(); ++i)
	{
		auto& particle = m_particles[i];

		if (!particle.m_isActive)
			continue;
		else if (particle.m_lifeRemaining <= 0)
		{
			particle.m_isActive = false;

			// To be reused
			m_available_ParticleSlots.push(i);
		}
		else
		{
			particle.m_position += particle.m_velocity * m_MM.m_simulationSpeed;
			//particle.m_rotation += 0.01f * m_MM.m_simulationSpeed;

			// Calculate the lifeTime remaining
			float lifePercent = particle.m_lifeRemaining / particle.m_lifeTime;

			// Update Velocity
			if (particle.m_gravity)
				particle.m_velocity.y += GRAVITY * m_MM.m_simulationSpeed;
			else
			{
				// Velocity
				particle.m_velocity.x = glm::mix(particle.m_velocityEnd.x, particle.m_velocityBegin.x, lifePercent);
				particle.m_velocity.y = glm::mix(particle.m_velocityEnd.y, particle.m_velocityBegin.y, lifePercent);
			}

			particle.m_size = glm::mix(particle.m_scaleEnd, particle.m_scaleBegin, lifePercent);
			particle.m_colour = glm::mix(particle.m_colourEnd, particle.m_colourBegin, lifePercent);
			particle.m_lifeRemaining -= m_MM.m_simulationSpeed;
		}
	}
}

void Emitter_2D::ParticleSetUp(Particle_2D& particle)
{
	//Default set values
	particle.m_position = m_GM.m_position;
	particle.m_isActive = true;

	// Velocity from EmissionModule
	particle.m_velocityBegin = m_PAM.m_velocityStart;
	particle.m_velocityEnd = m_PAM.m_velocityEnd;
	particle.m_gravity = m_PAM.m_gravity;

	// Velocity Variation for the Individual Particle
	glm::vec2 rand_Vel;
	rand_Vel.x = Random::Float() * m_PAM.m_velocityVariation.x;
	rand_Vel.y = Random::Float() * m_PAM.m_velocityVariation.y;

	particle.m_velocityBegin += rand_Vel;
	particle.m_velocityEnd += rand_Vel;

	// Color
	particle.m_colourBegin = m_PAM.m_colourBegin;
	particle.m_colourEnd = m_PAM.m_colourEnd;

	// Lifetime
	particle.m_lifeTime = m_PAM.m_lifeTime;
	particle.m_lifeRemaining = m_PAM.m_lifeTime;

	// Scale Variation
	float scaleVariation = Random::Float() * m_PAM.m_scaleVariation;

	particle.m_scaleBegin = m_PAM.m_scaleBegin + scaleVariation;
	particle.m_scaleEnd = m_PAM.m_scaleEnd + scaleVariation;

	particle.m_type = m_RM.m_type;
}

void Emitter_2D::Emit(const int particleAmount)
{
	// Emit only if enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initailisation of the particle
			Particle_2D particle;
			ParticleSetUp(particle);

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}

void Emitter_2D::ClearAllParticles()
{
	// Particle_2D Behaviour
	for (short i = 0; i < m_particles.size(); ++i)
	{
		auto& particle = m_particles[i];

		if (particle.m_isActive)
		{
			particle.m_lifeRemaining = 0;
			particle.m_isActive = false;

			// To be reused
			m_available_ParticleSlots.push(i);
		}
	}
}