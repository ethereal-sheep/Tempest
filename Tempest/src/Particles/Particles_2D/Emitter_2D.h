/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef EMITTER_H
#define EMITTER_H

#include <vector>
#include <queue>

#include "EmissionModules_2D.h"

// Note, glm::vectors are used to utilise the mix function
struct Emitter_2D
{
	// Particle_2D Management
	std::vector<Particle_2D> m_particles;
	std::queue<short> m_available_ParticleSlots;

	MainModule_2D m_MM;
	GameObjectModule_2D m_GM;
	ParticleArchetypeModule_2D m_PAM;
	EmissionModule_2D m_EM;
	RendererModule_2D m_RM;


	Emitter_2D();
	void Update();
	void ClearAllParticles();

protected:
	virtual void SelfUpdate();
	virtual void Emit(const int particleAmount);
};

#endif // !EMITTER_H
