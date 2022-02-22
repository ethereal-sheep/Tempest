/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef EXPLOSION_EMITTER_3D_H
#define EXPLOSION_EMITTER_3D_H

#include "Emitter.h"

#include <gtx/transform.hpp>


struct ExplosionEmitter_3D final : public Emitter
{
	ExplosionEmitter_3D();
	void Emit(const int particleAmount) override;

	glm::vec3 m_RandomSpawnDirection;
};

#endif // ! EXPLOSION_EMITTER_H

