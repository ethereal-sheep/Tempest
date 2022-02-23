/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Particle_2D.h"

Particle_2D::Particle_2D()
	: m_position{ 0.0f, 0.0f }
	, m_velocity{ 0.0f, 0.0f }
	, m_rotation{ 0.0f }
	, m_colour{ 0.0f, 0.0f, 0.0f, 0.0f }
	, m_size{ 1.0f }
	, m_lifeTime{ 1.0f }
	, m_lifeRemaining{ 1.0f }
	, m_isActive{ false }
	, m_type{ ParticleType::Circle }
{
}

Particle_2D::Particle_2D(glm::vec2 pos, glm::vec2 vec, glm::vec4 colour, float size, float lifeTime, bool isActive, ParticleType particleType)
	: m_position{ pos }
	, m_velocity{ vec }
	, m_rotation{ 0.0f }
	, m_colour { colour }
	, m_size {size}
	, m_lifeTime {lifeTime}
	, m_lifeRemaining {lifeTime}
	, m_isActive {isActive}
	, m_type{ particleType }
{
}