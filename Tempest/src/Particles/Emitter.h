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

#include <glm.hpp>
#include <vector>
#include <queue>

#include "EmissionModules.h"

// Note, glm::vectors are used to utilise the mix function
struct Emitter
{
	// Particles Management
	std::vector<Particle> m_particles;
	std::queue<short> m_available_ParticleSlots;

	MainModule m_MM;
	GameObjectModule m_GM;
	ParticleArchetypeModule m_PAM;
	EmissionModule m_EM;
	RendererModule m_RM;

	Emitter();
	void Update();
	void ClearAllParticles();

protected:
	virtual void SelfUpdate();
	virtual void Emit(const int particleAmount);
};


#endif // !EMITTER_H
