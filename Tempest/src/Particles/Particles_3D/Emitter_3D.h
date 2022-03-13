/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef EMITTER_3D_H
#define EMITTER_3D_H

#include "Particles_3D.h"
#include "EmissionModules_3D.h"

#include <vector>
#include <queue>

struct Emitter_3D
{
	std::vector<Particle_3D> m_particles;
	std::queue<short> m_available_ParticleSlots;

	MainModule_3D				m_MM;
	GameObjectModule_3D			m_GM;
	ParticleArchetypeModule_3D	m_PAM;
	EmissionModule_3D			m_EM;
	RendererModule_3D			m_RM;
	
	Emitter_3D();
	void Update(const float dt);
	void ClearAllParticles();

protected:
	virtual void SelfUpdate(const float dt);
	virtual void Emit(const int particleAmount);
	virtual void ParticleSetUp(Particle_3D& particle);
	virtual void OnDeath();
};

#endif // !EMITTER_3D_H
