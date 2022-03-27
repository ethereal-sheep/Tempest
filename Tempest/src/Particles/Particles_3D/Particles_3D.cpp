/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Particles_3D.h"

Particle_3D::Particle_3D()
	: m_position { 0.0f, 0.0f, 0.0f }
	, m_rotation{ 0.0f, 0.0f, 0.0f }

	, m_velocity { 0.0f, 0.0f, 0.0f }
	, m_velocityBegin{ 0.0f, 0.0f, 0.0f }
	, m_velocityEnd{ 0.0f, 0.0f, 0.0f }

	, m_colour { 0.0f, 0.0f, 0.0f, 0.0f }
	, m_colourBegin{ 0.0f, 0.0f, 0.0f, 0.0f }
	, m_colourEnd{ 0.0f, 0.0f, 0.0f, 0.0f }

	, m_scale { 1.0f, 1.0f, 1.0f }
	, m_scaleBegin { 1.0f, 1.0f, 1.0f }
	, m_scaleEnd { 0.0f, 0.0f, 0.0f }

	, m_lifeTime { 1.0f }
	, m_lifeRemaining{ 1.0f }

	, m_isActive { false }
	, m_gravity { false }

	, m_emissive { false }
{}
