/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

/*
* References:
* http://www.cplusplus.com/forum/general/129187/
* https://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
*/

#include "CircularMotionEmitter.h"

#include "Logger/Log.h"

CircularMotionEmitter::CircularMotionEmitter()
	: Emitter()
	, m_centrePoint { glm::vec2(0.0f, 0.0f)}
	, m_radius { 0.0f }
	, m_angle {0.0f }
{}

void CircularMotionEmitter::SelfUpdate()
{
	if (m_MM.m_preWarm)
	{
		Emit(m_EM.m_rateOverTime);
		m_MM.m_preWarm = false;
	}

	m_angle += 0.1f;  // or some other value.  Higher numbers = circles faster

	float x = cos(m_angle) * m_radius;
	float y = sin(m_angle) * m_radius;

	x += m_centrePoint.x;
	y += m_centrePoint.y;

	// Update Emittor position
	m_GM.m_position.x = x;
	m_GM.m_position.y = y;

	// Emitter emittion
	if (m_EM.m_spawnCountTimer <= 0.f)
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

	// Emitter Lifetime update
	if (m_MM.m_duration <= 0.f)
		m_GM.m_active = false;
	else
		m_MM.m_duration -= m_MM.m_simulationSpeed;
}


void CircularMotionEmitter::Emit(const int particleAmount)
{
	// Emit only if enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initailisation of the particle
			Particle particle;

			// Position
			particle.m_position = m_GM.m_position;
			particle.m_isActive = true;

			// Rotation
			//particle.m_rotation = Random::Float() * 2.0f * std::numbers::pi;
			particle.m_rotation = m_angle;

			// Velocity
			particle.m_velocity = m_PAM.m_startVelocity;
			particle.m_velocity.x += m_PAM.m_velocityVariation.x; //* (Random::Float() - 0.5f);
			particle.m_velocity.y += m_PAM.m_velocityVariation.y; //* (Random::Float() - 0.5f);

			// Color
			particle.m_colour.r = m_PAM.m_colourBegin.r; //(Random::Float() - 0.5f);
			particle.m_colour.g = m_PAM.m_colourBegin.g; //(Random::Float() - 0.5f);
			particle.m_colour.b = m_PAM.m_colourBegin.b; //(Random::Float() - 0.5f);

			// Rendering Type
			particle.m_type = m_RM.m_type;

			// Lifetime
			particle.m_lifeTime = m_PAM.m_lifeTime;
			particle.m_lifeRemaining = m_PAM.m_lifeTime;
			particle.m_size = m_PAM.m_sizeBegin + m_PAM.m_sizeVariation;// *(Random::Float() - 0.5f);

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}