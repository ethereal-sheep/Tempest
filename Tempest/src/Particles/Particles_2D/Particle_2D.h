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

struct Particle_2D
{
	Particle_2D();

	glm::vec2 m_position;

	glm::vec2 m_velocityBegin, m_velocityEnd;
	glm::vec2 m_velocity;

	float m_rotation;

	glm::vec4 m_colourBegin, m_colourEnd;
	glm::vec4 m_colour;

	float m_scaleBegin, m_scaleEnd;
	float m_size;

	float m_lifeTime;
	float m_lifeRemaining;
	bool  m_isActive;

	bool m_gravity;

	ParticleType m_type;
};

#endif // !PARTICLES_H
