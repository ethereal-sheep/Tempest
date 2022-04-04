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

	, m_velocityBegin {0.f, 0.f}
	, m_velocityEnd {0.f, 0.f}
	, m_velocity{ 0.f, 0.f }

	, m_rotation{ 0.0f }

	, m_colourBegin {0.f, 0.f, 0.f, 0.f }
	, m_colourEnd{ 0.f, 0.f, 0.f, 0.f }
	, m_colour{ 0.0f, 0.0f, 0.0f, 0.0f }

	, m_scaleBegin { 1.0f }
	, m_scaleEnd { 1.0f }
	, m_size{ 1.0f }

	, m_lifeTime{ 1.0f }
	, m_lifeRemaining{ 1.0f }

	, m_isActive{ false }
	, m_gravity{ false }

	, m_type{ ParticleType::Circle }
{
}
