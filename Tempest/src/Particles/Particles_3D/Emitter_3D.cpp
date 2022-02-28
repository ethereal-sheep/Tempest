/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Emitter_3D.h"
#include "Graphics/Basics/RenderSystem.h"

Emitter_3D::Emitter_3D()
{
	m_particles.resize(m_MM.m_maxParticles);

	// Store all available slots
	for (short i = 0; i < m_MM.m_maxParticles; ++i)
		m_available_ParticleSlots.push(i);

	if (m_MM.m_preWarm)
		Emit(m_EM.m_rateOverTime);
}

void Emitter_3D::SelfUpdate()
{
	if (m_MM.m_preWarm)
	{
		Emit(m_EM.m_rateOverTime);
		m_MM.m_preWarm = false;
	}

	// Update Emitter Position
	m_GM.m_position += m_GM.m_velocity * m_MM.m_simulationSpeed;

	// Emitter_3D emission
	if (m_EM.m_spawnCountTimer <= 0.0f)
	{
		while (m_EM.m_spawnCountTimer <= 0.0f)
		{
			// Emit Particle
			Emit(m_EM.m_rateOverTime);

			// Ensure if simulation speed is different from the spawnTimeInterval, still spawn right amount
			m_EM.m_spawnCountTimer += m_EM.m_spawnTimeInterval;
		}

		// Reset time interval
		m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	}
	else
		m_EM.m_spawnCountTimer -= m_MM.m_simulationSpeed;

	// Emitter_3D Lifetime update
	if (m_MM.m_duration <= 0.f)
		m_GM.m_active = false;
	else
		m_MM.m_duration -= m_MM.m_simulationSpeed;
}

void Emitter_3D::Update()
{
	SelfUpdate();

	// Particle_3D Behaviour
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

			// lifeTime and percentage of lifeTime remaining
			float lifePercent = particle.m_lifeRemaining / particle.m_lifeTime;
			particle.m_lifeRemaining -= m_MM.m_simulationSpeed;

			// Scale
			particle.m_scale.x = glm::mix(m_PAM.m_scaleEnd.x, m_PAM.m_scaleBegin.x, lifePercent);
			particle.m_scale.y = glm::mix(m_PAM.m_scaleEnd.y, m_PAM.m_scaleBegin.y, lifePercent);
			particle.m_scale.z = glm::mix(m_PAM.m_scaleEnd.z, m_PAM.m_scaleBegin.z, lifePercent);

			// Colour
			particle.m_colour = glm::mix(m_PAM.m_colourEnd, m_PAM.m_colourBegin, lifePercent);
			
			
			// Change the string in this function
			Tempest::Service<Tempest::RenderSystem>::Get().SubmitModel(particle);
		}
	}
}

void Emitter_3D::Emit(const int particleAmount)
{
	// Emit only if enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initailisation of the particle
			Particle_3D particle;

			particle.m_position = m_GM.m_position;
			particle.m_isActive = true;
			//particle.m_rotation = Random::Float() * 2.0f * std::numbers::pi;

			// Velocity
			particle.m_velocity = m_PAM.m_startVelocity;
			particle.m_velocity.x += m_PAM.m_velocityVariation.x; //* (Random::Float() - 0.5f);
			particle.m_velocity.y += m_PAM.m_velocityVariation.y; //* (Random::Float() - 0.5f);

			// Color
			particle.m_colour.r = m_PAM.m_colourBegin.r; //(Random::Float() - 0.5f);
			particle.m_colour.g = m_PAM.m_colourBegin.g; //(Random::Float() - 0.5f);
			particle.m_colour.b = m_PAM.m_colourBegin.b; //(Random::Float() - 0.5f);

			// Lifetime
			particle.m_lifeTime = m_PAM.m_lifeTime;
			particle.m_lifeRemaining = m_PAM.m_lifeTime;

			// Scale
			particle.m_scale.x = m_PAM.m_scaleBegin.x + m_PAM.m_scaleVariation.x;// *(Random::Float() - 0.5f);
			particle.m_scale.y = m_PAM.m_scaleBegin.y + m_PAM.m_scaleVariation.y;// *(Random::Float() - 0.5f);
			particle.m_scale.z = m_PAM.m_scaleBegin.z + m_PAM.m_scaleVariation.z;// *(Random::Float() - 0.5f);

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}

void Emitter_3D::ClearAllParticles()
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