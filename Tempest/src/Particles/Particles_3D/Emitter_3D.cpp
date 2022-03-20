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
#include "../Random.h"

#include "Logger/Log.h"

#define GRAVITY -9.8f


Emitter_3D::Emitter_3D()
{
	m_particles.resize(m_MM.m_maxParticles);

	// Store all available slots
	for (short i = 0; i < m_MM.m_maxParticles; ++i)
		m_available_ParticleSlots.push(i);

	if (m_MM.m_preWarm)
		Emit(m_EM.m_rateOverTime);
}

void Emitter_3D::Update(const float dt)
{
	if (m_MM.m_preWarm)
	{
		Emit(m_EM.m_rateOverTime);
		m_MM.m_preWarm = false;
	}

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
		//m_EM.m_spawnCountTimer -= m_MM.m_simulationSpeed;
		m_EM.m_spawnCountTimer -= dt;

	// Initial Burst Emission
	if (m_EM.m_burstTime <= 0.0f && m_EM.m_startBurstCycle == false)
	{
		// Emit Initial Burst Particle
		Emit(m_EM.m_burstCount);

		// Reduce cycle
		m_EM.m_startBurstCycle = true;
	}
	else
	{	
		// There is burst cycle to continue
		if (m_EM.m_burstCycle >= 1)
		{
			m_EM.m_burstCountTimer -= dt;

			if (m_EM.m_burstCountTimer <= 0.0f)
			{
				// Reset Timer
				m_EM.m_burstCountTimer = m_EM.m_burstInterval;

				// Emit Burst Particle
				Emit(m_EM.m_burstCount);

				// Reduce cycle
				--m_EM.m_burstCycle;
			}
		}
	}


	// Update Emitter Position
	//m_GM.m_position += m_GM.m_velocity * m_MM.m_simulationSpeed;
	m_GM.m_position += m_GM.m_velocity * dt;

	// Emitter_3D Lifetime update
	if (m_MM.m_duration <= 0.f)
	{
		OnDeath();
		m_GM.m_active = false;
	}
	else
		//m_MM.m_duration -= m_MM.m_simulationSpeed;
		m_MM.m_duration -= dt;

	ParticleUpdate(dt);
}

void Emitter_3D::ParticleUpdate(const float dt)
{
	// Particle_3D Behaviour
	for (short i = 0; i < m_particles.size(); ++i)
	{
		auto& particle = m_particles[i];

		if (!particle.m_isActive)
			continue;
		else if (particle.m_lifeRemaining <= 0)
		{
	/*		if (particle.m_rebirth)
				particle.Reborn();
			else*/
			{
				particle.m_isActive = false;

				// To be reused
				m_available_ParticleSlots.push(i);
			}
		}
		else
		{
			//particle.m_position += particle.m_velocity * m_MM.m_simulationSpeed;
			particle.m_position += particle.m_velocity * dt;

			// Update Velocity
			if (particle.m_gravity)
				particle.m_velocity.y += GRAVITY * dt;

			//particle.m_rotation += 0.01f * m_MM.m_simulationSpeed;

			// lifeTime and percentage of lifeTime remaining
			float lifePercent = particle.m_lifeRemaining / particle.m_lifeTime;
			//particle.m_lifeRemaining -= m_MM.m_simulationSpeed;
			particle.m_lifeRemaining -= dt;

			// Scale
			particle.m_scale.x = glm::mix(particle.m_scaleEnd.x, particle.m_scaleBegin.x, lifePercent);
			particle.m_scale.y = glm::mix(particle.m_scaleEnd.y, particle.m_scaleBegin.y, lifePercent);
			particle.m_scale.z = glm::mix(particle.m_scaleEnd.z, particle.m_scaleBegin.z, lifePercent);

			// Colour
			particle.m_colour = glm::mix(particle.m_colourEnd, particle.m_colourBegin, lifePercent);
			
			//LOG_INFO("Particle Rot X: {0}", particle.m_rotation.x);
			//LOG_INFO("Particle Rot Y: {0}", particle.m_rotation.y);
			//LOG_INFO("Particle Rot Z: {0}", particle.m_rotation.z);

			// Change the string in this function
			auto t = glm::translate(particle.m_position);
			auto s = glm::scale(particle.m_scale);
			auto r = glm::rotate(particle.m_rotation.x, glm::vec3(1.f, 0.f, 0.f))
				   * glm::rotate(particle.m_rotation.y, glm::vec3(0.f, 1.f, 0.f))
				   * glm::rotate(particle.m_rotation.z, glm::vec3(0.f, 0.f, 1.f));

			Tempest::Service<Tempest::RenderSystem>::Get().SubmitModel(particle, (t * r * s));
		}
	}
}

void Emitter_3D::ParticleSetUp(Particle_3D& particle)
{
	particle.m_position = m_GM.m_position;
	particle.m_isActive = true;
	//particle.m_rotation = Random::Float() * 2.0f * std::numbers::pi;

	// Velocity
	particle.m_velocity = m_PAM.m_startVelocity;
	particle.m_gravity = m_PAM.m_gravity;

	// Velocity Variations
	if (m_PAM.m_velocityVariation.x >= 1)
		particle.m_velocity.x += Random::Float() * m_PAM.m_velocityVariation.x;

	if (m_PAM.m_velocityVariation.y >= 1)
		particle.m_velocity.y += Random::Float() * m_PAM.m_velocityVariation.y;

	if (m_PAM.m_velocityVariation.z >= 1)
		particle.m_velocity.z += Random::Float() * m_PAM.m_velocityVariation.z;

	// Color
	particle.m_colourBegin = m_PAM.m_colourBegin;
	particle.m_colourEnd = m_PAM.m_colourEnd;

	// Lifetime
	particle.m_lifeTime = m_PAM.m_lifeTime;
	particle.m_lifeRemaining = m_PAM.m_lifeTime;

	// Scale Variation
	glm::vec3 scaleVariation{ 0.0f, 0.0f, 0.0f };

	if (m_PAM.m_scaleVariation.x >= 1)
		scaleVariation.x = Random::Float() * m_PAM.m_scaleVariation.x;

	if (m_PAM.m_scaleVariation.y >= 1)
		scaleVariation.y = Random::Float() * m_PAM.m_scaleVariation.y;

	if (m_PAM.m_scaleVariation.z >= 1)
		scaleVariation.z = Random::Float() * m_PAM.m_scaleVariation.z;

	particle.m_scaleBegin = m_PAM.m_scaleBegin + scaleVariation;
	particle.m_scaleEnd = m_PAM.m_scaleEnd;

	particle.m_renderingPath = m_RM.m_renderingPath;
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
			ParticleSetUp(particle);
		
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

void Emitter_3D::OnDeath()
{}