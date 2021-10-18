/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Core.h"
#include "Events/Events/IEvent.h"

namespace Tempest
{
	struct Simulate : public Event 
	{

		Simulate(id_t a, id_t d) :
			attacker(a), defender(d)
		{}

		id_t attacker;
		id_t defender;
	};

	struct Input : public Event 
	{
		Input(id_t a, id_t d, int i = 0) : attacker(a), defender(d), input(i) {}

		id_t attacker;
		id_t defender;
		int input;
	};
}