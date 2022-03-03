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
#include <string>

struct Particle_3D
{
	Particle_3D();
	//Particle_3D(glm::vec3 pos, glm::vec3 vec, glm::vec3 scale, glm::vec4 colour, float lifeTime, bool isActive);

	glm::vec3 m_position;
	glm::vec3 m_velocity;

	float m_rotation;

	glm::vec4 m_colourBegin, m_colourEnd;
	glm::vec4 m_colour;

	glm::vec3 m_scaleBegin, m_scaleEnd;
	glm::vec3 m_scale;

	float m_lifeTime;
	float m_lifeRemaining;
	bool m_isActive;

	bool m_rebirth; // Allows particle to restart its effect agin once death

	// Eg: Models/Cube.a
	std::string m_renderingPath;
};

#endif // !PARTICLES_3D_H

