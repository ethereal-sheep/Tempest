


#include "Smoke_Poof_Emitter_3D.h"

#include "../Random.h"

#define GRAVITY -9.8f

Smoke_Poof_Emitter_3D::Smoke_Poof_Emitter_3D()
{
	m_GM.m_position = glm::vec3{ 0.0f, 0.0f, 0.0f };
	m_MM.m_duration = 0.8f;
	m_GM.m_active = true;
	m_MM.m_preWarm = true;

	m_EM.m_spawnTimeInterval = 1.0f;
	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;

	m_EM.m_rateOverTime = 20;
	Emitter_3D::UpdateMaxParticle(1000);

	// Particle Architype values - without consideration for default ctor
	m_PAM.m_gravity = false;
	m_PAM.m_velocityStart = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityEnd = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityVariation = glm::vec3{ 3.0f, 1.0f, 3.0f };

	m_PAM.m_scaleBegin = glm::vec3{ 0.3f, 0.3, 0.3f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.f, 0.f, 0.f };

	m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.8f };
	m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 0.2f };

	m_PAM.m_lifeTime = 0.5f;
	m_RM.m_renderingPath = "Models/Cube.a";
}

void Smoke_Poof_Emitter_3D::Emit(const int particleAmount)
{
	// Emit only if enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initialisation of the particle
			Particle_3D particle;
			ExplosionEmitter_3D::ParticleSetUp(particle);

			particle.m_velocityEnd = glm::vec3{ 0.0f, 3.0f, 0.0f };

			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}

void Smoke_Poof_Emitter_3D::ParticleUpdate(const float dt)
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
			// lifeTime and percentage of lifeTime remaining
			float lifePercent = particle.m_lifeRemaining / particle.m_lifeTime;
			//particle.m_lifeRemaining -= m_MM.m_simulationSpeed;
			particle.m_lifeRemaining -= dt;

			//particle.m_position += particle.m_velocity * m_MM.m_simulationSpeed;
			particle.m_position += particle.m_velocity * dt;
			//particle.m_rotation += 0.01f * m_MM.m_simulationSpeed;

			// Update Velocity
			if (particle.m_gravity)
				particle.m_velocity.y += GRAVITY * dt;
			else
			{
				// Velocity
				particle.m_velocity.x = glm::mix(particle.m_velocityEnd.x, particle.m_velocityBegin.x, lifePercent);
				particle.m_velocity.z = glm::mix(particle.m_velocityEnd.z, particle.m_velocityBegin.z, lifePercent);

				if(lifePercent < 0.5f)
					particle.m_velocity.y = glm::mix(particle.m_velocityEnd.y, particle.m_velocityBegin.y, lifePercent);
			}

			// Scale
			particle.m_scale.x = glm::mix(particle.m_scaleEnd.x, particle.m_scaleBegin.x, lifePercent);
			particle.m_scale.y = glm::mix(particle.m_scaleEnd.y, particle.m_scaleBegin.y, lifePercent);
			particle.m_scale.z = glm::mix(particle.m_scaleEnd.z, particle.m_scaleBegin.z, lifePercent);

			// Colour
			particle.m_colour = glm::mix(particle.m_colourEnd, particle.m_colourBegin, lifePercent);
		}
	}
}