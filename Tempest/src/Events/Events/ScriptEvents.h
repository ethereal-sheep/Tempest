/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
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

		Simulate(id_t a, id_t d, id_t aa, id_t da) :
			attacker(a), defender(d), attack_action(aa), defend_action(da)
		{}

		id_t attacker;
		id_t defender;
		id_t attack_action;
		id_t defend_action;
	};

	struct Input : public Event
	{
		Input(id_t a, id_t d, int i = 0) : attacker(a), defender(d), input(i) {}

		id_t attacker;
		id_t defender;
		int input;
	};

	struct Roll : public Event
	{
		Roll(id_t o, id_t e, int i = 0) : owner(o), enemy(e), input(i) {}

		id_t owner;
		id_t enemy;
		int input;
	};

	struct Resolve : public Event
	{
		Resolve(id_t o, id_t e, int i = 0) : owner(o), enemy(e), input(i) {}

		id_t owner;
		id_t enemy;
		int input;
	};
}