/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef PARTICLES_H
#define PARTICLES_H

#include <glm.hpp>

enum class ParticleType
{
	Circle = 0
	, Square = 1
};

struct Particle
{
	Particle();
	Particle(glm::vec2 pos, glm::vec2 vec, glm::vec4 colour, float size, float lifeTime, bool isActive, ParticleType particleType);

	glm::vec2 m_position;
	glm::vec2 m_velocity;

	float m_rotation;

	glm::vec4 m_colour;
	float m_size;
	float m_lifeTime;
	float m_lifeRemaining;
	bool  m_isActive;

	ParticleType m_type;
};

#endif // !PARTICLES_H
