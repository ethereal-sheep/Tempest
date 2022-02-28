


#include "InteractiveParticle_3D.h"

void InteractiveParticle_3D::Emit(const int particleAmount)
{
	// Emit only if there is enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		// Initialisation of the particle
		Particle_3D particle;

		particle.m_position = m_GM.m_position;
		particle.m_isActive = true;
		//particle.m_rotation = Random::Float() * 2.0f * std::numbers::pi;

		// Velocity - RNG
		particle.m_velocity = m_PAM.m_startVelocity;

		// Spawn Positoin of the particle

	}
}