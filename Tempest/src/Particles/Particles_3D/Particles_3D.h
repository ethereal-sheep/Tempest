/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef PARTICLES_3D_H
#define PARTICLES_3D_H

#include <glm.hpp>

struct Particle_3D
{
	Particle_3D();
	Particle_3D(glm::vec3 pos, glm::vec3 vec, glm::vec4 colour, float size, float lifeTime, bool isActive);

	glm::vec3 m_position;
	glm::vec3 m_velocity;

	float m_rotation;

	glm::vec4 m_colour;
	float m_size;
	float m_lifeTime;
	float m_lifeRemaining;
	bool m_isActive;
};

#endif // !PARTICLES_3D_H

