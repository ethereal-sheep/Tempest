/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef EXPLOSION_EMITTER_H
#define EXPLOSION_EMITTER_H

#include "Emitter.h"

#include <gtx/transform.hpp>


struct ExplosionEmitter final : public Emitter
{
	void Emit(const int particleAmount) override;
};

#endif // ! EXPLOSION_EMITTER_H

